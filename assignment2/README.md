## Student Information
- Student Numbers: 3692965 and 3638715

## Program Status
- Working Correctly: Mostly
- Known Defects: False positive for bracketed expression in combination
with a nested lambda expression. Infinite loop when parsing positive example
(\x x x)(\x x x)

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

### Utility Functions
- **beta_reduction**: Takes a lambda expression and an argument, performs beta-reduction, and returns the resulting node.
- **alpha_conversion**:
- **eval**:
- **substitute**:
- **unique_var**:

### Main Function
- Reads a file given by argument
- Creates a `Parser` instance and attempts to parse the input into an AST. Afterwards creates an `Interpreter` instance and attempts to evaluate the AST.
- Handles parsing/interpreting errors by catching exceptions and reporting error messages, cleaning up resources before exiting with status 1 or status 2 in case of max limit reached.
- On successful interpreting, prints the result of the evaluation. Exits with status 0.

### How to Run the Program
Simply run the program with the following command:
```make run```

Included are a positive.txt and negative.txt which can be automatically ran with the following commands:
```make pos``` or ```make neg```

```make clean``` will remove all object files and the executable.

