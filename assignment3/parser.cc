// parser.cc
#include "parser.h"
#include <sstream>

VariableNode::VariableNode(const std::string& name) : name(name) {}
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
    std::cout << "Current: " << current << std::endl;

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
    } else if (current == '^') {
      tokens.push_back({TokenType::Caret, "^"});
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
    } else {
      throw std::runtime_error("Unexpected token");
    }
  }

  tokens.push_back({TokenType::End, ""});
}

Node* Parser::parse_judgement() {
  Node* expr = parse_expression();

  if (tokens[pos].value == ":") {
    pos++; // consume ':'
    
    Node* type = parse_type();
    return new JudgementNode(expr, type);
  } else {
    return expr;
  }
}

Node* Parser::parse_type() {
  if (tokens[pos].type == TokenType::UVar) {
    // Handle untyped variable
    return new TypeNode(tokens[pos++].value);
  } else if (tokens[pos].type == TokenType::LParen) {
    // Handle parentheses
    pos++; // Consume '('
    Node* innerType = parse_type(); // Parse the inner type expression

    if (tokens[pos].type != TokenType::RParen) {
      throw std::runtime_error("Expected ')'");
    }
    pos++; // Consume ')'

    // Check for '->' after ')'
    if (tokens[pos].type == TokenType::Arrow) {
      pos++; // Consume '->'
      Node* rightType = parse_type();
      return new TypeNode(innerType->to_string() + " -> " + rightType->to_string());
    } else {
      return innerType;
    }
  } else {
    // Handle type -> type
    Node* leftType = parse_type();
    if (tokens[pos].type != TokenType::Arrow) {
      throw std::runtime_error("Expected '->' in type expression");
    }
    pos++; // Consume '->'
    Node* rightType = parse_type();
    return new TypeNode(leftType->to_string() + " -> " + rightType->to_string());
  }
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
  
  std::string str = tokens[pos].value;

  if (tokens[pos].type == TokenType::LVar || tokens[pos].type == TokenType::UVar) {
    return new VariableNode{tokens[pos++].value};
  } else if (tokens[pos].type == TokenType::LParen){
    pos++; // consume '('
    Node* node = parse_expression(); // parse expression within the brackets
    if (tokens[pos].type == TokenType::RParen) {
      pos++; // consume ')'
    } else {
      throw std::runtime_error("Expected ')'");
    }
    return node; // the expression inside the brackets is treated as one atom
  } else if (tokens[pos].type == TokenType::Lambda) {
    return parse_lambda();
  } else {
    std::cout << "Unexpected character: " << tokens[pos].value << std::endl;
    throw std::runtime_error("Unexpected character encountered");
  }
}


Node* Parser::parse_lambda() {
  pos++; // Skip the '\' character
  if (tokens[pos].type != TokenType::LVar) {
    throw std::runtime_error("Expected lambda parameter");
  }
  std::string param = tokens[pos++].value; // Consume the parameter

  Node* type = nullptr;
  if (tokens[pos].type == TokenType::Caret) {
    pos++; // Consume '^'
    type = parse_type(); // Parse the type
  }

  // Consume '.' if present
  if (tokens[pos].type == TokenType::Dot) {
    pos++; // Consume '.'
  }

  Node* body = parse_expression(); // Parse the body of the lambda
  return new LambdaNode(param, type, body); // Pass the type to the constructor
}

Node* Parser::parse(const std::string& input_str) {
    input = input_str;
    pos = 0;
    std::cout << "Expression: " << input << std::endl;
    tokenize(input);
    Node* result = parse_judgement();

    if (pos < tokens.size() && tokens[pos].type != TokenType::End) {
        throw std::runtime_error("Unexpected character at end of input");
    }

    return result;
}

std::string generate_dot(Node* node) {
  static int counter = 0;
  std::ostringstream out;

  if (!node) return "";
  int cur_id = counter++;
  std::string label;

  if (auto v = dynamic_cast<VariableNode*>(node)) {
    label = "Variable: " + v->name;
  } else if (auto l = dynamic_cast<LambdaNode*>(node)) {
    label = "Lambda: " + l->param;
    int body_id = counter;
    out << generate_dot(l->body);
    out << cur_id << " -> " << body_id << ";\n";
  } else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
    label = "Application";
    int left_id = counter;
    out << generate_dot(a->left);
    out << cur_id << " -> " << left_id << ";\n";

    int right_id = counter;
    out << generate_dot(a->right);
    out << cur_id << " -> " << right_id << ";\n";
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
