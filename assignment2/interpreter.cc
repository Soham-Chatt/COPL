#include "interpreter.h"
const int MAX_ITERATIONS = 100;

Node* Interpreter::eval(Node* node, std::unordered_set<std::string>& bound_vars, int& iterations) {
  if (iterations >= MAX_ITERATIONS) {
    return node; // Reached max iterations
  }

  iterations++;

  if (auto app = dynamic_cast<ApplicationNode*>(node)) {
    Node* left = eval(app->left, bound_vars, iterations); // Evaluate left child
    Node* right = eval(app->right, bound_vars, iterations); // Evaluate right child

    if (auto lambda = dynamic_cast<LambdaNode*>(left)) {
      bound_vars.insert(lambda->param);
      Node* subst = substitute(lambda->body, lambda->param, right);
      return eval(subst, bound_vars, iterations);
    }
    return new ApplicationNode{left, right}; // Return if no reduction was done
  }
  else if (auto lambda = dynamic_cast<LambdaNode*>(node)) {
    if (bound_vars.find(lambda->param) != bound_vars.end()) {
      // Alpha-conversion
      std::string new_var = unique_var(lambda->param, bound_vars);
      lambda->body = substitute(lambda->body, lambda->param, new VariableNode{new_var});
      lambda->param = new_var;
    }
    lambda->body = eval(lambda->body, bound_vars, iterations);
    return lambda;
  }
  return node;
}


Node* Interpreter::substitute(Node* node, const std::string& var, Node* value) {
  // If the node is a variable, and its name matches the variable to be substituted, return the substitution value
  if (auto v = dynamic_cast<VariableNode*>(node)) {
    if (v->name == var) {
      return value; // Return the substitution value
    }
  }
  // If the node is a lambda, and its parameter is not the variable to be substituted, recursively substitute in the body
  else if (auto l = dynamic_cast<LambdaNode*>(node)) {
    if (l->param != var) {
      l->body = substitute(l->body, var, value);
    }
  }
  // If the node is an application, recursively substitute in both the left and right child nodes
  else if (auto a = dynamic_cast<ApplicationNode*>(node)) {
    a->left = substitute(a->left, var, value);
    a->right = substitute(a->right, var, value);
  }
  return node; // If no substitution was done, return the original node
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
