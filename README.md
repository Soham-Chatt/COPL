## Student Information
- Student Numbers: 3692965 and 3638715

## Program Status
- Working Correctly: Yes
- Known Defects: None

## Deviations from the Assignment
None. We have adhered to the requirements of the assignment according to the **must** rules 
and have implemented **should** and **may** rules where possible.

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

### Utility Functions
- **current_char**: Gets the current character being parsed.
- **skip_whitespace**: Skips whitespace in the input string.
- **parse_variable**: Parses a variable from the input.
- **parse_expression**: Parses an expression or a series of applications.
- **parse_atom**: Parses an atom (variable, lambda, or bracket-enclosed expression).
- **parse_lambda**: Parses a lambda expression from the input.

### Main Function
- Reads an input from the user.
- Creates a `Parser` instance and attempts to parse the input into an AST.
- Handles parsing errors by catching exceptions and reporting error messages, cleaning up resources before exiting with status 1.
- On successful parsing, prints the AST and a string representation of the parsed expression, exiting with status 0.

### How to Run the Program
Simply run the program with the following command:
```make run``` 


```make clean``` will remove all object files and the executable.

