#include <string>
#include <iostream>
#include <vector>
#include <cctype>

class Node {
public:
    virtual ~Node() = default;
};

class VariableNode : public Node {
public:
    std::string name;
    VariableNode(const std::string& name) : name(name) {}
};

class LambdaNode : public Node {
public:
    std::string param;
    Node* body;
    LambdaNode(const std::string& param, Node* body) : param(param), body(body) {}
};

class ApplicationNode : public Node {
public:
    Node* left;
    Node* right;
    ApplicationNode(Node* left, Node* right) : left(left), right(right) {}
};

class Parser {
private:
    std::string input; // Input string to be parsed
    size_t pos = 0; // Current position in the input string

    // Get the current character from the input string
    char current_char() {
        return pos < input.size() ? input[pos] : '\0';
    }

    // Skip whitespace characters in the input
    void skip_whitespace() {
        while (pos < input.size() && std::isspace(input[pos])) {
            ++pos;
        }
    }

    // Parse a variable from the input
    std::string parse_variable() {
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



    // Parse an expression, which can be an atom or a series of applications
    Node* parse_expression() {
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

    // Parse an atom, which can be a variable, lambda or another expression
    Node* parse_atom() {
        skip_whitespace();

        // Lambda expression
        if (current_char() == '\\') {
            return parse_lambda();
        }
            // Bracket means nested expression so continue and parse the nested expression
        else if (current_char() == '(') {
            ++pos;
            Node* node = parse_expression();
            skip_whitespace();
            // End of nested expression so skip the bracket and return the node
            if (current_char() == ')') {
                ++pos;
                return node;
            }
                // Mismatched brackets
            else {
                throw std::runtime_error("Expected ')'");
            }
        }
            // Variable
        else {
            return new VariableNode{parse_variable()};
        }
    }

    // Parse a lambda expression from the input
    Node* parse_lambda() {
        ++pos; // Skip the '\' character
        std::string param = parse_variable(); // Parse the parameter name
        skip_whitespace();
        if (current_char() == '.') {
            ++pos; // Skip the '.' character
        }
        Node* body = parse_expression(); // Parse the body of the lambda
        return new LambdaNode{param, body};
    }

public:
    // Parse the input string into an AST
    Node* parse(const std::string& input_str) {
        input = input_str;
        pos = 0;
        return parse_expression();
    }
};


void print_tree(Node* node, int depth = 0) {
    if (!node) return;

    for(int i = 0; i < depth; ++i) std::cout << "  ";

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

int main() {
    std::string input;
    std::getline(std::cin, input);

    Parser parser;
    Node* ast = parser.parse(input);

    // Print the AST
    print_tree(ast);

    delete ast;
    return 0;
}

