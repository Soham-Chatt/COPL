## Student Information
- Student Numbers: 3692965 and 3638715

## Program Status
- Working Correctly: Yes
- Known Defects: None

## Deviations from the Assignment
None. We have adhered to the requirements of the assignment according to the **must** rules 
and have implemented the following additional features:

**should**
- report an error for syntax errors
- use the least amount of standard library code
- terminate within a short amount of time
- be whitespace insensitive
- produce the exact same result when parsing its own output
- output an unambiguous expression

**may**
- accept multiple expressions, one per line
- print an error to standard error
- print understandable error messages
- use the least amount of whitespace and parentheses in its output
- include an explanation of how the program works in the README
- include positives and negatives files

## Program Explanation
This is a C++ program that implements a parser for lambda calculus expressions. Below is a brief overview of the program structure:

### Classes and Methods

#### `Node` Class
- **Node**: An abstract class representing a node in the AST.
- **VariableNode**: A class to represent variable nodes, holding the variable name.
- **LambdaNode**: A class to represent lambda function nodes, storing the parameter name and a pointer to the body node.
- **ApplicationNode**: A class representing function application nodes, holding pointers to function and argument nodes.

#### `Parser` Class
- **Parser**: Implements the parser with methods to parse lambda calculus expressions and build the AST.
- **parse**: A public method initiating the parsing and returns the root of the AST.

### Important Functions
- **parse_variable**: Parses a variable from the input.
- **parse_expression**: Parses an expression or a series of applications.
- **parse_atom**: Parses an atom (variable, lambda, or bracket-enclosed expression).
- **parse_lambda**: Parses a lambda expression from the input.
A **generate_dot** function is also included and can be used added by the user in the main function
but isn't used in the program. This is because we would otherwise have to use arguments which was not allowed for this assignment yet.

### Main Function
- Reads an input from the user.
- Creates a `Parser` instance and attempts to parse the input into an AST.
- Handles parsing errors by catching exceptions and reporting error messages, cleaning up resources before exiting with status 1.
- On successful parsing, prints an unambiguous form of the parsed expression, exiting with status 0.

### How to Run the Program
Simply run the program with the following command:
```make run```

Included are a positives.txt and negatives.txt which can be automatically ran with the following commands:
```make run``` or ```make neg```

```make clean``` will remove all object files and the executable.


