#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum TokenType {
    T_INT,
    T_STRING,
    T_FLOAT,
    T_CHAR,
    T_BOOL,
    T_CONST,
    T_ID,
    T_NUM,
    T_WORD,
    T_CHARACTER,
    T_DECIMAL,
    T_BOOLEAN,
    T_AND,
    T_OR,
    T_NOT,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_COMMA,
    T_GT,
    T_LT,
    T_EGT,
    T_ELT,
    T_CMP,
    T_WHILE,
    T_FOR,
    T_EOF
};

struct Token {
    TokenType type;
    string value;
    int lineNumber;
};

class ThrowError {
   public:
    static void unExpectedTokenError(string val, int lineNumber) {
        cout << "Syntax error: unexpected token '" << val << "' at line " << lineNumber << endl;
        exit(1);
    }
    static void expectedTokenError(string val, int lineNumber) {
        cout << "Syntax error: expected assignment operator " << val << " at line " << lineNumber << endl;
        exit(1);
    }
    static void sementicErrorVariableAlreadyDeclared(string val, int lineNumber) {
        cout << "Semantic error: Variable '" << val << "' is already declared. Error on line " << lineNumber << endl;
        exit(1);
    }
    static void sementicErrorFunctionAlreadyDeclared(string val, int lineNumber) {
        cout << "Semantic error: Function '" << val << "' is already declared. Error on line " << lineNumber << endl;
        exit(1);
    }
    static void sementicErrorVarableNotDeclared(string val, int lineNumber) {
        cout << "Semantic error: Variable '" << val << "' is not declared at line " << lineNumber << endl;
        exit(1);
    }
    static void sementicErrorConstantAssignment(string val, int lineNumber) {
        cout << "Semantic error: Cannot assign value to constant variable '" << val << "' at line " << lineNumber << endl;
        exit(1);
    }
    static void sementicErrorInvalidType(string val, int lineNumber, string expected, string actual) {
        cout << "Semantic error: Invalid type '" << val << "' at line " << lineNumber << endl;
        cout << "Can not append a " << actual << " in a " << expected;
        exit(1);
    }
};

class Lexer {
   private:
    string src;
    size_t pos;
    int lineNumber;

   public:
    Lexer(const string &src) : src(src), pos(0), lineNumber(1) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];

            if (current == '\n') {
                lineNumber++;
                pos++;
                continue;
            }
            if (isspace(current)) {
                pos++;
                continue;
            }
            if (isdigit(current)) {
                string num = consumeNumber();
                if (num.find('.') != string::npos) {
                    tokens.push_back(Token{T_DECIMAL, num, lineNumber});
                } else
                    tokens.push_back(Token{T_NUM, num, lineNumber});
                continue;
            }
            if (isalpha(current)) {
                string word = consumeWord();
                TokenType type = T_ID;

                if (word == "int")
                    type = T_INT;
                if (word == "string")
                    type = T_STRING;
                else if (word == "char")
                    type = T_CHAR;
                else if (word == "float")
                    type = T_FLOAT;
                else if (word == "bool")
                    type = T_BOOL;
                else if (word == "const")
                    type = T_CONST;
                else if (word == "true" || word == "false")
                    type = T_BOOLEAN;
                else if (word == "if")
                    type = T_IF;
                else if (word == "else")
                    type = T_ELSE;
                else if (word == "return")
                    type = T_RETURN;
                else if (word == "while")
                    type = T_WHILE;
                else if (word == "for")
                    type = T_FOR;

                tokens.push_back(Token{type, word, lineNumber});
                continue;
            }
            if (current == '"') {
                pos++;
                string word = "";
                while (pos < src.size() && src[pos] != '"') {
                    word += src[pos];
                    pos++;
                }
                pos++;
                tokens.push_back(Token{T_WORD, word, lineNumber});
                continue;
            }
            if (current == '\'') {
                pos++;
                string word = "";
                while (pos < src.size() && src[pos] != '\'') {
                    word += src[pos];
                    pos++;
                }
                pos++;
                tokens.push_back(Token{T_CHARACTER, word, lineNumber});
                continue;
            }
            if (current == '/' && pos + 1 < src.size() && src[pos + 1] == '*') {
                pos += 2;                       // Move past '/*'
                while (pos + 1 < src.size()) {  // Ensure we are not accessing out of bounds
                    if (src[pos] == '*' && src[pos + 1] == '/') {
                        pos += 2;  // Move past '*/'
                        break;
                    }
                    if (src[pos] == '\n') {
                        lineNumber++;  // Increment line number on newline
                    }
                    pos++;
                }

                continue;  // Skip to the next token
            }
            if (current == '/') {
                pos++;
                if (src[pos] == '/') {
                    while (pos < src.size() && src[pos] != '\n')
                        pos++;
                    continue;
                }
            }

            char next = pos + 1 < src.size() ? src[pos + 1] : '\0';
            if (current == '=') {
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
            } else if (current == '+') {
                tokens.push_back(Token{T_PLUS, "+", lineNumber});
            } else if (current == '-') {
                tokens.push_back(Token{T_MINUS, "-", lineNumber});
            } else if (current == '*') {
                tokens.push_back(Token{T_MUL, "*", lineNumber});
            } else if (current == '/') {
                tokens.push_back(Token{T_DIV, "/", lineNumber});
            } else if (current == '(') {
                tokens.push_back(Token{T_LPAREN, "(", lineNumber});
            } else if (current == ')') {
                tokens.push_back(Token{T_RPAREN, ")", lineNumber});
            } else if (current == '{') {
                tokens.push_back(Token{T_LBRACE, "{", lineNumber});
            } else if (current == '}') {
                tokens.push_back(Token{T_RBRACE, "}", lineNumber});
            } else if (current == ';') {
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
            } else if (current == ',') {
                tokens.push_back(Token{T_COMMA, ";", lineNumber});
            } else if (current == '=' && next == '=') {
                tokens.push_back(Token{T_CMP, "=", lineNumber});
                pos++;
            } else if (current == '>' && next == '=') {
                tokens.push_back(Token{T_EGT, ">=", lineNumber});
                pos++;
            } else if (current == '<' && next == '=') {
                tokens.push_back(Token{T_ELT, "<=", lineNumber});
                pos++;
            } else if (current == '>') {
                tokens.push_back(Token{T_GT, ">", lineNumber});
            } else if (current == '<') {
                tokens.push_back(Token{T_LT, "<", lineNumber});
            } else if (current == '&' && next == '&') {
                tokens.push_back(Token{T_AND, "&&", lineNumber});
                pos++;
            } else if (current == '|' && next == '|') {
                tokens.push_back(Token{T_OR, "||", lineNumber});
                pos++;
                // } else if ( current == '!' && next == '=') {
                //     tokens.push_back(Token{T_CMP, "!=", lineNumber});
            } else if (current == '!') {
                tokens.push_back(Token{T_NOT, "!", lineNumber});
            } else if (current == '\0') {
                break;
            } else {
                cout << "Unexpected character: " << current << " at line " << lineNumber << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber});
        return tokens;
    }

    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.')) pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    void printTokens(const vector<Token> &tokens) {
        for (const auto &token : tokens) {
            cout << "Token: " << token.value << "  Type: " << token.type << "  LineNumber: " << token.lineNumber << endl;
        }
    }
};

