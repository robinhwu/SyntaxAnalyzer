//
// Created by robin on 2022/5/14.
//

#ifndef SYNTAXANALYZER_SYMBOLTABLE_H
#define SYNTAXANALYZER_SYMBOLTABLE_H

#include <string>
#include <set>
#include "fstream"
#include <map>
#include <vector>

using namespace std;

namespace symboltable
{
    enum Kind { STATIC, FIELD, ARG, VAR, NONE };

    struct ST
    {
        string type;
        Kind kind;
        int index;
    };
}

class SymbolTable {
public:
    SymbolTable();

    void startSubroutine();

    void Define(string name, string type, symboltable::Kind kind);

    int VarCount(symboltable::Kind kind);

    symboltable::Kind KindOf(string name);

    string TypeOf(string name);

    int IndexOf(string name);

    map<string, symboltable::ST > classTable;

    map<string, symboltable::ST > functionTable;

    int static_count;

    int field_count;

    int arg_count;

    int var_count;
};


#endif //SYNTAXANALYZER_SYMBOLTABLE_H

