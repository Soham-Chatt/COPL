#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <unordered_set>

class Interpreter {
public:
  Node *eval(Node *node, std::unordered_set<std::string> &bound_vars, int &iterations);

  Node *substitute(Node *node, const std::string &var, Node *value);

  std::string unique_var(const std::string &var, const std::unordered_set<std::string> &bound_vars);

  Node *beta_reduction(LambdaNode *lambda, Node *argument);

  void alpha_conversion(LambdaNode *lambda, std::unordered_set<std::string> &bound_vars);
};

#endif // INTERPRETER_H
