#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum TokenType {
    T_INT,
    T_ID,
    T_NUM,
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
    T_GT,
    T_LT,
    T_WHILE,
    T_FOR,
    T_EOF
};

struct Token {
    TokenType type;
    string value;
    int lineNumber;
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
                tokens.push_back(Token{T_NUM, consumeNumber(), lineNumber});
                continue;
            }
            if (isalpha(current)) {
                string word = consumeWord();
                TokenType type = T_ID;

                if (word == "int")
                    type = T_INT;
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

            switch (current) {
                case '=':
                    tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
                    break;
                case '+':
                    tokens.push_back(Token{T_PLUS, "+", lineNumber});
                    break;
                case '-':
                    tokens.push_back(Token{T_MINUS, "-", lineNumber});
                    break;
                case '*':
                    tokens.push_back(Token{T_MUL, "*", lineNumber});
                    break;
                case '/':
                    tokens.push_back(Token{T_DIV, "/", lineNumber});
                    break;
                case '(':
                    tokens.push_back(Token{T_LPAREN, "(", lineNumber});
                    break;
                case ')':
                    tokens.push_back(Token{T_RPAREN, ")", lineNumber});
                    break;
                case '{':
                    tokens.push_back(Token{T_LBRACE, "{", lineNumber});
                    break;
                case '}':
                    tokens.push_back(Token{T_RBRACE, "}", lineNumber});
                    break;
                case ';':
                    tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
                    break;
                case '>':
                    tokens.push_back(Token{T_GT, ">", lineNumber});
                    break;
                // TODO: Add support for '<' operator
                default:
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
        while (pos < src.size() && isdigit(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }
};

class SymbolTable {
   public:
    void declareVariable(const string &name, const string &type) {
        if (symbolTable.find(name) != symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    string getVariableType(const string &name) {
        if (symbolTable.find(name) == symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const {
        return symbolTable.find(name) != symbolTable.end();
    }

   private:
    map<string, string> symbolTable;
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
        if (tokens[pos].type == T_INT) {
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
        } else {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
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
    void parseDeclaration() {
        expect(T_INT);                                // Expect and consume the int keyword.
        string varName = expectAndReturnValue(T_ID);  // Expect and return the variable name (identifier).
        symTable.declareVariable(varName, "int");     // Register the variable in the symbol table with type "int".
        expect(T_SEMICOLON);                          // Expect the semicolon to end the statement.
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
    void parseAssignment(bool semiColon = true) {
        string varName = expectAndReturnValue(T_ID);
        symTable.getVariableType(varName);  // Ensure the variable is declared in the symbol table.
        expect(T_ASSIGN);
        string expr = parseExpression();
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
    string parseExpression() {
        string term = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            TokenType op = tokens[pos++].type;
            string nextTerm = parseTerm();                                                        // Parse the next term in the expression.
            string temp = icg.newTemp();                                                          // Generate a temporary variable for the result
            icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm);  // Intermediate code for operation
            term = temp;
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            string nextExpr = parseExpression();                         // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                 // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " > " + nextExpr);  // Intermediate code for the comparison.
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
    string parseTerm() {
        string factor = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            TokenType op = tokens[pos++].type;
            string nextFactor = parseFactor();
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
    string parseFactor() {
        if (tokens[pos].type == T_NUM) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_ID) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        } else {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }
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

    /*
        Why both functions are needed:
        - The `expect` function is useful when you are only concerned with ensuring the correct token type without needing its value.
        - For example, ensuring a semicolon `;` or a keyword `if` is present in the source code.
        - The `expectAndReturnValue` function is needed when the parser not only needs to check for a specific token but also needs to use the value of that token in the next stages of compilation or interpretation.
        - For example, extracting the name of a variable (`T_ID`) or the value of a constant (`T_NUMBER`) to process it in a symbol table or during expression evaluation.
    */

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

        parseAssignment();  // Expect ";"

        string condition = parseExpression();  // Parse the condition
        expect(T_SEMICOLON);                   // Expect ";"

        parseAssignment(false);  // Parse the increment statement
        expect(T_RPAREN);   // Expect ")"

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
};

class TACToAssemblyConverter {
   private:
    std::vector<std::string> tac;                                  // Stores the three-address code
    std::vector<std::string> assembly;                             // Stores the generated assembly code
    std::unordered_map<std::string, std::string> tempRegisterMap;  // Map for temp variables to registers

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

        if (op == "*") {
            assembly.push_back("MOV R1, " + op1);
            assembly.push_back("MUL R1, " + op2);
            tempRegisterMap[target] = "R1";
        } else if (op == "+") {
            assembly.push_back("MOV R1, " + op1);
            assembly.push_back("ADD R1, " + op2);
            tempRegisterMap[target] = "R1";
        }
    }

    // Function to handle assignment instructions
    void handleAssignment(const std::vector<std::string> &tokens) {
        std::string target = tokens[0];
        std::string value = tokens[2];
        assembly.push_back("MOV " + target + ", " + value);
    }

    // Function to handle conditional jumps
    void handleCondition(const std::vector<std::string> &tokens) {
        std::string condition = tokens[2];
        std::string op1 = tokens[1];
        std::string op2 = tokens[3];
        std::string label = tokens[5];

        assembly.push_back("MOV R1, " + op1);
        assembly.push_back("CMP R1, " + op2);
        if (condition == ">") {
            assembly.push_back("JG " + label);
        }
    }

   public:
    // Constructor to initialize the three-address code
    TACToAssemblyConverter(const std::vector<std::string> &tacCode) : tac(tacCode) {}

    // Function to convert TAC to assembly
    void convert() {
        for (const std::string &line : tac) {
            std::vector<std::string> tokens = split(line);

            if (tokens.empty()) continue;

            if (tokens.size() == 3 && tokens[1] == "=") {
                handleAssignment(tokens);
            } else if (tokens.size() == 5 && tokens[3] == "*") {
                handleArithmetic(tokens);
            } else if (tokens[0] == "if") {
                handleCondition(tokens);
            } else if (tokens[0] == "goto") {
                assembly.push_back("JMP " + tokens[1]);
            } else if (tokens[0].back() == ':') {
                assembly.push_back(tokens[0]);  // Labels
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
    int x;
    int i;
    for(i = 0 ;10>i;i=i+1){
        i= i+1;
    }
    x = 10;
    int y;
    y = 20;
    int sum;
    sum = x + y * 3;

    if(5 > 3){
        x = 20;
    }
    else
    {
        x = 30;
    }
    while(x > 0){
        x = x - 1;
    }
    )";


    Lexer lexer(src);
    vector<Token> tokens = lexer.tokenize();
    cout << "---------------------------------------------------<Tokenization Complete>-------------------------------------------------------------" << endl;

    SymbolTable symTable;
    IntermediateCodeGnerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    cout << "---------------------------------------------------<Parsing Complete>-------------------------------------------------------------" << endl;
    icg.printInstructions();
    // vector<string> tac = icg.getInstructions();

    // std::vector<std::string> tac = {
    //     "x = 10",
    //     "y = 20",
    //     "t0 = y * 3",
    //     "t1 = x + t0",
    //     "sum = t1",
    //     "t2 = 5 > 3",
    //     "t3 = t2",
    //     "if t3 goto L1",
    //     "goto L2",
    //     "L1:",
    //     "x = 20",
    //     "L2:"};

    // TACToAssemblyConverter converter(tac);
    // converter.convert();
    // converter.printAssembly();

    return 0;
}