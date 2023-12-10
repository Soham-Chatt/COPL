#include "interpreter.h"

const int MAX_ITERATIONS = 10000;

std::string
Interpreter::is_conflict(std::unordered_set<std::string> bound_vars, const std::unordered_set<std::string> &free_vars) {
  // Check if a free var is found in bound var
  for (auto &var: free_vars) {
    if (bound_vars.find(var) != bound_vars.end()) {
      return var;
    }
  }
  return "";
}

Node *Interpreter::beta_reduction(LambdaNode *lambda, Node *argument, std::unordered_set<std::string> &bound_vars,
                                  std::unordered_set<std::string> &free_vars) {
  find_bound_vars(lambda->body, bound_vars);
  find_free_vars(argument, free_vars);

  // Perform alpha conversion if necessary on the body of the lambda
  std::string conflict = is_conflict(bound_vars, free_vars);
  if (!conflict.empty()) {
    lambda->body = alpha_conversion(lambda->body, conflict, bound_vars);
  }

  Node *subst = substitute(lambda->body->copy(), lambda->param, argument, bound_vars);

  return subst;
}

Node *Interpreter::alpha_conversion(Node *body, std::string &param, std::unordered_set<std::string> &bound_vars) {
  std::string new_var = unique_var(param, bound_vars);
  // Substitute all occurrences of param with new_var
  body = substitute(body, param, new VariableNode{new_var}, bound_vars);
  if (auto l = dynamic_cast<LambdaNode *>(body)) {
    l->param = new_var;
  }
  return body;
}


Node *Interpreter::eval(Node *node, int &iterations) {
  if (iterations >= MAX_ITERATIONS) {
    throw std::runtime_error("Maximum number of iterations reached");
  }

  iterations++;

  std::unordered_set<std::string> bound_vars = {};
  std::unordered_set<std::string> free_vars = {};
  // Evaluate the left and right nodes
  if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    Node *left = eval(a->left->copy(), iterations);
    Node *right = eval(a->right->copy(), iterations);
    // If the left node is a lambda, perform beta reduction
    if (auto l = dynamic_cast<LambdaNode *>(left)) {
      Node *subst = beta_reduction(l, right, bound_vars, free_vars);
      delete left;
      delete right;
      return eval(subst, iterations);
    }
    return new ApplicationNode{left, right};
  }

  return node->copy();
}


Node *
Interpreter::substitute(Node *node, const std::string &var, Node *value, std::unordered_set<std::string> &bound_vars) {
  // Substitute all var with value
  if (auto v = dynamic_cast<VariableNode *>(node)) {
    if (v->name == var) {
      return value->copy();
    }
    return new VariableNode{v->name};
  } else if (auto l = dynamic_cast<LambdaNode *>(node)) {
    // If the variable is bound, no substitution
    if (l->param == var) {
      return new LambdaNode{l->param, l->body->copy()};
    }
    auto new_body = substitute(l->body, var, value, bound_vars);
    return new LambdaNode{l->param, new_body};
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    // Substitute in left and right nodes
    return new ApplicationNode{substitute(a->left, var, value, bound_vars),substitute(a->right, var, value, bound_vars)};
  }
  return node->copy();
}

std::string Interpreter::unique_var(const std::string &var, const std::unordered_set<std::string> &bound_vars) {
  // Generate a new unique variable name by appending a number to the original variable name
  std::string new_var = var;
  int counter = 1;
  while (bound_vars.find(new_var) != bound_vars.end()) {
    new_var = var + std::to_string(counter++);
  }
  return new_var;
}

void Interpreter::find_bound_vars(Node *node, std::unordered_set<std::string> &bound_vars) {
  if (auto l = dynamic_cast<LambdaNode *>(node)) {
    bound_vars.insert(l->param);
    find_bound_vars(l->body, bound_vars);
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    find_bound_vars(a->left, bound_vars);
    find_bound_vars(a->right, bound_vars);
  }
}

void Interpreter::find_free_vars(Node *node, std::unordered_set<std::string> &free_vars) {
  if (auto v = dynamic_cast<VariableNode *>(node)) {
    free_vars.insert(v->name);
  } else if (auto l = dynamic_cast<LambdaNode *>(node)) {
    find_free_vars(l->body, free_vars);
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    find_free_vars(a->left, free_vars);
    find_free_vars(a->right, free_vars);
  }
}