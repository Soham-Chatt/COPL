#include "parser.h"
#include <iostream>
#include <string>

int main() {
  Parser parser;
  std::string expression;

  while (std::getline(std::cin, expression)) {
    try {
      auto parsedExpression = parser.parse(expression);
      std::cout << "Parsed successfully: " << parsedExpression->to_string() << std::endl;
    } catch (const std::runtime_error &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }

  return 0;
}