enum SymbolType {
    INT = TokenType::T_INT,
    FLOAT = TokenType::T_FLOAT,
    CHAR = TokenType::T_CHAR,
    STRING = TokenType::T_STRING,
    BOOL = TokenType::T_BOOL
};

class Utils {
   public:
    static SymbolType getTokenType(TokenType type) {
        if (type == T_INT) {
            return SymbolType::INT;
        } else if (type == T_FLOAT) {
            return SymbolType::FLOAT;
        } else if (type == T_CHAR) {
            return SymbolType::CHAR;
        } else if (type == T_STRING) {
            return SymbolType::STRING;
        } else if (type == T_BOOL) {
            return SymbolType::BOOL;
        }
        return SymbolType::INT;
    }

    static TokenType getSymbolType(SymbolType type) {
        if (type == SymbolType::INT) {
            return T_INT;
        } else if (type == SymbolType::FLOAT) {
            return T_FLOAT;
        } else if (type == SymbolType::CHAR) {
            return T_CHAR;
        } else if (type == SymbolType::STRING) {
            return T_STRING;
        } else if (type == SymbolType::BOOL) {
            return T_BOOL;
        }
        return T_INT;
    }
    static string getSymbolTypeString(SymbolType type) {
        if (type == SymbolType::INT) {
            return "int";
        } else if (type == SymbolType::FLOAT) {
            return "float";
        } else if (type == SymbolType::CHAR) {
            return "char";
        } else if (type == SymbolType::STRING) {
            return "string";
        }
        return "int";
    }
};

struct Symbol {
    std::string name;
    SymbolType type;  // int, float, etc.
    bool isConstant;
    vector<pair<SymbolType, string>> params;
};

class SymbolTable {
   public:
    void declareVariable(const string &name, const SymbolType type, int lineNumber, bool isConstant = false) {
        if (symbolTable.find(name) != symbolTable.end()) {
            ThrowError::sementicErrorVariableAlreadyDeclared(name, lineNumber);
        }
        symbolTable[name] = Symbol{name, type, isConstant};
    }

    SymbolType getVariableType(const string &name) {
        if (symbolTable.find(name) == symbolTable.end()) {
            ThrowError::sementicErrorVarableNotDeclared(name, 0);
        }
        return symbolTable[name].type;
    }

    bool isConstant(const string &name) {
        if (symbolTable.find(name) == symbolTable.end()) {
            ThrowError::sementicErrorVarableNotDeclared(name, 0);
        }
        return symbolTable[name].isConstant;
    }

    bool isDeclared(const string &name) const {
        return symbolTable.find(name) != symbolTable.end();
    }

    void declareFunction(const string &name, const SymbolType returnType, const vector<pair<SymbolType, string>> &params, int lineNumber) {
        if (symbolTable.find(name) != symbolTable.end()) {
            ThrowError::sementicErrorFunctionAlreadyDeclared(name, lineNumber);
        }
        symbolTable[name] = Symbol{name, returnType, false, params};
    }

