%{
#include <stdio.h>
#include <ctype.h>

int lineno = 1; // Track line number
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
KEYWORD     "if"|"else"|"for"|"while"|"int"|"return"

%%
"//".*                            { /* Skip single-line comments */ }
" "|\t                            { /* Skip whitespace */ }
\n                                { lineno++; }
{KEYWORD}                         { printf("Keyword: %s\n", yytext); }
{ID}                              { printf("Identifier: %s\n", yytext); }
{DIGIT}+                          { printf("Number: %s\n", yytext); }
"=="|"!="|"<="|">="|"<"|">"       { printf("Relational Operator: %s\n", yytext); }
"+"|"-"|"*"|"/"                   { printf("Arithmetic Operator: %s\n", yytext); }
";"|"{"|"}"|"("|")"                   { printf("Delimiter: %s\n", yytext); }
.                                 { printf("Not Found: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    yylex();
    return 0;
}