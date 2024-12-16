# Lexical Analyzer Documentation

## Overview
The lexical analyzer, also known as the scanner or lexer, is the first phase of a compiler. It is responsible for reading the source code and converting it into a sequence of tokens, which are meaningful sequences of characters. These tokens are then passed to the subsequent phases of the compiler for further processing.

## Objectives
- **Tokenize Source Code**: Identify and classify meaningful sequences in the input source code.
- **Error Detection**: Detect invalid tokens and report lexical errors.
- **Simplification**: Reduce the complexity of the subsequent phases by abstracting the raw input into structured tokens.

## Input and Output
- **Input**: The raw source code written in a programming language.
- **Output**: A sequence of tokens, where each token has the following attributes:
  - Token type (e.g., identifier, keyword, operator, literal)
  - Token value (the exact substring from the source code)
  - Line number (for error reporting and debugging)

## Components of the Lexical Analyzer

### 1. **Token Categories**
The lexical analyzer identifies the following categories of tokens:
- **Keywords**: Reserved words in the language (e.g., `if`, `else`, `while`).
- **Identifiers**: Names for variables, functions, or other user-defined entities.
- **Literals**: Numeric, string, or character constants.
- **Operators**: Arithmetic, logical, relational, and assignment operators.
- **Delimiters**: Symbols used for structuring the code (e.g., `;`, `,`, `{`, `}`).
- **Comments**: Single-line and multi-line comments, which are ignored by the compiler.

### 2. **Finite Automata**
The lexical analyzer uses finite automata to recognize tokens:
- **Deterministic Finite Automaton (DFA)**: Used for token recognition.
- **Transition Table**: Maps states to transitions based on input characters.

### 3. **Symbol Table**
A symbol table is used to store identifiers and their associated metadata. The lexical analyzer populates this table with entries for identifiers and literals.

### 4. **Error Handling**
The lexical analyzer detects lexical errors such as:
- Unrecognized characters
- Unterminated strings
- Invalid numeric literals

It reports errors with line numbers and a description of the issue.

## Workflow
1. **Input Buffering**: The source code is read into an input buffer to optimize character reading.
2. **Lexeme Recognition**: The lexer scans the input buffer to identify lexemes that match predefined patterns.
3. **Token Generation**: For each recognized lexeme, a corresponding token is generated.
4. **Error Reporting**: If an invalid lexeme is encountered, an error message is generated.
5. **Token Output**: The generated tokens are sent to the parser.

## Implementation Details
- **Regular Expressions**: Define patterns for tokens (e.g., `[a-zA-Z_][a-zA-Z0-9_]*` for identifiers).
- **Input Buffering**: Uses a two-buffer scheme to handle large inputs efficiently.
- **Lookahead**: Handles ambiguities in token recognition by peeking at upcoming characters.

## Example
### Input Source Code:
```c
int x = 10;
if (x > 5) {
    x = x + 1;
}
```

### Generated Tokens:
| Token Type   | Token Value | Line Number |
|--------------|-------------|-------------|
| Keyword      | `int`       | 1           |
| Identifier   | `x`         | 1           |
| Operator     | `=`         | 1           |
| Literal      | `10`        | 1           |
| Delimiter    | `;`         | 1           |
| Keyword      | `if`        | 2           |
| Delimiter    | `(`         | 2           |
| Identifier   | `x`         | 2           |
| Operator     | `>`         | 2           |
| Literal      | `5`         | 2           |
| Delimiter    | `)`         | 2           |
| Delimiter    | `{`         | 2           |
| Identifier   | `x`         | 3           |
| Operator     | `=`         | 3           |
| Identifier   | `x`         | 3           |
| Operator     | `+`         | 3           |
| Literal      | `1`         | 3           |
| Delimiter    | `;`         | 3           |
| Delimiter    | `}`         | 4           |

## Error Examples
### Input Source Code:
```c
int 123abc = 10;
```

### Error Report:
| Line Number | Error Description                  |
|-------------|------------------------------------|
| 1           | Invalid identifier: `123abc`      |

## Conclusion
The lexical analyzer is a crucial component of the compiler, ensuring that the raw source code is converted into structured tokens. This process facilitates the functioning of subsequent compiler phases, such as parsing and semantic analysis.

