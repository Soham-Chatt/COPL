## Student Information
- Student Numbers: 3692965 and 3638715

## Program Status
- Working Correctly: Yes
- Known Defects: None

## Deviations from the Assignment
None. We have adhered to the requirements of the assignment according to the **must** rules
and have implemented **should** and **may** rules where possible.

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

