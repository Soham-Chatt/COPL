#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <memory>
#include <cctype>
#include <sstream>

class Node {
public:
  virtual ~Node() = default;
  virtual std::string to_string() const = 0;
};

class VariableNode : public Node {
public:
  std::string name;
  VariableNode(const std::string& name);
  std::string to_string() const override;
};

class LambdaNode : public Node {
public:
  std::string param;
  std::unique_ptr<Node> body;
  LambdaNode(const std::string& param, std::unique_ptr<Node> body);
  std::string to_string() const override;
};

class ApplicationNode : public Node {
public:
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  ApplicationNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
  std::string to_string() const override;
};

class Parser {
public:
  std::unique_ptr<Node> parse(const std::string& input_str);
private:
  std::string input;
  size_t pos = 0;

  char current_char();
  void skip_whitespace();
  std::string parse_variable();
  std::unique_ptr<Node> parse_expression();
  std::unique_ptr<Node> parse_atom();
  std::unique_ptr<Node> parse_lambda();
};

void print_tree(const Node* node, int depth = 0);
std::string generate_dot(const Node* node);

#endif // PARSER_H
