## Student Information
- Students: Peter Branger (3692965) and Soham Chatterjee (3638715)
- Class: Concepts of Programming Languages (4032CNPREY)
- Group name: PeterBranger-SohamChatterjee

## Software
- gcc version: 11.4.0
- OS: WSL/Ubuntu

## Deviations from the Assignment and Defects
For some expressions, the output with parentheses is a bit off. However, this only has to do with the to_string methods.
The actual AST is generated correctly and can be verified using the -d flag.
We have adhered to the requirements of the assignment according to the **must** rules
and have implemented the following additional features:

**should**
- report an error for syntax errors
- use the least amount of standard library code
- output the judgment in an unambiguous and standardizes output format
- be whitespace insensitive

**may**
- include an explanation of how the program works in the README
- include positives and negatives files
- process multiple judgements, one per line
- print to standard error on exit status code 1
- print understandable error messages

## Program Explanation
This is a C++ program that implements a type-checker for lambda calculus expressions. Below is a brief overview of the program structure:

### Classes and Methods

#### `Node` Class
Mostly such as in assignment 1, with the addition of inherited node classes for type and judgement nodes.

#### `Parser` Class
Parses the input into an AST for a simply-typed lambda calculus expression.

### Important Functions
- tokenize: Breaks down the input string into tokens for parsing.
- parse_judgement: Parses a judgement from the tokenized input, consisting of an expression and a type.
- parse_expression: Parses an expression from the tokenized input.
- parse_atom: Parses an atomic expression from the tokenized input.
- parse_lambda: Parses a lambda expression from the tokenized input.
- parse_single_type: Parses a single type from the tokenized input.
- parse_type: Parses a type, handling function types with '->', from the tokenized input.
- parse: The main entry point for parsing an input string into a judgement node.
- getDerivation: Checks if the derivation of a judgement node is correct.
- extractTypes: Extracts types from a string, useful in type checking.
- getType: Determines the type of given node.

A **generate_dot** function is also included and can be used added by the user in the main function by using the argument -d.

### Main Function
- Reads a file given by argument
- Creates a `Parser` instance and attempts to parse the input into an AST and checks if the types are valid.
- Handles parsing/type-checking errors by catching exceptions and reporting error messages, cleaning up resources before exiting with status 1.
- On successful interpreting and derivation, prints the parsed expression. Exits with status 0.

### How to Run the Program
Simply run the program with the following command:
```make run```

Included are a positives.txt and negatives.txt which can be automatically ran with the following commands:
```make run``` or ```make neg```

```make clean``` will remove all object files and the executable.

