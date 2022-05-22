//
// Created by robin on 2022/4/30.
//

#ifndef SYNTAXANALYZER_COMPILATIONENGINE_H
#define SYNTAXANALYZER_COMPILATIONENGINE_H

#include <string>
#include "fstream"
#include "SymbolTable.h"
#include "VMWriter.h"

using namespace std;

class CompilationEngine {
public:
    CompilationEngine(string filename, VMWriter * VM, SymbolTable * ST);

    ~CompilationEngine();

    void advance();

    void compileClass();

    void compileClassVarDec();

    void compileSubroutine();

    void compileSubroutineBody();

    void compileParameterList();

    void compileVarDec();

    void compileStatements();

    void compileDo();

    void compileLet();

    void compileWhile();

    void compileReturn();

    void compileIf();

    void compileExpression();

    void compileTerm();

    void compileExpressionList();

    vm::Segment kind2Segment(symboltable::Kind kind);

    ifstream fin;

    string line, keyWordType, keyWord, className, functionName, subroutineType;

    ofstream fout;

    SymbolTable * ST_;

    VMWriter * VM_;

    int numArgs_;

    int labelNum_;
};


#endif //SYNTAXANALYZER_COMPILATIONENGINE_H

