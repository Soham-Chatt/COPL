#include "parser.h"
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

  // Read line by line
  while (std::getline(inFile, line)) {
    Node *root;
    // Parse the line
    try {
      root = parser.parse(line);
//      std::cout << parser.generate_dot(root, -1) << std::endl;
    } catch (std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      delete root;
      return 1;
    }

    delete root;
  }

  return 0;
}

