#include "parser.h"
#include <string>
#include <iostream>
#include <vector>
#include <cctype>


int main() {
    std::cout << "Enter lambda expressions to parse, one per line. Press enter on an empty line to finish.\n";
    std::vector<std::string> inputs;
    std::string input;
    int exprNr = 1;

    // Get the expressions from the user
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            break;
        }
        inputs.push_back(input);
    }

    Parser parser;

    // Iterating through each expression of input and keeping track of the expression number
    for (const auto& inp : inputs) {
        Node* ast = nullptr;
        try {
            ast = parser.parse(inp);
        } catch (const std::runtime_error& e) {
            std::cerr << "Expression " << exprNr << ": " << e.what() << '\n';
            delete ast;
            exprNr++; // Increment expression number
            continue;
        }

        // Displaying the expression number before printing the tree and the result
        std::cout << "Expression " << exprNr << ":\n";
        print_tree(ast);
        std::cout << "Result: " << ast->to_string() << std::endl << std::endl;

        delete ast;
        exprNr++; // Increment expression number
    }
    return 0;
}



