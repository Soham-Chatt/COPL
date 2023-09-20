// main.cc
#include "parser.h"
#include <iostream> // std::cout, std::cin, std::cerr
#include <string> // std::string
#include <vector> // For the (multi-line) expressions

// Short introduction to the program
void intro() {
    std::cout << "This program is meant for parsing lambda calculus expressions.\n"
                 "The program will parse the expression and print the abstract syntax tree and the result.\n"
                 "The program will also print the expression number before printing the tree and the result.\n\n"
                 "Enter lambda expressions to parse, one per line. Press enter on an empty line to finish.\n" <<
                 std::endl;
}

void getInputs(std::vector<std::string>& inputs) {
    std::string input;

    // Get the expressions from the user
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            break;
        }
        inputs.push_back(input);
    }
}

void parseAndEvaluateExpressions(const std::vector<std::string>& inputs, Parser& parser) {
    int exprNr = 1;
    // Iterating through each expression of input and keeping track of the expression number
    for (const auto& inp : inputs) {
        Node* ast = nullptr;
        try {
            ast = parser.parse(inp);
        } catch (const std::runtime_error& e) {
            std::cerr << "Expression " << exprNr << ": " << e.what() << '\n';
            delete ast;
            exprNr++;
            continue;
        }

        // Displaying the expression number before printing the tree and the result
        std::cout << "Expression " << exprNr << ":\n";
        print_tree(ast);
        std::cout << "Result: " << ast->to_string() << std::endl << std::endl;

        delete ast;
        exprNr++; // Increment expression number
    }
}

int main() {
    intro();

    std::vector<std::string> inputs;
    getInputs(inputs);

    Parser parser;
    parseAndEvaluateExpressions(inputs, parser);

    return 0;
}




