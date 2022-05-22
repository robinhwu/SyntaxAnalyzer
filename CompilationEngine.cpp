//
// Created by robin on 2022/4/30.
//

#include "CompilationEngine.h"
#include <string>
#include <iostream>
#include <regex>
#include <set>

using namespace std;

set<string> nonTerminal = {"class", "classVarDec", "subroutineDec", "parameterList", "subroutineBody", "varDec", "statements",
                           "LetStatement", "ifStatement", "whileStatement", "doStatement", "returnStatement", "expression",
                           "term", "expressionList"};

set<string> terminalElement = {"keyword", "symbol", "integerConstant", "stringConstant", "identifier"};

CompilationEngine::CompilationEngine(string filename,VMWriter * VM, SymbolTable * ST) {
    fin.open(filename + "Token.xml");
    if (!fin.is_open())
        cerr << "Error: Could not open " << filename << ".\n";
    ofstream output(filename + "Compilation.xml");
    fout = dynamic_cast<basic_ofstream<char> &&>(output);
    getline(fin, line);
    VM_ = VM;
    ST_ = ST;
    numArgs_=0;
    labelNum_=0;
    compileClass();
}

CompilationEngine::~CompilationEngine() {
    if(fin.is_open()) {
        fin.close();
    }
    if (fout.is_open()) {
        fout.close();
    }
}

void CompilationEngine::advance() {
    getline(fin, line);
    keyWordType = line.substr(line.find_first_of("<") + 1, line.find_first_of(">") - line.find_first_of("<") - 1);
    int front = line.find_first_of(" "), back = line.find_first_of(" ", front+1);
    keyWord = line.substr(front+1, back-front-1);
}

void CompilationEngine::compileClass() {
    fout << "<class>" << endl;
    advance();

    fout << line << endl;
    advance();
    className = keyWord;
    fout << line << endl;
    advance();
    fout << line << endl;
    advance();
    //classVarDec* subroutineDec*

    while (keyWord == "static" || keyWord == "field" || keyWord == "constructor" || keyWord == "function"
           || keyWord == "method" || keyWord == "void") {
        if (keyWord == "static" || keyWord == "field") {
            compileClassVarDec();
        } else if (keyWord == "constructor" || keyWord == "function" || keyWord == "method" ) {
            compileSubroutine();
        } else {

        }
    }


    fout << line << endl;
    fout << "</class>" << endl;
}

