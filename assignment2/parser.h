// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <cctype>

class Node {
public:
  virtual std::string to_string() const = 0;

  virtual Node *copy() const = 0;

  virtual ~Node() = default;
};

class VariableNode : public Node {
public:
  std::string name;

  VariableNode(const std::string &name);

  std::string to_string() const override;

  Node *copy() const override {
    return new VariableNode(*this);
  }
};

class LambdaNode : public Node {
public:
  std::string param;
  Node *body;

  LambdaNode(const std::string &param, Node *body);

  std::string to_string() const override;

  Node *copy() const override {
    return new LambdaNode(param, body->copy());
  }

  ~LambdaNode();
};

class ApplicationNode : public Node {
public:
  Node *left;
  Node *right;

  ApplicationNode(Node *left, Node *right);

  std::string to_string() const override;

  Node *copy() const override {
    return new ApplicationNode(left->copy(), right->copy());
  }

  ~ApplicationNode();
};


class Parser {
public:
  Node *parse(const std::string &input_str);

  std::string generate_dot(Node *node);

private:
  std::string input;
  size_t pos = 0;

  char current_char();

  void skip_whitespace();

  std::string parse_variable();

  Node *parse_expression();

  Node *parse_atom();

  Node *parse_lambda();

};


#endif //PARSER_H
