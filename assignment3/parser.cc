// parser.cc
#include "parser.h"
#include <sstream>

VariableNode::VariableNode(const std::string& name)
    : name(name) {}


std::string VariableNode::to_string() const {
  return name;
}

LambdaNode::LambdaNode(const std::string& param, Node* type, Node* body)
    : param(param), type(type), body(body) {}

std::string LambdaNode::to_string() const {
  std::string typeStr = type ? type->to_string() : "";
  return "\\" + param + (typeStr.empty() ? "" : " ^ " + typeStr) + "." + body->to_string();
}

LambdaNode::~LambdaNode() {
  delete body;
  delete type;
}

ApplicationNode::ApplicationNode(Node* left, Node* right) : left(left), right(right) {}
std::string ApplicationNode::to_string() const {
    return "(" + left->to_string() + " " + right->to_string() + ")";
}
ApplicationNode::~ApplicationNode() {
    delete left;
    delete right;
}

TypeNode::TypeNode(const std::string& body) : body(body) {}

std::string TypeNode::to_string() const {
  return body;
}

JudgementNode::JudgementNode(Node *left, Node *right) : left(left), right(right) {}

std::string JudgementNode::to_string() const {
  return "(" + left->to_string() + " : " + right->to_string() + ")";
}

JudgementNode::~JudgementNode() {
  delete left;
  delete right;
}

void Parser::tokenize(const std::string& inputString) {
  size_t lpos = 0;
  while (lpos < inputString.length()) {
    char current = inputString[lpos];
    if (std::isspace(current)) {
      lpos++;
      continue;
    }

    if (current == '\\') {
      tokens.push_back({TokenType::Lambda, "\\"});
      lpos++;
    } else if (current == '(') {
      tokens.push_back({TokenType::LParen, "("});
      lpos++;
    } else if (current == ')') {
      tokens.push_back({TokenType::RParen, ")"});
      lpos++;
    } else if (current == '.') {
      tokens.push_back({TokenType::Dot, "."});
      lpos++;
    } else if (current == ':') {
      tokens.push_back({TokenType::Colon, ":"});
      lpos++;
    } else if (std::isalpha(current)) {
      std::string value;
      do {
        value += inputString[lpos++];
      } while (lpos < inputString.length() && std::isalnum(inputString[lpos]));

      if (std::isupper(value[0])) {
        tokens.push_back({TokenType::UVar, value});
      } else {
        tokens.push_back({TokenType::LVar, value});
      }
    } else if (current == '-' && lpos + 1 < inputString.length() && inputString[lpos + 1] == '>') {
      tokens.push_back({TokenType::Arrow, "->"});
      lpos += 2; // Skip past '->'
    }  else if (current == '^') {
      tokens.push_back({TokenType::Caret, "^"});
      lpos++;
    } else {
      throw std::runtime_error("Unexpected token");
    }
  }

  tokens.push_back({TokenType::End, ""});
}

Node* Parser::parse_judgement() {
  Node* expr = parse_expression();

  if (tokens[pos].type != TokenType::Colon) {
    throw std::runtime_error("Missing type for judgement");
  }
  pos++; // consume ':'

  Node* type = parse_type();
  return new JudgementNode(expr, type);
}


Node* Parser::parse_expression() {
  Node* expr = parse_atom();

  while (true) {
    // Check if the current character is the start of a new atom
    if (tokens[pos].type == TokenType::LParen || std::isalpha(tokens[pos].value[0])) {
      Node* right = parse_atom();
      expr = new ApplicationNode(expr, right);
    } else {
      break; // No more applications, exit loop
    }
  }

  return expr;
}

Node* Parser::parse_atom() {
  if (tokens[pos].type == TokenType::LVar) {
    std::string varName = tokens[pos++].value; // Consume the LVar

    return new VariableNode(varName);
  } else if (tokens[pos].type == TokenType::LParen){
    pos++; // consume '('
    Node* node = parse_expression(); // parse expression within the brackets
    if (tokens[pos].type == TokenType::RParen) {
      pos++; // consume ')'
    } else {
      throw std::runtime_error("Expected ')' but got '" + tokens[pos].value + "' instead.");
    }
    return node; // the expression inside the brackets is treated as one atom
  } else if (tokens[pos].type == TokenType::Lambda) {
    return parse_lambda();
  } else {
    throw std::runtime_error("Unexpected character encountered: " + tokens[pos].value + "");
  }
}

Node* Parser::parse_lambda() {
  pos++; // Skip the '\' character
  if (tokens[pos].type != TokenType::LVar) {
    throw std::runtime_error("Expected lambda parameter");
  }
  std::string param = tokens[pos].value;
  pos++; // Consume the parameter
  if (tokens[pos].type != TokenType::Caret) {
    throw std::runtime_error("Missing type for lambda parameter");
  }
  pos++; // Consume '^'

  Node* type = parse_type(); // Parse the type

  Node* body = parse_expression(); // Parse the body of the lambda
  return new LambdaNode(param, type, body); // Pass the type to the constructor
}