    bool isFunctionDeclared(const string &name, const SymbolType returnType, const vector<pair<SymbolType, string>> &params) {
        if (symbolTable.find(name) != symbolTable.end() && symbolTable[name].params.size() != params.size() && symbolTable[name].type != returnType) {
            int count = 0;
            for (int i = 0; i < params.size(); i++) {
                if (symbolTable[name].params[i].first == params[i].first) {
                    count++;
                }
            }
            if (count == params.size()) {
                return true;
            }
        }
        return false;
    }

   private:
    map<string, Symbol> symbolTable;
};

class IntermediateCodeGnerator {
   private:
    int labelCounter;

   public:
    vector<string> instructions;
    int tempCount = 0;
    IntermediateCodeGnerator() : labelCounter(0) {}

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    string newLabel() {
        return "L" + to_string(labelCounter++);  // Generate a label like L0, L1, L2, ...
    }

    void addInstruction(const string &instr) {
        instructions.push_back(instr);
    }

    void printInstructions() {
        for (const auto &instr : instructions) {
            cout << instr << endl;
        }
    }

    vector<string> getInstructions() {
        return instructions;
    }
};

class Parser {
   public:
    // Constructor
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGnerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}
    // here the private member of this class are being initalized with the arguments passed to this constructor

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
    }

   private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGnerator &icg;

    void parseStatement() {
        TokenType type = tokens[pos].type;
        if (type == T_INT || type == T_STRING || type == T_CHAR || type == T_FLOAT || type == T_BOOL) {
            parseDeclaration();
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_IF) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else if (tokens[pos].type == T_LBRACE) {
            parseBlock();
        } else if (tokens[pos].type == T_WHILE) {
            parseWhileStatement();
        } else if (tokens[pos].type == T_FOR) {
            parseForStatement();
        } else if (tokens[pos].type == T_CONST) {
            parseDeclaration(false, true);
        } else {
            ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);
        }
    }

    /*
     parseDeclaration handles the parsing of variable declarations.
     It expects the token type to be `T_INT` (for declaring an integer type variable),
     followed by an identifier (variable name), and a semicolon to terminate the statement.
     It also registers the declared variable in the symbol table with type "int".
     Example:
     int x;   // This will be parsed and the symbol table will store x with type "int".
    */
    void parseDeclaration(bool intialization = false, bool isConst = false) {
        if (isConst) {
            expect(T_CONST);
        }
        TokenType type = tokens[pos].type;
        expect(type);  // Expect and consume the type keyword.
        string varName = expectAndReturnValue(T_ID);
        if (tokens[pos].type == T_LPAREN) {  // Expect and return the variable name (identifier).
            parseFunctionDeclaration(Utils::getTokenType(type), varName);
            return;
        }
        symTable.declareVariable(varName, Utils::getTokenType(type), tokens[pos].lineNumber, isConst);  // Register the variable in the symbol table with type.
        if (tokens[pos].type == T_ASSIGN) {
            expect(T_ASSIGN);
            string expr = "";
            if (type == T_INT) {
                expr = parseExpression();
            } else if (type == T_STRING) {
                expr = parseString();
            } else if (type == T_CHAR) {
                expr = '"' + parseCharacter() + '"';
            } else if (type == T_FLOAT) {
                expr = parseExpression(TokenType::T_FLOAT);
            } else if (type == T_BOOL || tokens[pos].type == T_NOT) {
                expr = parseBooleanExpression();
            }

            icg.addInstruction(varName + " = " + expr);  // Generate intermediate code for the assignment.
        } else if (tokens[pos].type != T_ASSIGN && intialization) {
            ThrowError::expectedTokenError("=", tokens[pos].lineNumber);
        }
        expect(T_SEMICOLON);  // Expect the semicolon to end the statement.
    }

    string parseCharacter() {
        string result;

        // Ensure the current token is a character literal
        if (tokens[pos].type != T_CHARACTER) {
            ThrowError::expectedTokenError("character literal", tokens[pos].lineNumber);
        }

        // Extract the character literal value
        result = tokens[pos].value;

        // Validate character literal (e.g., 'a', '\n')
        if (result.size() != 1) {
            ThrowError::unExpectedTokenError("Invalid character literal", tokens[pos].lineNumber);
        }

        pos++;  // Move to the next token

        return result;
    }

    string parseString() {
        string result;
        string tempVar = icg.newTemp();           // Generate a temporary variable for the final result.
        icg.addInstruction(tempVar + " = \"\"");  // Initialize the tempVar with an empty string.

        while (tokens[pos].type != T_SEMICOLON) {
            if (tokens[pos].type == T_WORD) {                                      // Handle string literals
                string strTemp = icg.newTemp();                                    // Generate a temporary variable for this string.
                icg.addInstruction(strTemp + " = \"" + tokens[pos].value + "\"");  // Assign the literal to temp.
                icg.addInstruction(tempVar + " = " + tempVar + " + " + strTemp);   // Concatenate to the final result.
                pos++;                                                             // Move to the next token
            } else if (tokens[pos].type == T_PLUS) {                               // Handle the concatenation operator
                pos++;                                                             // Skip the `+` operator
            } else if (tokens[pos].type == T_ID) {                                 // Handle variables in the string
                string varName = tokens[pos].value;                                // Get the variable name

                SymbolType type = symTable.getVariableType(varName);           // Get the type of the variable from the symbol table
                if (type != SymbolType::STRING && type != SymbolType::CHAR) {  // Ensure the variable is a string
                    ThrowError::sementicErrorInvalidType(varName, tokens[pos].lineNumber, "String", Utils::getSymbolTypeString(type));
                }
                icg.addInstruction(tempVar + " = " + tempVar + " + " + varName);  // Concatenate the variable to the final result.
                pos++;                                                            // Move to the next token
            } else {
                ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);  // Invalid token
            }
        }

        return tempVar;  // Return the temporary variable holding the final result.
    }

    /*
     parseAssignment handles the parsing of variable assignments.
     It expects an identifier (variable name), an assignment operator `=`, followed by an expression,
     and a semicolon at the end of the statement.
     It checks if the variable is declared in the symbol table, parses the expression, and generates intermediate code
     for the assignment.
     Example:
     x = 10;   -->  This will be parsed, checking if x is declared, then generating intermediate code like `x = 10`.
    */
    void parseAssignment(bool semiColon = true, bool canBeDecalaration = false) {
        TokenType type = tokens[pos].type;
        if ((type == T_INT || type == T_STRING || type == T_CHAR || type == T_FLOAT || type == T_BOOL) && canBeDecalaration) {
            parseDeclaration(true);
            return;
        } else if (canBeDecalaration) {
            ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);
        }
        string varName = expectAndReturnValue(T_ID);
        type = Utils::getSymbolType(symTable.getVariableType(varName));  // Ensure the variable is declared in the symbol table.
        if (symTable.isConstant(varName)) {
            ThrowError::sementicErrorConstantAssignment(varName, tokens[pos].lineNumber);
        }
        expect(T_ASSIGN);
        string expr = "";
        if (type == T_INT) {
            expr = parseExpression();
        } else if (type == T_STRING) {
            expr = parseString();
        } else if (type == T_CHAR) {
            expr = '"' + parseCharacter() + '"';
        } else if (type == T_FLOAT) {
            expr = parseExpression(T_FLOAT);
        } else if (type == T_BOOL) {
            expr = parseBooleanExpression();
        }
        icg.addInstruction(varName + " = " + expr);  // Generate intermediate code for the assignment.
        if (semiColon)
            expect(T_SEMICOLON);
    }
    /*
         parseIfStatement handles the parsing of `if` statements.
         It expects the keyword `if`, followed by an expression in parentheses that serves as the condition.
         If the condition evaluates to true, it executes the statement inside the block. If an `else` part is present,
         it executes the corresponding statement after the `else` keyword.
         Intermediate code for the `if` statement is generated, including labels for conditional jumps.
         Example:
         if(5 > 3) { x = 20; }  --> This will generate intermediate code for the condition check and jump instructions.
    */
    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);                 // Expect and consume the opening parenthesis for the condition.
        string cond = parseExpression();  // Parse the condition expression inside the parentheses.
        expect(T_RPAREN);

        string temp = icg.newTemp();              // Generate a new temporary variable for the condition result.
        icg.addInstruction(temp + " = " + cond);  // Generate intermediate code for storing the condition result.

        string trueLabel = icg.newLabel();   // Generate a label for the true branch.
        string falseLabel = icg.newLabel();  // Generate a label for the false branch.
        string endLabel = icg.newLabel();    // Generate a label for the end of the if-else statement.

        icg.addInstruction("if " + temp + " goto " + trueLabel);  // Jump to trueLabel if the condition is true.
        icg.addInstruction("goto " + falseLabel);                 // Otherwise, jump to falseLabel.
        icg.addInstruction(trueLabel + ":");                      // Mark the true branch label.

        parseStatement();  // Parse the statement for the true branch.

        if (tokens[pos].type == T_ELSE) {            // If an `else` part exists, handle it.
            icg.addInstruction("goto " + endLabel);  // Jump to the end after executing the true branch.
            icg.addInstruction(falseLabel + ":");    // Mark the false branch label.

            expect(T_ELSE);
            parseStatement();  // Parse the statement inside the else block.

            icg.addInstruction(endLabel + ":");  // Mark the end of the if-else statement.
        } else {
            icg.addInstruction(falseLabel + ":");  // Mark the false branch label (no else block).
        }
    }
    /*
        parseReturnStatement handles the parsing of `return` statements.
        It expects the keyword `return`, followed by an expression to return, and a semicolon to terminate the statement.
        It generates intermediate code to represent the return of the expression.
        Example:
        return x + 5;   -->  This will generate intermediate code like `return x + 5`.
    */
    void parseReturnStatement() {
        expect(T_RETURN);
        string expr = parseExpression();
        icg.addInstruction("return " + expr);  // Generate intermediate code for the return statement.
        expect(T_SEMICOLON);
    }
    /*
        parseBlock handles the parsing of block statements, which are enclosed in curly braces `{ }`.
        It parses the statements inside the block recursively until it reaches the closing brace.
        Example:
        { x = 10; y = 20; }   -->  This will parse each statement inside the block.
    */
    void parseBlock() {
        expect(T_LBRACE);  // Expect and consume the opening brace `{`.
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();  // Parse the statements inside the block.
        }
        expect(T_RBRACE);
    }
    /*
        parseExpression handles the parsing of expressions involving addition, subtraction, or comparison operations.
        It first parses a term, then processes addition (`+`) or subtraction (`-`) operators if present, generating
        intermediate code for the operations.
        Example:
        5 + 3 - 2;  -->  This will generate intermediate code like `t0 = 5 + 3` and `t1 = t0 - 2`.
    */
    string parseExpression(TokenType type = TokenType::T_INT) {
        string term = parseTerm(type);
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            TokenType op = tokens[pos++].type;
            string nextTerm = parseTerm(type);                                                    // Parse the next term in the expression.
            string temp = icg.newTemp();                                                          // Generate a temporary variable for the result
            icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm);  // Intermediate code for operation
            term = temp;
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            string nextExpr = parseExpression(type);                     // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                 // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " > " + nextExpr);  // Intermediate code for the comparison.
            term = temp;
        }
        if (tokens[pos].type == T_LT) {
            pos++;
            string nextExpr = parseExpression(type);                     // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                 // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " < " + nextExpr);  // Intermediate code for the comparison.
            term = temp;
        }
        if (tokens[pos].type == T_EGT) {
            pos++;
            string nextExpr = parseExpression(type);                      // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                  // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " >= " + nextExpr);  // Intermediate code for the comparison.
            term = temp;
        }
        if (tokens[pos].type == T_ELT) {
            pos++;
            string nextExpr = parseExpression(type);                      // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                  // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " <= " + nextExpr);  // Intermediate code for the comparison.
            term = temp;
        }
        return term;
    }
    /*
        parseTerm handles the parsing of terms involving multiplication or division operations.
        It first parses a factor, then processes multiplication (`*`) or division (`/`) operators if present,
        generating intermediate code for the operations.
        Example:
        5 * 3 / 2;   This will generate intermediate code like `t0 = 5 * 3` and `t1 = t0 / 2`.
    */
    string parseTerm(TokenType type = TokenType::T_INT) {
        string factor = parseFactor(type);
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            TokenType op = tokens[pos++].type;
            string nextFactor = parseFactor(type);
            string temp = icg.newTemp();                                                             // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + factor + (op == T_MUL ? " * " : " / ") + nextFactor);  // Intermediate code for operation.
            factor = temp;                                                                           // Update the factor to be the temporary result.
        }
        return factor;
    }
    /*
        parseFactor handles the parsing of factors in expressions, which can be either numeric literals, identifiers
        (variables), or expressions inside parentheses (for sub-expressions).
        Example:
        5;          -->  This will return the number "5".
        x;          -->  This will return the identifier "x".
        (5 + 3);    --> This will return the sub-expression "5 + 3".
    */
    string parseFactor(TokenType type = TokenType::T_INT) {
        if (tokens[pos].type == T_NUM && type == T_INT) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_DECIMAL && type == T_FLOAT) {  // Handle float literals
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_ID) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            string expr = parseExpression(type);
            expect(T_RPAREN);
            return expr;
        } else {
            ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);
            return "";
        }
    }

    string parseBooleanExpression(TokenType type = TokenType::T_BOOL) {
        string term = parseBooleanTerm(type);
        while (tokens[pos].type == T_OR) {  // Handle 'or' logical operator
            TokenType op = tokens[pos++].type;
            string nextTerm = parseBooleanTerm(type);                     // Parse the next term in the boolean expression.
            string temp = icg.newTemp();                                  // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " || " + nextTerm);  // Intermediate code for 'or' operation.
            term = temp;
        }
        return term;
    }

    string parseBooleanTerm(TokenType type = TokenType::T_BOOL) {
        string factor = parseBooleanFactor(type);
        while (tokens[pos].type == T_AND) {  // Handle 'and' logical operator
            TokenType op = tokens[pos++].type;
            string nextFactor = parseBooleanFactor(type);
            string temp = icg.newTemp();                                      // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + factor + " && " + nextFactor);  // Intermediate code for 'and' operation.
            factor = temp;                                                    // Update the factor to be the temporary result.
        }
        return factor;
    }

    string parseBooleanFactor(TokenType type = TokenType::T_BOOL) {
        if (tokens[pos].type == T_NOT) {  // Handle 'not' logical operator
            TokenType op = tokens[pos++].type;
            string nextFactor = parseBooleanFactor(type);    // Parse the next factor for the 'not' operation.
            string temp = icg.newTemp();                     // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = !" + nextFactor);  // Intermediate code for 'not' operation.
            return temp;
        } else if (tokens[pos].type == T_BOOLEAN) {  // Handle Boolean literals (true/false)
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_ID) {  // Handle identifiers (variables)
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_LPAREN) {  // Handle sub-expressions in parentheses
            expect(T_LPAREN);
            string expr = parseBooleanExpression(type);
            expect(T_RPAREN);
            return expr;
        } else {
            ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);
            return "";
        }
    }

    /*
    Why both functions are needed:
    - The `expect` function is useful when you are only concerned with ensuring the correct token type without needing its value.
    - For example, ensuring a semicolon `;` or a keyword `if` is present in the source code.
    - The `expectAndReturnValue` function is needed when the parser not only needs to check for a specific token but also needs to use the value of that token in the next stages of compilation or interpretation.
    - For example, extracting the name of a variable (`T_ID`) or the value of a constant (`T_NUMBER`) to process it in a symbol table or during expression evaluation.
    */
    /*
         expect function:
         This functin is used to check whether the current token matches the expected type.
         If the token type does not match the expected type, an error message is displayed
         and the program exits. If the token type matches, it advances the position to the next token.
     */
    void expect(TokenType type) {
        if (tokens[pos].type != type) {
            cout << "Syntax error: expected '" << type << "' at line " << tokens[pos].lineNumber << endl;
            cout << "Token Value: " << tokens[pos].value << "  Token LineNumber: " << tokens[pos].lineNumber << "  Token Type: " << tokens[pos].type << endl;
            exit(1);
        }
        pos++;
    }
    /*
    Explanation:
    - The `expect` function ensures that the parser encounters the correct tokens in the expected order.
    - It's mainly used for non-value-based tokens, such as keywords, operators, and delimiters (e.g., semicolons).
    - If the parser encounters an unexpected token, it halts the process by printing an error message, indicating where the error occurred (line number) and what was expected.
    - The `pos++` advances to the next token after confirming the expected token is present.

    Use Case:
    - This function is helpful when checking for the correct syntax or structure in a language's grammar, ensuring the parser processes the tokens in the correct order.
    */
    string expectAndReturnValue(TokenType type) {
        string value = tokens[pos].value;
        expect(type);
        return value;
    }

    void parseWhileStatement() {
        expect(T_WHILE);                  // Expect and consume the `while` keyword.
        expect(T_LPAREN);                 // Expect and consume the opening parenthesis for the condition.
        string cond = parseExpression();  // Parse the condition expression inside the parentheses.
        expect(T_RPAREN);                 // Expect and consume the closing parenthesis.

        string startLabel = icg.newLabel();  // Generate a new label for the start of the loop.
        string endLabel = icg.newLabel();    // Generate a new label for the end of the loop.
        string temp = icg.newTemp();         // Generate a temporary variable for the condition result.

        icg.addInstruction(startLabel + ":");                    // Start of the loop.
        icg.addInstruction(temp + " = " + cond);                 // Evaluate the condition.
        icg.addInstruction("if " + temp + " goto " + endLabel);  // Exit loop if condition is false.

        parseStatement();  // Parse the statement inside the loop.

        icg.addInstruction("goto " + startLabel);  // Jump back to the start of the loop.
        icg.addInstruction(endLabel + ":");        // End of the loop.
    }

    void parseForStatement() {
        expect(T_FOR);     // Expect "for"
        expect(T_LPAREN);  // Expect "("

        parseAssignment(true, true);  // Expect ";"

        string condition = parseExpression();  // Parse the condition
        expect(T_SEMICOLON);                   // Expect ";"

        parseAssignment(false);  // Parse the increment statement
        expect(T_RPAREN);        // Expect ")"

        string startLabel = icg.newLabel();
        string endLabel = icg.newLabel();
        string incrementLabel = icg.newLabel();

        icg.addInstruction(startLabel + ":");  // Label for the start of the loop
        icg.addInstruction("if " + condition + " goto " + incrementLabel);
        icg.addInstruction("goto " + endLabel);
        icg.addInstruction(incrementLabel + ":");

        parseStatement();  // Parse the statement block
        // icg.addInstruction(increment);  // Execute increment
        icg.addInstruction("goto " + startLabel);
        icg.addInstruction(endLabel + ":");  // Label for the end of the loop
    }

    void parseFunctionDeclaration(SymbolType returnType, string funcName) {
        // Generate TAC for function start
        icg.addInstruction("func " + funcName + " :");

        expect(T_LPAREN);  // Expect '('
        vector<pair<SymbolType, string>> params = parseParameterList();

        if (symTable.isFunctionDeclared(funcName, returnType, params)) {
            ThrowError::sementicErrorFunctionAlreadyDeclared(funcName, tokens[pos].lineNumber);
        }

        // Generate TAC for parameters
        for (const auto &param : params) {
            icg.addInstruction("param " + Utils::getSymbolTypeString(param.first) + " " + param.second);
        }

        expect(T_RPAREN);  // Expect ')'

        // If the function is a prototype (ends with ';'), just declare it
        if (tokens[pos].type == T_SEMICOLON) {
            pos++;
            symTable.declareFunction(funcName, returnType, params, tokens[pos].lineNumber);
            icg.addInstruction("endfunc");
            return;
        }

        // Declare the function in the symbol table
        symTable.declareFunction(funcName, returnType, params, tokens[pos].lineNumber);

        // Parse the function body and generate TAC for the block
        parseBlock();

        // Generate TAC for function end
        icg.addInstruction("endfunc");
    }

    vector<pair<SymbolType, string>> parseParameterList() {
        vector<pair<SymbolType, string>> parameters;

        // If the next token is not a closing parenthesis, we expect parameters to be present
        if (tokens[pos].type != T_RPAREN) {
            // Parse the first parameter
            SymbolType type = parseType();                  // Parse the type of the parameter (e.g., int, float)
            string paramName = expectAndReturnValue(T_ID);  // Expect and parse the parameter name (ID)

            parameters.push_back(make_pair(type, paramName));  // Store type and name as a pair

            // Check if there are more parameters
            while (tokens[pos].type == T_COMMA) {
                pos++;                                   // Skip the comma
                type = parseType();                      // Parse the type of the next parameter
                paramName = expectAndReturnValue(T_ID);  // Expect and parse the parameter name

                parameters.push_back(make_pair(type, paramName));  // Store parameter
            }
        }

        // Return the list of parameters (type, name) pairs
        return parameters;
    }

    SymbolType parseType() {
        SymbolType st;
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING || tokens[pos].type == T_CHAR) {
            st = Utils::getTokenType(tokens[pos].type);
            pos++;
        } else {
            // Handle invalid type
            ThrowError::unExpectedTokenError(tokens[pos].value, tokens[pos].lineNumber);
        }
        return st;
    }

    // string parseFunctionCall() {
    //     string funcName = expectAndReturnValue(T_ID);  // Expect function name
    //     expect(T_LPAREN);                      // Expect '('
    //     vector<string> args = parseArgumentList();
    //     expect(T_RPAREN);  // Expect ')'

    //     // Generate TAC for function call
    //     string temp = icg.newTemp();
    //     string argString = join(args, ", ");
    //     icg.addInstruction(temp + " = CALL " + funcName + " (" + argString + ")");
    //     return temp;
    // }
};

