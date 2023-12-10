#include "parser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>

int main(int argc, char *argv[]) {
  if (argc > 3 || argc < 2) {
    std::cerr << "Usage: " << argv[0] << " [file_name] <-d>" << std::endl;
    return 1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile) {
    std::cerr << "Cannot open input file: " << argv[1] << std::endl;
    return 1;
  }

  bool debugMode = (argc == 3 && std::string(argv[2]) == "-d");
  std::string line;
  Parser parser;

  // Read line by line
  while (std::getline(inFile, line)) {
    Node *root;
    // Parse the line
    try {
      root = parser.parse(line);
      std::cout << "Parsed successfully: " << root->to_string() << std::endl;
      if (debugMode) {
        std::cout << "Dot Tree: \n" << parser.generate_dot(root, -1) << std::endl;
      }
    } catch (std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

    delete root;
  }

  return 0;
}

