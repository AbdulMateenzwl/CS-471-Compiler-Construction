# Three Address Code (TAC) Generator in C++

## Introduction

This project is a **Three Address Code (TAC) Generator** implemented in C++. It demonstrates the process of compiling a simplified programming language by performing lexical analysis, parsing, semantic analysis, and generating intermediate code. The generated TAC serves as an intermediate representation, crucial for further optimization and translation into target machine code.

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
  - [Lexer](#lexer)
  - [Parser](#parser)
  - [Symbol Table](#symbol-table)
  - [Intermediate Code Generator](#intermediate-code-generator)
  - [Error Handling](#error-handling)
- [Supported Language Constructs](#supported-language-constructs)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Execution](#execution)
- [Usage](#usage)
- [Example](#example)
  - [Input Source Code](#input-source-code)
  - [Lexer Output](#lexer-output)
  - [Generated Three-Address Code](#generated-three-address-code)
- [License](#license)

## Features

- **Lexical Analysis**: Tokenizes the input source code into meaningful tokens.
- **Parsing**: Builds an abstract syntax tree (AST) from the token stream based on grammar rules.
- **Semantic Analysis**: Checks for semantic correctness, including type checking and scope management.
- **Intermediate Code Generation**: Produces three-address code for expressions and statements.
- **Symbol Table Management**: Maintains information about identifiers, their types, and scopes.
- **Control Flow Constructs**: Supports `if`, `else`, `while`, and `for` loops.
- **Function Handling**: Supports function declarations, definitions, and return statements.
- **Error Reporting**: Provides detailed syntax and semantic error messages.

## Architecture

### Lexer

The **Lexer** class is responsible for converting the raw source code into a sequence of tokens that represent the syntactic elements of the language.

**Key Responsibilities:**

- **Tokenization of Identifiers and Keywords**: Recognizes variable names, function names, and language keywords.
- **Literal Handling**: Processes numeric literals (integers and floats), string literals, and character literals.
- **Operator and Delimiter Recognition**: Identifies arithmetic operators (`+`, `-`, `*`, `/`), assignment operators, comparison operators, and delimiters like parentheses and semicolons.
- **Comment Skipping**: Ignores single-line (`//`) and multi-line (`/* ... */`) comments to prevent them from affecting the token stream.
- **Line Number Tracking**: Keeps track of line numbers to provide informative error messages.

**Methods:**

- `tokenize()`: Initiates the tokenization process and returns a vector of tokens.
- `consumeNumber()`: Reads numeric literals, handling integers and decimals.
- `consumeWord()`: Reads identifiers and checks if they are keywords.
- `printTokens(const vector<Token> &tokens)`: Outputs the list of tokens for debugging purposes.

### Parser

The **Parser** class takes the token stream from the lexer and constructs an abstract syntax tree (AST) while performing syntax and semantic analysis.

**Key Responsibilities:**

- **Statement Parsing**: Parses different types of statements including declarations, assignments, control structures, and function definitions.
- **Expression Parsing**: Handles arithmetic and logical expressions, respecting operator precedence and associativity.
- **Function Parsing**: Manages function declarations, parameter lists, and function bodies.
- **Control Structures**: Parses `if-else` statements, `while` loops, and `for` loops, generating appropriate control flow in the TAC.
- **Semantic Analysis**: Validates variable declarations, type compatibility, and scope rules.
- **Intermediate Code Generation**: Works in tandem with the intermediate code generator to emit TAC during parsing.

**Methods:**

- `parseProgram()`: Entry point for parsing the entire program.
- `parseStatement()`: Determines the type of statement and delegates to the appropriate parsing function.
- `parseDeclaration()`: Parses variable and constant declarations.
- `parseAssignment()`: Parses assignment statements.
- `parseIfStatement()`, `parseWhileStatement()`, `parseForStatement()`: Parse control flow statements.
- `parseFunctionDeclaration()`: Parses function declarations and definitions.
- `parseExpression()`, `parseTerm()`, `parseFactor()`: Parse components of expressions.

### Symbol Table

The **SymbolTable** class keeps track of identifiers, their types, scopes, and other attributes necessary for semantic analysis.

**Key Responsibilities:**

- **Declaration Management**: Records variable and function declarations to prevent redeclaration errors.
- **Type Information**: Stores type information for identifiers to facilitate type checking.
- **Constant Handling**: Identifies constants to prevent unintended assignments.
- **Function Signatures**: Maintains function parameter types and return types for correct function usage.

**Methods:**

- `declareVariable(const string &name, SymbolType type, int lineNumber, bool isConstant)`: Adds a new variable to the symbol table.
- `declareFunction(const string &name, SymbolType returnType, const vector<pair<SymbolType, string>> &params, int lineNumber)`: Records a new function.
- `getVariableType(const string &name)`: Retrieves the type of a declared variable.
- `isDeclared(const string &name)`: Checks if an identifier has been declared.
- `isConstant(const string &name)`: Determines if a variable is a constant.

### Intermediate Code Generator

The **IntermediateCodeGenerator** class produces three-address code during the parsing process.

**Key Responsibilities:**

- **Temporary Variable Management**: Generates temporary variables for intermediate results.
- **Label Management**: Generates unique labels for control flow constructs.
- **Instruction Emission**: Provides methods to add instructions to the TAC.
- **Code Output**: Offers functionality to output or retrieve the generated TAC.

**Methods:**

- `newTemp()`: Returns a new temporary variable name.
- `newLabel()`: Returns a new label for use in control flow.
- `addInstruction(const string &instr)`: Adds an instruction to the TAC list.
- `printInstructions()`: Outputs the TAC to the console.
- `getInstructions()`: Retrieves the list of TAC instructions.

**Example TAC Instructions:**

- Arithmetic operations: `t0 = x + y`
- Assignments: `x = 10`
- Conditional jumps: `if t1 goto L1`
- Labels: `L1:`

### Error Handling

The **ThrowError** class provides static methods for reporting syntax and semantic errors encountered during parsing.

**Key Responsibilities:**

- **Unexpected Tokens**: Reports when an unexpected token is encountered.
- **Expected Tokens**: Reports when an expected token is missing.
- **Semantic Errors**: Handles errors like variable redeclaration, undeclared variables, type mismatches, and invalid assignments to constants.
- **Error Messages**: Provides clear and informative error messages with line numbers and offending tokens.

**Methods:**

- `unExpectedTokenError(const string &val, int lineNumber)`: Reports unexpected tokens.
- `expectedTokenError(const string &val, int lineNumber)`: Reports missing expected tokens.
- `semanticErrorVariableAlreadyDeclared(const string &val, int lineNumber)`: Reports redeclaration of variables.
- `semanticErrorFunctionAlreadyDeclared(const string &val, int lineNumber)`: Reports redeclaration of functions.
- `semanticErrorVarableNotDeclared(const string &val, int lineNumber)`: Reports usage of undeclared variables.
- `semanticErrorConstantAssignment(const string &val, int lineNumber)`: Reports assignment to constants.

## Supported Language Constructs

- **Data Types**: `int`, `float`, `char`, `string`
- **Variables**: Declaration (`int x;`), initialization (`int x = 10;`), constants (`const int x = 5;`)
- **Operators**: Arithmetic (`+`, `-`, `*`, `/`), comparison (`>`, `<`)
- **Control Flow**: `if`, `else`, `while`, `for`
- **Functions**: Declaration, definition, and return statements
- **Expressions**: Arithmetic and logical expressions with proper operator precedence
- **Comments**: Single-line (`// comment`) and multi-line (`/* comment */`)

## Getting Started

### Prerequisites

- **Compiler**: A C++ compiler supporting C++11 or later (e.g., GCC, Clang, MSVC)
- **Development Environment**: Optional IDE or text editor for code editing

### Compilation

Compile the code using a C++ compiler. For example, using GCC:

```bash
g++ -std=c++11 ThreeAddressCoding.cpp -o tac_generator
```