// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <cctype>
#include <stack>

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
    return new VariableNode(name);
  }
};

class LambdaNode : public Node {
public:
  std::string param;
  Node *type;
  Node *body;

  LambdaNode(const std::string &param, Node *type, Node *body);

  std::string to_string() const override;

  Node *copy() const override {
    return new LambdaNode(param, type ? type->copy() : nullptr, body->copy());
  }

  ~LambdaNode() override;
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

  ~ApplicationNode() override;
};

class TypeNode : public Node {
public:
  std::string body;

  TypeNode(const std::string &body);

  std::string to_string() const override;

  Node *copy() const override {
    return new TypeNode(*this);
  }
};

class JudgementNode : public Node {
public:
  Node *left;
  Node *right;

  std::string to_string() const override;

  Node *copy() const override {
    return new JudgementNode(left->copy(), right->copy());
  }

  ~JudgementNode() override;

public:
  JudgementNode(Node *left, Node *right);
};

enum class TokenType {
  Lambda, Arrow, LParen, RParen, Dot, End, LVar, UVar, Caret, Colon
};

struct Token {
  TokenType type;
  std::string value;
};

struct Gamma {
  std::string var;
  std::string type;
};

class Parser {
public:
  Node *parse(const std::string &input_str);

  void tokenize(const std::string &inputString);

  std::string generate_dot(Node *node, int parent_id);

private:
  std::string input;
  size_t pos = 0;
  std::vector<Token> tokens;
  std::stack<Gamma> gamma_stack;

  Node *parse_expression();

  Node *parse_atom();

  Node *parse_lambda();

  Node *parse_judgement();

  Node *parse_type();

  Node *parse_single_type();

  bool get_derivation(Node *root);

  Node *get_type(Node *root);

  std::pair<std::string, std::string> extract_types(const std::string &str);
};

#endif //PARSER_H
