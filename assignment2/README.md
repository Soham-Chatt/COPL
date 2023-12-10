## Student Information
- Student Numbers: 3692965 and 3638715

## Program Status
- Working Correctly: Yes
- Known Defects: None. Our program goes into an infinite loop for an expression such as (\x y)((\x (x x))(\x (x x))). This is correct according
to our reduction strategy.

## Deviations from the Assignment
None. We have adhered to the requirements of the assignment according to the **must** rules
and have implemented **should** and **may** rules where possible.

## Program Explanation
This is a C++ program that implements an interpreter for lambda calculus expressions. Below is a brief overview of the program structure:

### Classes and Methods

#### `Node` Class
As in assignment 1.

#### `Parser` Class
As in assignment 1.

#### `Interpreter` Class
- **Interpreter**: Responsible for traversing and evaluating the AST.

### Important Functions
- **beta_reduction**: Takes a lambda expression and an argument, performs beta-reduction, and returns the resulting node.
- **alpha_conversion**: Takes a lambda expression and a variable name, performs alpha-conversion, and returns the resulting node.
- **eval**: Takes a node and evaluates it, returning the resulting node.
- **substitute**: Takes a node and a variable name and substitutes all instances of the variable with the node, returning the resulting node.
- **unique_var**: Takes a node and a variable name and returns a unique variable name based on the given variable name.
A **generate_dot** function is also included and can be used added by the user in the main function by using the argument -d.

### Main Function
- Reads a file given by argument
- Creates a `Parser` instance and attempts to parse the input into an AST. Afterward creates an `Interpreter` instance and attempts to evaluate the AST.
- Handles parsing/interpreting errors by catching exceptions and reporting error messages, cleaning up resources before exiting with status 1 or status 2 in case of max limit reached.
- On successful interpreting, prints the result of the evaluation. Exits with status 0.

### How to Run the Program
Simply run the program with the following command:
```make run```

Included are a positives.txt and negatives.txt which can be automatically ran with the following commands:
```make run``` or ```make neg```

```make clean``` will remove all object files and the executable.

