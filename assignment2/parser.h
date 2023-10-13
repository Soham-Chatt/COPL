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
    int depth;

    Node() {
        depth = 0;
    }

    virtual ~Node() = default;
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
    Node* body;
    LambdaNode(const std::string& param, Node* body);

    std::string to_string() const override;

    ~LambdaNode();
};

class ApplicationNode : public Node {
public:
    Node* left;
    Node* right;
    ApplicationNode(Node* left, Node* right);

    std::string to_string() const override;

    ~ApplicationNode();
};

class Parser {
public:
    Node* parse(const std::string& input_str);
private:
    std::string input;
    size_t pos = 0;

    char current_char();
    void skip_whitespace();
    std::string parse_variable();
    Node* parse_expression();
    Node* parse_atom();
    Node* parse_lambda();
};

void print_tree(Node* node);
void assign_depth(Node* node, int depth = 0);
std::string generate_dot(Node* node);
#endif //PARSER_H
