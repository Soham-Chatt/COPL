#include "interpreter.h"

const int MAX_ITERATIONS = 10001;

Node *Interpreter::beta_reduction(LambdaNode *lambda, Node *argument, std::unordered_set<std::string> &bound_vars) {
  std::cout << "Beta reduction: " << lambda->param << " -> " << argument->to_string() << std::endl;
  // Do alpha conversion if we are dealing with a bound variable
  if (bound_vars.find(lambda->param) != bound_vars.end()) {
    alpha_conversion(lambda, bound_vars);
  }

  Node *subst = substitute(lambda->body->copy(), lambda->param, argument, bound_vars);
  return subst;
}

void Interpreter::alpha_conversion(LambdaNode *lambda, std::unordered_set<std::string> &bound_vars) {
  std::cout << "Alpha conversion: " << lambda->param << std::endl;
  std::string new_var = unique_var(lambda->param, bound_vars);
  bound_vars.insert(new_var);
  std::cout << "New variable: " << new_var << std::endl;
  lambda->body = substitute(lambda->body, lambda->param, new VariableNode{new_var}, bound_vars);
  lambda->param = new_var;
}

Node *Interpreter::eval(Node *node, std::unordered_set<std::string> &bound_vars, int &iterations) {
  if (iterations >= MAX_ITERATIONS) {
    throw std::runtime_error("Maximum number of iterations reached");
  }

  iterations++;

  if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    Node *left = eval(a->left->copy(), bound_vars, iterations);
    Node *right = eval(a->right->copy(), bound_vars, iterations);
    std::cout << "Application: " << left->to_string() << " " << right->to_string() << std::endl;

    if (auto l = dynamic_cast<LambdaNode *>(left)) {
      Node *subst = beta_reduction(l, right, bound_vars);
      delete left;
      delete right;
      return eval(subst, bound_vars, iterations);
    }
    return new ApplicationNode{left, right};
  }

  return node->copy();
}


Node *Interpreter::substitute(Node *node, const std::string &var, Node *value, std::unordered_set<std::string> &bound_vars) {
  // Substitute all var with value
  if (auto v = dynamic_cast<VariableNode *>(node)) {
    if (v->name == var) {
      if (bound_vars.find(var) == bound_vars.end()) {
        std::cout << "Substitute: " << var << " -> " << value->to_string() << std::endl;
        return value->copy();
      }
    }
    return new VariableNode{v->name};
  } else if (auto l = dynamic_cast<LambdaNode *>(node)) {
    // If the variable is bound, no substitution
    if (l->param == var) {
      return new LambdaNode{l->param, l->body->copy()};
    }
    bound_vars.insert(l->param);
    auto new_body = substitute(l->body, var, value, bound_vars);
    bound_vars.erase(l->param);
    return new LambdaNode{l->param, new_body};
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    // Substitute in left and right nodes
    return new ApplicationNode{substitute(a->left, var, value, bound_vars), substitute(a->right, var, value, bound_vars)};
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

  // find all bound variables and enter them into the unordered set
  // to do this we traverse the tree and check which variables are in a lambda/application node
  // this is done recursively, the is a void function, so it does not return anything
// also avoids duplicates
void Interpreter::find_bound_vars(Node *node, std::unordered_set<std::string> &bound_vars) {
  if (auto v = dynamic_cast<VariableNode *>(node)) {
    // do nothing
  } else if (auto l = dynamic_cast<LambdaNode *>(node)) {
    bound_vars.insert(l->param);
    find_bound_vars(l->body, bound_vars);
  } else if (auto a = dynamic_cast<ApplicationNode *>(node)) {
    find_bound_vars(a->left, bound_vars);
    find_bound_vars(a->right, bound_vars);
  }
}
