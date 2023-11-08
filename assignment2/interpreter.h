#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <unordered_set>

class Interpreter {
public:
  Node *eval(Node *node, int &iterations);

  Node *substitute(Node *node, const std::string &var, Node *value, std::unordered_set<std::string> &bound_vars);

  std::string unique_var(const std::string &var, const std::unordered_set<std::string> &bound_vars);

  Node *beta_reduction(LambdaNode *lambda, Node *argument, std::unordered_set<std::string> &bound_vars, std::vector<std::string> &free_vars);

  void find_bound_vars(Node *node, std::unordered_set<std::string> &bound_vars);

  void reset(std::unordered_set<std::string> &bound_vars);

  void find_free_vars(Node *node, std::vector<std::string> &free_vars);

  std::string is_conflict(std::unordered_set<std::string> bound_vars,
                   std::vector<std::string> free_vars);

  Node* alpha_conversion(Node *body, std::string &param,
                        std::unordered_set<std::string> &bound_vars);
};

#endif // INTERPRETER_H
