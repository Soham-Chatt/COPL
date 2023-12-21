// parser.cc
#include "parser.h"
#include <sstream>

VariableNode::VariableNode(const std::string &name) : name(name) {}

std::string VariableNode::to_string() const {
  return name;
}

LambdaNode::LambdaNode(const std::string &param, Node *body) : param(param), body(body) { /*...*/ }

std::string LambdaNode::to_string() const {
  return "\\" + param + " (" + body->to_string() + ")";
}

LambdaNode::~LambdaNode() {
  delete body;
}

ApplicationNode::ApplicationNode(Node *left, Node *right) : left(left), right(right) {}

std::string ApplicationNode::to_string() const {
  return "(" + left->to_string() + " " + right->to_string() + ")";
}

ApplicationNode::~ApplicationNode() {
  delete left;
  delete right;
}

char Parser::current_char() {
  return pos < input.size() ? input[pos] : '\0';
}

void Parser::skip_whitespace() {
  while (pos < input.size() && std::isspace(input[pos])) {
    ++pos;
  }
}

bool is_lambda_char(wchar_t ch) {
  return ch == '\\';
}

bool is_variable_start_char(wchar_t ch) {
  return std::isalpha(ch);
}

bool is_open_bracket(wchar_t ch) {
  return ch == '(';
}

std::string Parser::parse_variable() {
  // ⟨var⟩ ::= ⟨alphanum⟩ | ⟨var⟩ ⟨alphanum⟩
  skip_whitespace();
  std::string var;
  if (pos < input.size() && std::isalpha(input[pos])) {
    var += input[pos];
    ++pos;
  } else {
    throw std::runtime_error("Variable must start with an alphabetic character");
  }

  while (pos < input.size() && (std::isalpha(input[pos]) || std::isdigit(input[pos]))) {
    var += input[pos];
    ++pos;
  }
  return var;
}

Node *Parser::parse_expression() {
  // ⟨expr⟩ ::= ⟨var⟩ | '(' ⟨expr⟩ ')' | '\' ⟨var⟩ ⟨expr⟩ | ⟨expr⟩ ⟨expr⟩
  skip_whitespace();

  Node *expr = parse_atom();

  while (true) {
    skip_whitespace();
    // Check if the current character is the start of a new atom
    if (current_char() == '(' || std::isalpha(current_char())) {
      Node *right = parse_atom();
      expr = new ApplicationNode(expr, right);
    } else {
      break; // No more applications, exit loop
    }
  }

  return expr;
}

Node *Parser::parse_atom() {
  // ⟨atom⟩ ::= ⟨var⟩ | '(' ⟨expr⟩ ')' | '\' ⟨var⟩ ⟨expr⟩
  skip_whitespace();
  wchar_t ch = current_char();

  if (is_lambda_char(ch)) {
    return parse_lambda();
  } else if (is_open_bracket(ch)) {
    ++pos; // consume '('
    Node *node = parse_expression(); // parse expression within the brackets
    skip_whitespace();
    if (current_char() == ')') {
      ++pos; // consume ')'
    } else {
      throw std::runtime_error("Expected ')'");
    }
    return node; // the expression inside the brackets is treated as one atom
  } else if (is_variable_start_char(ch)) {
    return new VariableNode{parse_variable()};
  } else {
    throw std::runtime_error("Unexpected character encountered");
  }
}


Node *Parser::parse_lambda() {
  // ⟨lambda⟩ ::= '\' ⟨var⟩ ⟨expr⟩
  ++pos; // Skip the '\' character
  std::string param = parse_variable(); // Parse the parameter name
  skip_whitespace();
  if (current_char() == '.') {
    ++pos; // Skip the '.' character
  }
  Node *body = parse_atom(); // Parse the body of the lambda
  return new LambdaNode{param, body};
}

Node *Parser::parse(const std::string &input_str) {
  input = input_str;
  pos = 0;
  Node *result = parse_expression();

  skip_whitespace();
  if (pos < input.size()) {
    throw std::runtime_error("Unexpected character at end of input");
  }

  return result;
}

std::string Parser::generate_dot(Node *node) {
  static int counter = 0;
  std::ostringstream out;

  if (!node) return "";
  int cur_id = counter++;
  std::string label;

  if (auto v = dynamic_cast<VariableNode *>(node)) {
    label = "Variable: " + v->name;
  } else if (auto l = dynamic_cast<LambdaNode *>(node)) {
    label = "Lambda: " + l->param;
    int body_id = counter;
    out << generate_dot(l->body);
    out << cur_id << " -> " << body_id << ";\n";
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
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
