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

  // Read line by line
  while (std::getline(inFile, line)) {
    Node *root;
    Node *reduced = nullptr;
    // Parse the line
    try {
      root = parser.parse(line);
      std::cout << "Parsed successfully: " << root->to_string() << std::endl;
    } catch (std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

    int iterations = 0;
    // Evaluate the expression
    try {
      reduced = interpreter.eval(root, iterations);
      if (reduced) {
        std::cout << "Reduced expression: " << reduced->to_string() << std::endl;
      } else {
        delete reduced;
        std::cout << "Could not reduce the expression further." << std::endl;
      }
    } catch (std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

    delete root;
    delete reduced;
  }

  return 0;
}

