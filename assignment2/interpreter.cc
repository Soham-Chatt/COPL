#include "interpreter.h"
const int MAX_ITERATIONS = 10000;

Node* Interpreter::eval(Node* node, std::unordered_set<std::string>& bound_vars, int& iterations) {
  if (iterations >= MAX_ITERATIONS) {
    throw std::runtime_error("Maximum number of iterations reached");
  }

  iterations++;

  if (auto app = dynamic_cast<ApplicationNode*>(node)) {
    // Evaluate the left and right nodes
    Node* left = eval(app->left->copy(), bound_vars, iterations);
    Node* right = eval(app->right->copy(), bound_vars, iterations);

    // If left node is lambda, do substitution
    if (auto lambda = dynamic_cast<LambdaNode*>(left)) {
      bound_vars.insert(lambda->param); // Parameter is a bound variable
      Node* subst = substitute(lambda->body->copy(), lambda->param, right);
      delete left;
      delete right;
      return eval(subst, bound_vars, iterations);
    }
    return new ApplicationNode{left, right};
  }
  else if (auto lambda = dynamic_cast<LambdaNode*>(node)) {
    // Copy lambda and rename the bound variable if needed
    LambdaNode* new_lambda = new LambdaNode{lambda->param, lambda->body->copy()};
    if (bound_vars.find(new_lambda->param) != bound_vars.end()) {
      // Alpha-conversion
      std::string new_var = unique_var(new_lambda->param, bound_vars); // Name of the new variable
      new_lambda->body = substitute(new_lambda->body, new_lambda->param, new VariableNode{new_var}); // Substitute old variable with new variable
      new_lambda->param = new_var;
    }
    new_lambda->body = eval(new_lambda->body, bound_vars, iterations);
    return new_lambda;
  }
  return node->copy();
}

Node* Interpreter::substitute(Node* node, const std::string& var, Node* value) {
  // Substitute all var with value
  if (auto v = dynamic_cast<VariableNode*>(node)) {
    if (v->name == var) {
      return value->copy();
    }
    return new VariableNode{v->name};
  }
  else if (auto l = dynamic_cast<LambdaNode*>(node)) {
    // If the variable is bound, no substitution
    if (l->param == var) {
      return new LambdaNode{l->param, l->body->copy()};
    }
    return new LambdaNode{l->param, substitute(l->body, var, value)}; // Substitute in the body
  }
  else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
    // Substitute in left and right nodes
    return new ApplicationNode{substitute(a->left, var, value), substitute(a->right, var, value)};
  }
  return node->copy();
}

std::string Interpreter::unique_var(const std::string& var, const std::unordered_set<std::string>& bound_vars) {
  // Generate a new unique variable name by appending a number to the original variable name
  std::string new_var = var;
  int counter = 1;
  while (bound_vars.find(new_var) != bound_vars.end()) {
    new_var = var + std::to_string(counter++);
  }
  return new_var;
}