class TACToAssemblyConverter {
   private:
    std::vector<std::string> tac;       // Stores the three-address code
    std::vector<std::string> assembly;  // Stores the generated assembly code

    // Priority queue to manage registers (least recently used is prioritized)
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<>> registerQueue;
    std::unordered_map<std::string, int> registerUsage;               // Track register usage order
    std::unordered_map<std::string, std::string> variableToRegister;  // Map variables to allocated registers
    std::unordered_map<std::string, std::string> registerToVariable;  // Map registers to current variables
    int usageCounter = 0;                                             // Global counter for usage order

    // Initialize the priority queue with 8 registers
    void initializeRegisters() {
        for (int i = 1; i <= 8; ++i) {
            std::string reg = "R" + std::to_string(i);
            registerQueue.emplace(0, reg);
            registerUsage[reg] = 0;
        }
    }

    // Get the least recently used register and update its usage
    std::string getRegister(string &variable) {
        if (variableToRegister.find(variable) != variableToRegister.end()) {
            return variableToRegister[variable];
        }

        auto [_, reg] = registerQueue.top();
        registerQueue.pop();

        // If the register is already holding a variable, remove the mapping
        if (registerToVariable.find(reg) != registerToVariable.end()) {
            std::string oldVariable = registerToVariable[reg];
            variableToRegister.erase(oldVariable);
        }

        // Update mappings
        registerToVariable[reg] = variable;
        variableToRegister[variable] = reg;

        registerUsage[reg] = ++usageCounter;
        registerQueue.emplace(usageCounter, reg);
        return reg;
    }

