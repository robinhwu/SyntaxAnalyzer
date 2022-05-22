//
// Created by robin on 2022/5/14.
//

#include "SymbolTable.h"
#include <string>
#include <iostream>

SymbolTable::SymbolTable() {
    classTable.clear();
    functionTable.clear();
    static_count = 0;
    field_count = 0;
    arg_count = 0;
    var_count = 0;
}

void SymbolTable::startSubroutine() {
    functionTable.clear();
    arg_count = 0;
    var_count = 0;
}

void SymbolTable::Define(string name, string type, symboltable::Kind kind){
    switch(kind) {
        case symboltable::STATIC:
            classTable[name].type = type;
            classTable[name].kind = kind;
            classTable[name].index = static_count;
            static_count++;
            break;
        case symboltable::FIELD:
            classTable[name].type = type;
            classTable[name].kind = kind;
            classTable[name].index = field_count;
            field_count++;
            break;
        case symboltable::VAR:
            classTable[name].type = type;
            classTable[name].kind = kind;
            classTable[name].index = var_count;
            var_count++;
            break;
        case symboltable::ARG:
            classTable[name].type = type;
            classTable[name].kind = kind;
            classTable[name].index = arg_count;
            arg_count++;
            break;
    }
}

int SymbolTable::VarCount(symboltable::Kind kind){
    switch(kind)
    {
        case symboltable::STATIC:
            return static_count;
        case symboltable::FIELD:
            return field_count;
        case symboltable::ARG:
            return arg_count;
        case symboltable::VAR:
            return var_count;
        default:
            return -1;
    }
}

symboltable::Kind SymbolTable::KindOf(string name){
    string TokenString;
    if (classTable.count(name)) {
        return classTable[name].kind;
    } else if (functionTable.count(name)){
        return functionTable[name].kind;
    } else {
//        cerr << "wrong KindOf\n";
        return symboltable::NONE;
    }
}

string SymbolTable::TypeOf(string name){
    if (classTable.count(name)) {
        return classTable[name].type;
    } else if (functionTable.count(name)) {
        return functionTable[name].type;
    } else {
//        cerr << "wrong TypeOf\n";
        return "";
    }
}

int SymbolTable::IndexOf(string name){
    if (classTable.count(name)) {
        return classTable[name].index;
    } else if (functionTable.count(name)) {
        return functionTable[name].index;
    } else {
//        cerr << "wrong IndexOf\n";
        return -1;
    }
}