Node* Parser::parse_single_type() {
//  std::cout << "Parsing single type: " << tokens[pos].value << '\n';
  if (tokens[pos].type == TokenType::UVar) {
    return new TypeNode(tokens[pos++].value);
  } else if (tokens[pos].type == TokenType::LParen) {
    pos++; // Consume '('
    Node* innerType = parse_type(); // Parse the inner type expression

    if (tokens[pos].type != TokenType::RParen) {
      throw std::runtime_error("Expected ')' but got '" + tokens[pos].value + "' instead.");
    }
    pos++; // Consume ')'
    return innerType;
  } else {
    throw std::runtime_error("Unexpected type token");
  }
}

Node* Parser::parse_type() {
  Node* leftType = parse_single_type();
  // Check for '->' to handle function types
  while (tokens[pos].type == TokenType::Arrow) {
    pos++; // Consume '->'
    Node* rightType = parse_single_type();
    leftType = new TypeNode(leftType->to_string() + " -> " + rightType->to_string());
  }

  return leftType;
}

Node* Parser::parse(const std::string& input_str) {
    input = input_str;
    pos = 0;
    tokens.clear();
    tokenize(input);
    Node* result = parse_judgement();
    if (!getDerivation(result)) throw std::runtime_error("Derivation incorrect");

    if (pos < tokens.size() && tokens[pos].type != TokenType::End) {
        throw std::runtime_error("Unexpected character at end of input");
    }

    return result;
}

bool Parser::getDerivation(Node* root) {
  Node* left = getType(dynamic_cast<JudgementNode*>(root)->left);
  Node* right = dynamic_cast<JudgementNode*>(root)->right;
  return (left->to_string() == right->to_string());
}

std::pair<std::string, std::string> extractTypes(const std::string& str) {
  size_t pos = str.find("->");
  if (pos == std::string::npos) {
    throw std::runtime_error("Invalid format. Expected '[Type] -> [Type]'");
  }
  std::string firstType = str.substr(0, pos - 1);
  std::string secondType = str.substr(
      pos + 3);

  return {firstType, secondType};
}

Node* Parser::getType(Node* root) {
  if (auto l = dynamic_cast<LambdaNode*>(root)) {
    gamma_stack.push({l->param, l->type->to_string()});
    Node* temp = new TypeNode(l->type->to_string() + " -> " + getType(l->body)->to_string());
    return temp;
  } else if (auto a = dynamic_cast<ApplicationNode*>(root)) {
    Node* left = getType(a->left);
    Node* right = getType(a->right);
    std::pair<std::string, std::string> types = extractTypes(left->to_string());
    if (types.first != right->to_string()) throw std::runtime_error("Type mismatch");
    Node* temp = new TypeNode(types.second);
    return temp;
  } else if (auto v = dynamic_cast<VariableNode*>(root)) {
    if (gamma_stack.empty()) throw std::runtime_error("Variable has unknown type");
    if (v->to_string() != gamma_stack.top().var) throw std::runtime_error("Variable not in scope");
    std::string type = gamma_stack.top().type;
    gamma_stack.pop();
    return new TypeNode(type);
  } else {
    throw std::runtime_error("Unexpected node type: " + root->to_string());
  }
}

std::string Parser::generate_dot(Node* node, int parent_id = -1) {
  static int counter = 0;
  std::ostringstream out;

  if (!node) return "";

  int cur_id = counter++;
  std::string label = node->to_string();

  if (parent_id != -1) {
    // Draw an edge from the parent node to the current node
    out << parent_id << " -> " << cur_id << ";\n";
  }

  if (auto l = dynamic_cast<LambdaNode*>(node)) {
    // LambdaNode has a body and optionally a type
    out << generate_dot(l->type, cur_id);
    out << generate_dot(l->body, cur_id);
  } else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
    // ApplicationNode has left and right children
    out << generate_dot(a->left, cur_id);
    out << generate_dot(a->right, cur_id);
  } else if (auto j = dynamic_cast<JudgementNode*>(node)) {
    // JudgementNode has left and right children
    out << generate_dot(j->left, cur_id);
    out << generate_dot(j->right, cur_id);
  }

  out << cur_id << " [label=\"" << label << "\"];\n";
  return out.str();
}


void print_tree(Node* node) {
    if (!node) return;

    for(int i = 0; i < node->depth; ++i) {
      std::cout << "  ";
    }

    if (auto v = dynamic_cast<VariableNode*>(node)) {
        // If the node is a VariableNode, print the variable name
      std::cout << "Variable: " << v->name << '\n';
    } else if (auto l = dynamic_cast<LambdaNode*>(node)) {
        // If the node is a LambdaNode, recursively print the body of the lambda
      std::cout << "Lambda: " << l->param << '\n';
      print_tree(l->body);
    } else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
        // If the node is ApplicationNode, recursively print both the left and right child nodes
      std::cout << "Application:\n";
      print_tree(a->left);
      print_tree(a->right);
    }
}
