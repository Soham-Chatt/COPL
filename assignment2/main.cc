#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
    return 1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile) {
    std::cerr << "Cannot open input file: " << argv[1] << std::endl;
    return 1;
  }

  std::string line;
  Parser parser;
  Interpreter interpreter;

  while (std::getline(inFile, line)) {
    Node* root = parser.parse(line); // Parse each line and create AST

    std::unordered_set<std::string> bound_vars;
    int iterations = 0;

    Node* reduced = interpreter.eval(root, bound_vars, iterations);

    if (reduced) {
      std::cout << "Reduced expression: " << reduced->to_string() << std::endl;
    } else {
      std::cout << "Could not reduce the expression further." << std::endl;
    }
  }

  return 0;
}

