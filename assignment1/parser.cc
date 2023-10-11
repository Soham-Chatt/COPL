// parser.cc
#include "parser.h"

VariableNode::VariableNode(const std::string& name) : name(name) {}
std::string VariableNode::to_string() const {
    return name;
}

LambdaNode::LambdaNode(const std::string& param, Node* body) : param(param), body(body) { /*...*/ }
std::string LambdaNode::to_string() const {
    return "\\" + param + "." + body->to_string();
}
LambdaNode::~LambdaNode() {
    delete body;
}

ApplicationNode::ApplicationNode(Node* left, Node* right) : left(left), right(right) {}
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

std::string Parser::parse_variable() {
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

Node* Parser::parse_expression() {
    skip_whitespace();

    Node* node = parse_atom();

    while (true) {
        skip_whitespace();
        // If the next character is a '(' or a letter then it must be the start of another expression
        // Parse the right node because function node is left child and argument node is right child
        if (current_char() == '(' || std::isalpha(current_char())) {
            Node* right = parse_atom();
            node = new ApplicationNode{node, right};
        } else {
            break;
        }
    }

    return node;
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

Node* Parser::parse_atom() {
    skip_whitespace();

    wchar_t ch = current_char();
    if (is_lambda_char(ch)) {
        return parse_lambda();
    }
    else if (is_open_bracket(ch)) {
        ++pos;
        Node* node = parse_expression();
        skip_whitespace();
        if (current_char() == ')') {
            ++pos;
            return node;
        }
        else {
            throw std::runtime_error("Expected ')'");
        }
    }
    else if (is_variable_start_char(ch)) {
        return new VariableNode{parse_variable()};
    }
    else {
        throw std::runtime_error("Unexpected character encountered");
    }
}


Node* Parser::parse_lambda() {
    ++pos; // Skip the '\' character
    std::string param = parse_variable(); // Parse the parameter name
    skip_whitespace();
    if (current_char() == '.') {
        ++pos; // Skip the '.' character
    }
    Node* body = parse_expression(); // Parse the body of the lambda
    return new LambdaNode{param, body};
}

Node* Parser::parse(const std::string& input_str) {
    input = input_str;
    pos = 0;
    Node* result = parse_expression();

    skip_whitespace();
    if (pos < input.size()) {
        throw std::runtime_error("Unexpected character at end of input");
    }

    return result;
}

void print_tree(Node* node, int depth) {
    if (!node) return;

    for(int i = 0; i < depth; ++i) {
      std::cout << "  ";
    }

    if (auto v = dynamic_cast<VariableNode*>(node)) {
        // If the node is a VariableNode, print the variable name
        std::cout << "Variable: " << v->name << '\n';
    } else if (auto l = dynamic_cast<LambdaNode*>(node)) {
        // If the node is a LambdaNode, recursively print the body of the lambda
        std::cout << "Lambda: " << l->param << '\n';
        print_tree(l->body, depth+1);
    } else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
        // If the node is ApplicationNode, recursively print both the left and right child nodes
        std::cout << "Application:\n";
        print_tree(a->left, depth+1);
        print_tree(a->right, depth+1);
    }
}
