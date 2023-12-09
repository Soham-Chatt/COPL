#include "parser.h"

VariableNode::VariableNode(const std::string& name) : name(name) {}

std::string VariableNode::to_string() const {
  return name;
}

LambdaNode::LambdaNode(const std::string& param, std::unique_ptr<Node> body)
    : param(param), body(std::move(body)) {}

ApplicationNode::ApplicationNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
    : left(std::move(left)), right(std::move(right)) {}

char Parser::current_char() {
  return pos < input.size() ? input[pos] : '\0';
}

void Parser::skip_whitespace() {
  while (pos < input.size() && std::isspace(input[pos])) {
    ++pos;
  }
}
std::string Parser::parse_variable() {
  skip_whitespace();
  std::string var;
  if (pos < input.size() && std::isalpha(input[pos])) {
    var += input[pos++];
  } else {
    throw std::runtime_error("Variable must start with an alphabetic character");
  }
  while (pos < input.size() && (std::isalpha(input[pos]) || std::isdigit(input[pos]))) {
    var += input[pos++];
  }
  return var;
}

std::unique_ptr<Node> Parser::parse_expression() {
  skip_whitespace();
  auto expr = parse_atom();
  while (true) {
    skip_whitespace();
    if (current_char() == '(' || std::isalpha(current_char())) {
      auto right = parse_atom();
      expr = std::make_unique<ApplicationNode>(std::move(expr), std::move(right));
    } else {
      break;
    }
  }
  return expr;
}

std::unique_ptr<Node> Parser::parse_atom() {
  skip_whitespace();
  char ch = current_char();
  if (ch == '\\') {
    return parse_lambda();
  } else if (ch == '(') {
    ++pos;
    auto node = parse_expression();
    skip_whitespace();
    if (current_char() != ')') {
      throw std::runtime_error("Expected ')'");
    }
    ++pos;
    return node;
  } else if (std::isalpha(ch)) {
    return std::make_unique<VariableNode>(parse_variable());
  } else {
    throw std::runtime_error("Unexpected character encountered");
  }
}

std::unique_ptr<Node> Parser::parse_lambda() {
  ++pos; // Skip the '\' character
  std::string param = parse_variable(); // Parse the parameter name
  skip_whitespace();

  auto body = parse_atom(); // Parse the body of the lambda

  return std::make_unique<LambdaNode>(param, std::move(body));
}


std::unique_ptr<Node> Parser::parse(const std::string& input_str) {
  input = input_str;
  pos = 0;
  auto result = parse_expression();
  if (pos < input.size()) {
    throw std::runtime_error("Unexpected character at end of input");
  }
  return result;
}

std::string LambdaNode::to_string() const {
  return "\\" + param + "." + body->to_string();
}

std::string ApplicationNode::to_string() const {
  return "(" + left->to_string() + " " + right->to_string() + ")";
}

void print_tree(const Node* node, int depth) {
  if (!node) return;

  for (int i = 0; i < depth; ++i) {
    std::cout << "  ";
  }

  std::cout << node->to_string() << '\n';
  if (auto l = dynamic_cast<const LambdaNode*>(node)) {
    print_tree(l->body.get(), depth + 1);
  } else if (auto a = dynamic_cast<const ApplicationNode*>(node)) {
    print_tree(a->left.get(), depth + 1);
    print_tree(a->right.get(), depth + 1);
  }
}

std::string generate_dot(const Node* node) {
  static int counter = 0;
  std::ostringstream out;
  if (!node) return "";
  int cur_id = counter++;
  std::string label = node->to_string();

  if (auto l = dynamic_cast<const LambdaNode*>(node)) {
    int body_id = counter;
    out << generate_dot(l->body.get());
    out << cur_id << " -> " << body_id << ";\n";
  } else if (auto a = dynamic_cast<const ApplicationNode*>(node)) {
    int left_id = counter;
    out << generate_dot(a->left.get());
    out << cur_id << " -> " << left_id << ";\n";

    int right_id = counter;
    out << generate_dot(a->right.get());
    out << cur_id << " -> " << right_id << ";\n";
  }

  out << cur_id << " [label=\"" << label << "\"];\n";
  return out.str();
}