void CompilationEngine::compileClassVarDec() {
    string name, type;
    symboltable::Kind kind;
    fout << "<classVarDec>" << endl;
//    for (int i = 0; i < 3; i++) {
//        fout << line << endl;
//        advance();
//    }
    if (keyWord == "static") {
        kind = symboltable::STATIC;
    } else {
        kind = symboltable::FIELD;
    }
    fout << line << endl;
    advance();
    type = keyWord;
    fout << line << endl;
    advance();
    name = keyWord;
    ST_->Define(name, type, kind);
    fout << line << endl;
    advance();

    while (keyWord == ",") {
        fout << line << endl;
        advance();
        name = keyWord;
        ST_->Define(name, type, kind);

        fout << line << endl;
        advance();
    }

    fout << line << endl;
    advance();

    fout << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine() {
    fout << "<subroutineDec>" << endl;

    ST_->startSubroutine();
    subroutineType = keyWord;
    if (subroutineType == "method") {
        ST_->Define("this", className, symboltable::ARG);
    }

    fout << line << endl;
    advance();

    string  returnType = keyWord;
    fout << line << endl;
    advance();

    functionName = className + "." + keyWord;
    fout << line << endl;
    advance();

    fout << line << endl;
    advance();

    compileParameterList();

    fout << line << endl;
    advance();

    compileSubroutineBody();

    fout << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList() {
    fout << "<parameterList>" << endl;
    string name, type;
    symboltable::Kind kind = symboltable::ARG;

    if (keyWord == "int" || keyWord == "char" || keyWord == "boolean" || keyWordType == "identifier") {
        type = keyWord;
        fout << line << endl;
        advance();
        name = keyWord;
        ST_->Define(name,type,kind);
        fout << line << endl;
        advance();
    }
    while (keyWord == ",") {
        fout << line << endl;
        advance();
        type = keyWord;
        fout << line << endl;
        advance();
        name = keyWord;
        ST_->Define(name,type,kind);
        fout << line << endl;
        advance();
    }
    fout << "</parameterList>" << endl;
}

void CompilationEngine::compileSubroutineBody() {
    fout << "<subroutineBody>" << endl;
    fout << line << endl;
    advance();

    while (keyWord == "var") {
        compileVarDec();
    }

    int nLocals = ST_->VarCount(symboltable::VAR);
    VM_->writeFunction(functionName, nLocals);

    if (subroutineType == "constructor") {
        VM_->writePush(vmwriter::CONST, ST_->VarCount(symboltable::FIELD));
        VM_->writeCall("Memory.alloc", 1);
        VM_->writePop(vm::POINTER,0);
    } else if (subroutineType == "method") {
        VM_->writePush(vmwriter::ARG, 0);
        VM_->writePop(vmwriter::POINTER, 0);
    }

    if (keyWord == "let" || keyWord == "if" || keyWord == "else" || keyWord == "do" || keyWord == "else" || keyWord == "return" || keyWord == "while") {
        compileStatements();
    }

    fout << line << endl;
    advance();

    fout << "</subroutineBody>" << endl;
}

void CompilationEngine::compileVarDec() {
    fout << "<varDec>" << endl;

    string name, type;
    symboltable::Kind kind = symboltable::VAR;

    fout << line << endl;
    advance();

    type = keyWord;
    fout << line << endl;
    advance();

    name = keyWord;
    ST_->Define(name,type,kind);
    fout << line << endl;
    advance();

    while (keyWord == ",") {
        fout << line << endl;
        advance();

        name = keyWord;
        ST_->Define(name,type,kind);
        fout << line << endl;
        advance();
    }

    fout << line << endl;
    advance();

    fout << "</varDec>" << endl;
}

void CompilationEngine::compileStatements() {
    fout << "<statements>" << endl;

    while (keyWord == "let" || keyWord == "if" || keyWord == "while" || keyWord == "do"  || keyWord == "return") {
        if (keyWord == "let") {
            compileLet();
        } else if (keyWord == "if") {
            compileIf();
        } else if (keyWord == "while") {
            compileWhile();
        } else if (keyWord == "do") {
            compileDo();
        } else {
            compileReturn();
        }
    }

    fout << "</statements>" << endl;
}

void CompilationEngine::compileIf() {
    fout << "<ifStatement>" << endl;

    stringstream labelElse, labelContinue;
    labelElse << "IFFALSE" << labelNum_;
    labelContinue << "CONTINUE" << labelNum_;
    labelNum_++;

    fout << line << endl;
    advance();

    fout << line << endl;
    advance();

    compileExpression();

    VM_->writeArithmetic(vmwriter::NOT);
    VM_->writeIf(labelElse.str());

    fout << line << endl;
    advance();

    fout << line << endl;
    advance();

    compileStatements();

    VM_->writeGoto(labelContinue.str());

    fout << line << endl;
    advance();

    VM_->writeLabel(labelElse.str());

    if (keyWord == "else") {

        fout << line << endl;
        advance();

        fout << line << endl;
        advance();

        compileStatements();

        fout << line << endl;
        advance();
    }

    VM_->writeLabel(labelContinue.str());

    fout << "</ifStatement>" << endl;
}

void CompilationEngine::compileLet() {
    fout << "<letStatement>" << endl;

    bool isArray = false;
    fout << line << endl;
    advance();

    string LHSVar = keyWord;
    fout << line << endl;
    advance();


    if (keyWord == "[") {
        isArray = true;

        fout << line << endl;
        advance();

        compileExpression();

        VM_->writePush(kind2Segment(ST_->KindOf(LHSVar)),ST_->IndexOf(LHSVar));
        VM_->writeArithmetic(vmwriter::ADD);

        fout << line << endl;
        advance();
    }

    fout << line << endl;
    advance();

    compileExpression();

    if (isArray) {
        VM_->writePop(vmwriter::TEMP, 0);      // store result in temp 0
        VM_->writePop(vmwriter::POINTER, 1);   // pop address pointer into pointer 1
        VM_->writePush(vmwriter::TEMP, 0);     // push result back onto stack
        VM_->writePop(vm::THAT, 0);            // Store right hand side evaluation in THAT 0.
    } else {
        VM_->writePop(kind2Segment(ST_->KindOf(LHSVar)), ST_->IndexOf(LHSVar));
    }


    fout << line << endl;
    advance();

    fout << "</letStatement>" << endl;
}

void CompilationEngine::compileWhile() {
    fout << "<whileStatement>" << endl;

    stringstream labelTrue, labelFalse;
    labelTrue << "WHILETRUE" << labelNum_;
    labelFalse << "WHILEFALSE" << labelNum_;
    labelNum_++;

    fout << line << endl;
    advance();

    fout << line << endl;
    advance();

    VM_->writeLabel(labelTrue.str());       // while true label to execute statements again

    compileExpression();

    VM_->writeArithmetic(vm::NOT);
    VM_->writeIf(labelFalse.str());

    fout << line << endl;
    advance();

    fout << line << endl;
    advance();

    compileStatements();

    VM_->writeGoto(labelTrue.str());
    VM_->writeLabel(labelFalse.str());

    fout << line << endl;
    advance();

    fout << "</whileStatement>" << endl;
}

void CompilationEngine::compileDo() {
    fout << "<doStatement>" << endl;

    fout << line << endl;
    advance();

    string name = keyWord;
    fout << line << endl;
    advance();

    if (keyWord == "(") {
        fout << line << endl;
        advance();

        VM_->writePush(vm::POINTER,0);

        compileExpressionList();

        fout << line << endl;
        advance();

        VM_->writeCall(className + "." + name,numArgs_+1);

        numArgs_ = 0;

    } else if (keyWord == ".") {

        fout << line << endl;
        advance();

        string subroutineName = keyWord;
        fout << line << endl;
        advance();

        if(ST_->KindOf(name)!=symboltable::NONE) {
            VM_->writePush(kind2Segment(ST_->KindOf(name)),ST_->IndexOf(name));

            fout << line << endl;
            advance();

            compileExpressionList();

            fout << line << endl;
            advance();

            VM_->writeCall(ST_->TypeOf(name)+"."+subroutineName,numArgs_+1);
        } else {
            fout << line << endl;
            advance();

            compileExpressionList();

            fout << line << endl;
            advance();

            VM_->writeCall(name+"."+subroutineName,numArgs_);
        }
        numArgs_ = 0;
    }

    fout << line << endl;
    advance();

    VM_->writePop(vm::TEMP,0);

    fout << "</doStatement>" << endl;
}

void CompilationEngine::compileReturn() {
    fout << "<returnStatement>" << endl;

    fout << line << endl;
    advance();

    if (keyWord != ";") {

        compileExpression();

        VM_->writeReturn();
        fout << line << endl;
        advance();
    } else {
        VM_->writePush(vm::CONST,0);
        VM_->writeReturn();
        fout << line << endl;
        advance();
    }

    fout << "</returnStatement>" << endl;
}

void CompilationEngine::compileExpression() {
    fout << "<expression>" << endl;

    string op;

    compileTerm();

    if (keyWord == "+" || keyWord == "-" || keyWord == "&lt;" || keyWord == "&gt;" || keyWord == "*" || keyWord == "/"
        || keyWord == "&amp;" || keyWord == "|" || keyWord == "=") {

        fout << line << endl;
        op = keyWord;
        advance();

        compileTerm();

        if (op == "+") {
            VM_->writeArithmetic(vm::ADD);
        } else if (op == "-") {
            VM_->writeArithmetic(vm::SUB);
        } else if (op == "&lt;") {
            VM_->writeArithmetic(vm::LT);
        } else if (op == "&gt;") {
            VM_->writeArithmetic(vm::GT);
        } else if (op == "*") {
            VM_->writeCall("Math.multiply",2);
        } else if (op == "/"){
            VM_->writeCall("Math.divide",2);
        } else if (op == "&amp;") {
            VM_->writeArithmetic(vm::AND);
        } else if (op == "|") {
            VM_->writeArithmetic(vm::OR);
        } else if (op == "=") {
            VM_->writeArithmetic(vm::EQ);
        }
    }

    fout << "</expression>" << endl;
}

void CompilationEngine::compileTerm() {
    fout << "<term>" << endl;

    string id, str_const;

    if (keyWordType == "integerConstant" || keyWordType == "stringConstant") {

        if (keyWordType == "integerConstant") {
            int currentInt = stoi(keyWord);
            VM_->writePush(vm::CONST, currentInt);
        } else {
            str_const = keyWord;
            VM_->writePush(vm::CONST,str_const.length());
            VM_->writeCall("String.new",1);
            for(int i=0;i<str_const.length();i++)
            {
                VM_->writePush(vm::CONST,(int)str_const[i]);
                VM_->writeCall("String.appendChar",2);
            }
        }

        fout << line << endl;
        advance();
    } else if (keyWordType == "keyword" && (keyWord == "true" || keyWord == "false" || keyWord == "null" || keyWord == "this")) {
        if (keyWord == "true") {
            VM_->writePush(vm::CONST,0);
            VM_->writeArithmetic(vm::NOT);
        } else if (keyWord == "this") {
            VM_->writePush(vm::POINTER,0);
        } else {
            VM_->writePush(vm::CONST,0);
        }

        fout << line << endl;
        advance();
    } else if (keyWord == "-" || keyWord == "~") {
        string op = keyWord;

        fout << line << endl;
        advance();

        if (keyWord == "(") {
            fout << line << endl;
            advance();

            compileExpression();

            fout << line << endl;
            advance();

        } else {

            compileTerm();

        }

        if (op == "-") {
            VM_->writeArithmetic(vm::NEG);
        } else if (op == "~"){
            VM_->writeArithmetic(vm::NOT);
        }

    } else if (keyWord == "(") {
        fout << line << endl;
        advance();

        compileExpression();

        fout << line << endl;
        advance();
    } else if (keyWordType == "identifier") {
        id = keyWord;
        fout << line << endl;
        advance();

        if (keyWord == "[") {
            fout << line << endl;
            advance();

            compileExpression();

            VM_->writePush(kind2Segment(ST_->KindOf(id)),ST_->IndexOf(id));
            VM_->writeArithmetic(vm::ADD);

            VM_->writePop(vm::POINTER,1);
            VM_->writePush(vm::THAT,0);

            fout << line << endl;
            advance();
        } else if (keyWord == "(") {
            fout << line << endl;
            advance();

            VM_->writePush(vm::POINTER,0);

            compileExpressionList();

            if (numArgs_ > 0) {
                VM_->writeCall(className+"."+id, numArgs_+1);
            } else {
                VM_->writeCall(className+"."+id, 1);
            }

            numArgs_ = 0;

            fout << line << endl;
            advance();
        } else if (keyWord == ".") {

            fout << line << endl;
            advance();

            string subroutineName = keyWord;

            fout << line << endl;
            advance();

            fout << line << endl;
            advance();

            compileExpressionList();

            if(ST_->KindOf(id)!=symboltable::NONE) {
                VM_->writePush(kind2Segment(ST_->KindOf(id)),ST_->IndexOf(id));
                VM_->writeCall(ST_->TypeOf(id)+"."+subroutineName,numArgs_+1);
            } else {
                VM_->writeCall(id+"."+subroutineName,numArgs_);
            }

            numArgs_ = 0;

            fout << line << endl;
            advance();
        } else {
            VM_->writePush(kind2Segment(ST_->KindOf(id)), ST_->IndexOf(id));
        }
    }

    fout << "</term>" << endl;
}

void CompilationEngine::compileExpressionList() {
    fout << "<expressionList>" << endl;

    while(keyWord != ")")
    {
        compileExpression();

        numArgs_++;

        if(keyWord==",")
        {
            fout << line << endl;
            advance();
        }
    }

    fout << "</expressionList>" << endl;
}

vm::Segment CompilationEngine::kind2Segment(symboltable::Kind kind) {
    switch (kind) {
        case symboltable::STATIC:
            return vm::STATIC;
        case symboltable::FIELD:
            return vm::THIS;
        case symboltable::ARG:
            return vm::ARG;
        default:
            return vm::LOCAL;
    }
}