    // Helper function to split a string by spaces
    std::vector<std::string> split(const std::string &line, char delimiter = ' ') {
        std::vector<std::string> tokens;
        std::istringstream stream(line);
        std::string token;
        while (std::getline(stream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Function to handle arithmetic instructions
    void handleArithmetic(const std::vector<std::string> &tokens) {
        std::string target = tokens[0];
        std::string op1 = tokens[2];
        std::string op = tokens[3];
        std::string op2 = tokens[4];

        std::string reg1 = getRegister(op1);
        assembly.push_back("MOV " + reg1 + ", " + op1);

        if (op == "+") {
            assembly.push_back("ADD " + reg1 + ", " + op2);
        } else if (op == "-") {
            assembly.push_back("SUB " + reg1 + ", " + op2);
        } else if (op == "*") {
            assembly.push_back("MUL " + reg1 + ", " + op2);
        } else if (op == "/") {
            assembly.push_back("DIV " + reg1 + ", " + op2);
        }

        assembly.push_back("MOV " + target + ", " + reg1);
    }

    // Function to handle assignment instructions
    void handleAssignment(const std::vector<std::string> &tokens) {
        std::string target = tokens[0];
        std::string value = tokens[2];
        std::string reg = getRegister(value);
        assembly.push_back("MOV " + reg + ", " + value);
        assembly.push_back("MOV " + target + ", " + reg);
    }

    // Function to handle conditional jumps
    void handleCondition(const std::vector<std::string> &tokens) {
        std::string op1 = tokens[1];
        std::string condition = tokens[2];
        std::string op2 = tokens[3];
        std::string label = tokens[5];

        std::string reg = getRegister(op1);
        assembly.push_back("MOV " + reg + ", " + op1);
        assembly.push_back("CMP " + reg + ", " + op2);
        if (condition == "<") {
            assembly.push_back("JL " + label);
        } else if (condition == ">") {
            assembly.push_back("JG " + label);
        } else if (condition == "<=") {
            assembly.push_back("JLE " + label);
        } else if (condition == ">=") {
            assembly.push_back("JGE " + label);
        } else if (condition == "==") {
            assembly.push_back("JE " + label);
        } else if (condition == "!=") {
            assembly.push_back("JNE " + label);
        }
    }

    // Function to handle logical operations
    void handleLogical(const std::vector<std::string> &tokens) {
        std::string target = tokens[0];
        std::string op1 = tokens[2];
        std::string op = tokens[3];
        std::string op2 = tokens[4];

        std::string reg = getRegister(op1);
        if (op == "&&") {
            assembly.push_back("MOV " + reg + ", " + op1);
            assembly.push_back("AND " + reg + ", " + op2);
        } else if (op == "||") {
            assembly.push_back("MOV " + reg + ", " + op1);
            assembly.push_back("OR " + reg + ", " + op2);
        } else if (op == "!") {
            assembly.push_back("MOV " + reg + ", " + op1);
            assembly.push_back("NOT " + reg);
        }
        assembly.push_back("MOV " + target + ", " + reg);
    }

    // Function to handle labels
    void handleLabel(const std::string &label) {
        assembly.push_back(label);
    }

    // Function to handle function definitions
    void handleFunction(const std::vector<std::string> &tokens) {
        assembly.push_back(tokens[1] + ":");
    }

    // Function to handle function calls
    void handleFunctionCall(const std::vector<std::string> &tokens) {
        for (size_t i = 1; i < tokens.size(); ++i) {
            assembly.push_back("PUSH " + tokens[i]);
        }
        assembly.push_back("CALL " + tokens[0]);
        assembly.push_back("ADD SP, " + std::to_string(tokens.size() - 1));
    }

   public:
    // Constructor to initialize the three-address code
    TACToAssemblyConverter(const std::vector<std::string> &tacCode) : tac(tacCode) {
        initializeRegisters();
    }

    // Function to convert TAC to assembly
    void convert() {
        for (const std::string &line : tac) {
            std::vector<std::string> tokens = split(line);

            if (tokens.empty()) continue;

            if (tokens.size() == 3 && tokens[1] == "=") {
                handleAssignment(tokens);
            } else if (tokens.size() == 5 && (tokens[3] == "+" || tokens[3] == "-" || tokens[3] == "*" || tokens[3] == "/")) {
                handleArithmetic(tokens);
            } else if (tokens[0] == "if") {
                handleCondition(tokens);
            } else if (tokens[0] == "goto") {
                assembly.push_back("JMP " + tokens[1]);
            } else if (tokens[0].back() == ':') {
                handleLabel(tokens[0]);
            } else if (tokens[0] == "func") {
                handleFunction(tokens);
            } else if (tokens[0] == "call") {
                handleFunctionCall(tokens);
            } else if (tokens.size() == 5 && (tokens[3] == "&&" || tokens[3] == "||" || tokens[3] == "!")) {
                handleLogical(tokens);
            }
        }
    }

    // Function to print the assembly code
    void printAssembly() {
        for (const std::string &line : assembly) {
            std::cout << line << std::endl;
        }
    }
};

int main() {
    string src = R"(

    // Integer Functionality    
    int x;
    x = 10;
    int y;
    y = 20;
    int sum;
    sum = x + y * 3;

    // Char Functionality
    char t = 'a';
    t='b';
    char ch = '4';

    // Float Functionality 
    float s = 0.2 + 9.4;
    float m = 0.4;
    s = s + m;
    s = (s + 0.5) * (m - 0.1) / 0.5;

    // String Functionality
    string f="sad"+"loif";
    string g = " Mateen";
    string h = "Hello" + g;
    f = f + " " + t;

    // Boolean Functionality
    bool flag = false;
    bool flag2 = true || (flag && true);
    flag = !flag;

    // Const Keyword
    const int z = 10;
    // z= 4;

    // Comments
    /*
        multi line comment
    */

    // If Else Functionality
    if( 5 > 3 ){
        x = 20;
        if ( 3 > 2 ){
            x = 30;
        }
        else {
            x = 40;
        }
    }
    else {
        x = 30;
    }


    // // Loops
    for(int i = 0 ;i <= 10 ;i=i+1){
        i = i+1;
    }

    while(x > 0){
        x = x - 1;
    }

    // Function Declaration
    string add(int val, int val2){
        return val + val2;
    }

    )";

    Lexer lexer(src);
    vector<Token> tokens = lexer.tokenize();
    cout << "---------------------------------------------------<Tokenization Complete>-------------------------------------------------------------" << endl;
    // lexer.printTokens(tokens);

    SymbolTable symTable;
    IntermediateCodeGnerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    cout << "---------------------------------------------------<Parsing Complete>-------------------------------------------------------------" << endl;
    // icg.printInstructions();
    vector<string> tac = icg.getInstructions();

    cout << "---------------------------------------------------<Three Address Code>-------------------------------------------------------------" << endl;
    TACToAssemblyConverter converter(tac);
    converter.convert();
    converter.printAssembly();

    return 0;
}