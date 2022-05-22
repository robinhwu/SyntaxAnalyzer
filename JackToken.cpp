//
// Created by robin on 2022/4/30.
//

#include "JackToken.h"
#include <string>
#include <iostream>
#include <regex>

using namespace std;

bool isString = false;

set<string> keyWordSet = {"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean",
                          "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};

set<char> symbolSet = {'{' , '}' , '(' , ')' , '[' , ']' , '.' , ',' , ';' , '+' , '-' , '*' , '/' , '&' , '|' , '<' ,
                       '>' , '=' , '~'};

JackToken::JackToken(string filename) {
    fin.open(filename + ".txt");
    if (!fin.is_open())
        cerr << "Error: Could not open " << filename << ".\n";
    ofstream output(filename + "Token.xml");
    fout = dynamic_cast<basic_ofstream<char> &&>(output);
    fout << "<tokens>" << endl;
}

JackToken::~JackToken() {
    if(fin.is_open()) {
        fin.close();
    }
    if (fout.is_open()) {
        fout << "</tokens>" << endl;
        fout.close();
    }
}

bool JackToken::hasMoreTokens() {
    return !fin.eof();
}

void JackToken::advance() {
    getline(fin, command);
    // Remove leading spaces
    command = std::regex_replace(command, std::regex("^ +"), "");
    // Remove trailing spaces
    command = regex_replace(command, std::regex(" +"), " ");
    if (!command.empty()) {
//        cout << command << endl;
        int lhs = 0, rhs = 0;
        currentToken = "";
        int size = command.size();

        for (int i = 0; i < size; i++) {
            if (isString) {
                if (command[i] == '"') {
                    rhs = i;
                    currentToken = command.substr(lhs, rhs-lhs+1);
                    write();
//                    fout << currentToken << endl;
                    isString = false;
                    lhs = rhs + 1;
                }
            } else {
                if (command[i] == '"') {
                    isString = true;
                    lhs = i;
                } else if (!isalnum(command[i])) {
                    rhs = i;
                    currentToken = command.substr(lhs, rhs-lhs);
                    if (!currentToken.empty()) {
                        write();
//                        fout << currentToken << endl;
                    }
                    if (!isspace(command[i])) {
                        currentToken = command[i];
                        write();
//                        fout <<  currentToken << endl;

                    }
                    lhs = rhs + 1;
                }
            }
        }
    }
}

TokenType JackToken::tokenType() {
    if (keyWordSet.count(currentToken)) {
        return KEYWORD;
    } else if (currentToken.size() == 1 && symbolSet.count(currentToken[0])) {
        return SYMBOL;
    } else if (currentToken.find_first_not_of("0123456789") == string::npos) {
        return INT_CONST;
    } else if (isString)  {
        return STRING_CONST;
    } else {
        return IDENTIFIER;
    }
}

void JackToken::write() {
    TokenType currentTokenType = tokenType();
    switch(currentTokenType) {
        case KEYWORD:
            fout << "<keyword> " << currentToken << " </keyword>" << endl;
            break;
        case SYMBOL:
            if (currentToken == "<") {
                currentToken = "&lt;";
            } else if (currentToken == ">") {
                currentToken = "&gt;";
            } else if (currentToken == "&") {
                currentToken = "&amp;";
            }
            fout << "<symbol> " << symbol() << " </symbol>" << endl;
            break;
        case INT_CONST:
            fout << "<integerConstant> " << intVal() << " </integerConstant>" << endl;
            break;
        case STRING_CONST:
            fout << "<stringConstant> " << stringVal() << " </stringConstant>" << endl;
            break;
        case IDENTIFIER:
            fout << "<identifier> " << identifier() << " </identifier>" << endl;
    }
}

KeyWordType JackToken::keyWord() {
    if (currentToken == "class") {
        return CLASS;
    } else if (currentToken == "method") {
        return  METHOD;
    } else if (currentToken == "int") {
        return INT;
    } else if (currentToken == "function") {
        return FUNCTION;
    } else if (currentToken == "boolean") {
        return BOOLEAN;
    } else if (currentToken == "constructor") {
        return CONSTRUCTOR;
    } else if (currentToken == "char") {
        return CHAR;
    } else if (currentToken == "void") {
        return VOID;
    } else if (currentToken == "var") {
        return VAR;
    } else if (currentToken == "static") {
        return STATIC;
    } else if (currentToken == "field") {
        return FIELD;
    } else if (currentToken == "let") {
        return LET;
    } else if (currentToken == "do") {
        return DO;
    } else if (currentToken == "if") {
        return IF;
    } else if (currentToken == "else") {
        return ELSE;
    } else if (currentToken == "while") {
        return WHILE;
    } else if (currentToken == "return") {
        return RETURN;
    } else if (currentToken == "true") {
        return TRUE;
    } else if (currentToken == "false") {
        return FALSE;
    } else if (currentToken == "null") {
        return NL;
    } else {
        return THIS;
    }
}

string JackToken::symbol() {
    return currentToken;
}

string JackToken::identifier() {
    return currentToken;
}

int JackToken::intVal() {
    return stoi(currentToken);
}

string JackToken::stringVal() {
    return currentToken.substr(1, currentToken.size()-2);
}




