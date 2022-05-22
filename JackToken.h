//
// Created by robin on 2022/4/30.
//

#ifndef SYNTAXANALYZER_JACKTOKEN_H
#define SYNTAXANALYZER_JACKTOKEN_H

#include <string>
#include <set>
#include "fstream"

using namespace std;

// Token types
enum TokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

// Keyword types
enum KeyWordType {CLASS, METHOD, INT, FUNCTION, BOOLEAN, CONSTRUCTOR, CHAR, VOID, VAR, STATIC, FIELD, LET, DO,
    IF,ELSE, WHILE, RETURN, TRUE, FALSE, NL, THIS};

class JackToken {

public:
    JackToken(string filename);

    ~JackToken();

    bool hasMoreTokens();

    void advance();

    TokenType tokenType();

    KeyWordType keyWord();

    string symbol();

    string identifier();

    int intVal();

    string stringVal();

    void write();

    ifstream fin;

    string command;

    ofstream fout;

    string currentToken;
};


#endif //SYNTAXANALYZER_JACKTOKEN_H

