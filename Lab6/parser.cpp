#include <cctype>
#include <iostream>
#include <map>
#include <string>
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
    T_EOF,
};
string TokenTypeToString(TokenType type) {
    switch (type) {
        case T_INT:
            return "T_INT";
        case T_ID:
            return "T_ID";
        case T_NUM:
            return "T_NUM";
        case T_IF:
            return "T_IF";
        case T_ELSE:
            return "T_ELSE";
        case T_RETURN:
            return "T_RETURN";
        case T_ASSIGN:
            return "T_ASSIGN";
        case T_PLUS:
            return "T_PLUS";
        case T_MINUS:
            return "T_MINUS";
        case T_MUL:
            return "T_MUL";
        case T_DIV:
            return "T_DIV";
        case T_LPAREN:
            return "T_LPAREN";
        case T_RPAREN:
            return "T_RPAREN";
        case T_LBRACE:
            return "T_LBRACE";
        case T_RBRACE:
            return "T_RBRACE";
        case T_SEMICOLON:
            return "T_SEMICOLON";
        case T_GT:
            return "T_GT";
        case T_EOF:
            return "T_EOF";
        default:
            return "UNKNOWN_TOKEN";
    }
}
void printLineNumber(int line) { cout << "Error on line no: " << line << endl; }
struct Token {
    TokenType type;
    string value;
    int line;
};

class Lexer {
   private:
    string src;
    size_t pos;
    /*
    It hold positive values.
    In C++, size_t is an unsigned integer data type used to represent the
    size of objects in bytes or indices, especially when working with
    memory-related functions, arrays, and containers like vector or string. You
    can also use the int data type but size_t is recommended one
    */

   public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        int line = 0;
        while (pos < src.size()) {
            char current = src[pos];
            if (current == '\n') {
                pos++;
                line++;
                continue;
            }

            if (isspace(current)) {
                pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, line});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current) {
                case '=':
                    tokens.push_back(Token{T_ASSIGN, "=", line});
                    break;
                case '+':
                    tokens.push_back(Token{T_PLUS, "+", line});
                    break;
                case '-':
                    tokens.push_back(Token{T_MINUS, "-", line});
                    break;
                case '*':
                    tokens.push_back(Token{T_MUL, "*", line});
                    break;
                case '/':
                    tokens.push_back(Token{T_DIV, "/", line});
                    break;
                case '(':
                    tokens.push_back(Token{T_LPAREN, "(", line});
                    break;
                case ')':
                    tokens.push_back(Token{T_RPAREN, ")", line});
                    break;
                case '{':
                    tokens.push_back(Token{T_LBRACE, "{", line});
                    break;
                case '}':
                    tokens.push_back(Token{T_RBRACE, "}", line});
                    break;
                case ';':
                    tokens.push_back(Token{T_SEMICOLON, ";", line});
                    break;
                case '>':
                    tokens.push_back(Token{T_GT, ">", line});
                    break;
                default:
                    cout << "Unexpected character: " << current << endl;
                    printLineNumber(line);
                    exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, ""});
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

class Parser {
   public:
    Parser(const vector<Token> &tokens) {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

   private:
    vector<Token> tokens;
    size_t pos;

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
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value
                 << endl;
            printLineNumber(tokens[pos].line);
            exit(1);
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
    }
    void parseDeclaration() {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment() {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression() {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression();  // After relational operator, parse the next
                                // expression
        }
    }

    void parseTerm() {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            pos++;
            parseFactor();
        }
    }

    void parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
            pos++;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value
                 << endl;
            printLineNumber(tokens[pos].line);

            exit(1);
        }
    }

    void expect(TokenType type) {
        if (tokens[pos].type == type) {
            pos++;
        } else {
            cout << "Syntax error: expected " << TokenTypeToString(type)
                 << " but found " << tokens[pos].value << endl;
            printLineNumber(tokens[pos].line);

            exit(1);
        }
    }
};

int main() {
    string input = R"(
        int a;
        a = 5;
        int b;
        b = a + 10;
        if (b > 10)
        {
            return (a;
        } else {
            return 0;
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}