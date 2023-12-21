## Student Information
- Students: Peter Branger (3692965) and Soham Chatterjee (3638715)
- Class: Concepts of Programming Languages (4032CNPREY)
- Group name: PeterBranger-SohamChatterjee

## Software 
- gcc version: 11.4.0
- OS: WSL/Ubuntu

## Program Status
- Working Correctly: Yes
- Known Defects: None. Our program goes into an infinite loop for an expression such as (\x y)((\x (x x))(\x (x x))). This is correct according
to our reduction strategy.

## Deviations from the Assignment
For some expressions, the output with parentheses is a bit off. However, this only has to do with the to_string methods.
The actual AST is generated correctly and can be verified using the -d flag.
We have adhered to the requirements of the assignment according to the **must** rules and have implemented the following additional features:

**should**
- report an error for syntax errors
- use the least amount of standard library code
- alpha-conversions only where beta-reduction would otherwise lead to variable capture
- be whitespace insensitive
- accept only one expression in the input file (we assume this is incorrectly stated, as all other assignments have accepted multiple expressions, which is also what we have done)

**may**
- include an explanation of how the program works in the README
- include positives and negatives files
- exit with status code 2 whenever a limit on the number of reductions is reached
- print to standard error on exit status code 1 or 2
- print understandable error messages

## Program Explanation
This is a C++ program that implements an interpreter for lambda calculus expressions. Below is a brief overview of the program structure:

### Classes and Methods

#### `Node` Class
As in assignment 1.

#### `Parser` Class
As in assignment 1.

#### `Interpreter` Class
- **Interpreter**: Responsible for traversing and evaluating the AST using leftmost-outermost reduction.

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

