#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <fstream>
using namespace std;

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    NUMBER,
    PUNCTUATION,
    UNKNOWN
};

struct Token
{
    string lexeme;
    TokenType type;
};

unordered_set<string>
    key_words = {"if", "else", "for", "while", "float", "char", "void", "double", "return", "int"};
unordered_set<char> operators = {'+', '-', '*', '/', '=', '>', '<', '&', '|', '!'};
unordered_set<char> punctuations = {'(', ')', '{', '}', '[', ']', ';', ','};

bool isKeyword(const string &str)
{
    return key_words.find(str) != key_words.end();
}

bool isOperator(char c)
{
    return operators.find(c) != operators.end();
}

bool isPunctuation(char c)
{
    return punctuations.find(c) != punctuations.end();
}

vector<Token> tokenize(const string &input)
{
    vector<Token> tokens;
    int i = 0;
    while (i < input.length())
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        if (isalpha(input[i]))
        {
            string lexeme;
            while (isalnum(input[i]))
            {
                lexeme += input[i];
                i++;
            }
            if (isKeyword(lexeme))
            {
                tokens.push_back({lexeme, KEYWORD});
            }
            else
            {
                tokens.push_back({lexeme, IDENTIFIER});
            }
        }
        else if (isdigit(input[i]))
        {
            string lexeme;
            while (isdigit(input[i]))
            {
                lexeme += input[i];
                i++;
            }
            tokens.push_back({lexeme, NUMBER});
        }
        else if (isOperator(input[i]))
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, OPERATOR});
            i++;
        }
        else if (isPunctuation(input[i]))
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, PUNCTUATION});
            i++;
        }
        else
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, UNKNOWN});
            i++;
        }
    }
    return tokens;
}

void printTokens(const vector<Token> &tokens)
{
    for (const Token &token : tokens)
    {
        cout << "Token: " << token.lexeme << ", Type: ";
        switch (token.type)
        {
        case KEYWORD:
            cout << "Keyword";
            break;
        case IDENTIFIER:
            cout << "Identifier";
            break;
        case OPERATOR:
            cout << "Operator";
            break;
        case NUMBER:
            cout << "Number";
            break;
        case PUNCTUATION:
            cout << "Punctuation";
            break;
        case UNKNOWN:
            cout << "Unknown";
            break;
        default:
            break;
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    ifstream inputFile(argv[1]);

    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    string line;

    while (getline(inputFile, line))
    {
        vector<Token> tokens = tokenize(line);
        printTokens(tokens);
    }

    inputFile.close();

    return 0;
}