//
// Created by robin on 2022/5/16.
//

#include "VMWriter.h"
#include <string>
#include <iostream>
#include <regex>

using namespace std;

VMWriter::VMWriter(string filename){
    fin.open(filename + "Token.xml");
    if (!fin.is_open())
        cerr << "Error: Could not open " << filename << ".\n";
    ofstream output(filename + ".vm");
    fout = dynamic_cast<basic_ofstream<char> &&>(output);
    getline(fin, line);
}

void VMWriter::writePush(vm::Segment segment, int index){
    fout << "push " << segment2String(segment) << " " << index << endl;
}

void VMWriter::writePop(vm::Segment segment, int index){
    fout << "pop " << segment2String(segment) << " " << index << endl;
}

void VMWriter::writeArithmetic(vm::Command command){
    fout << command2String(command) << endl;
}

void VMWriter::writeLabel(string label){
    fout << "label " << label << endl;
}

void VMWriter::writeGoto(string label){
    fout << "goto " << label << endl;
}

void VMWriter::writeIf(string label){
    fout << "if-goto " << label << endl;
}

void VMWriter::writeCall(string name, int nArgs){
    fout << "call " << name << " " << nArgs << endl;
}

void VMWriter::writeFunction(string name, int nLocals){
    fout << "function " << name << " " << nLocals << endl;
}

void VMWriter::writeReturn(void){
    fout << "return" << endl;
}

void  VMWriter::close(void){
    if(fin.is_open()) fin.close();
    if(fout.is_open()) fout.close();
}

string VMWriter::segment2String(vm::Segment seg)
{
    switch(seg)
    {
        case vm::CONST:
            return "constant";
        case vm::ARG:
            return "argument";
        case vm::LOCAL:
            return "local";
        case vm::STATIC:
            return "static";
        case vm::THIS:
            return "this";
        case vm::THAT:
            return "that";
        case vm::POINTER:
            return "pointer";
        default:
            return "temp";
    }
}

string VMWriter::command2String(vm::Command com)
{
    switch(com)
    {
        case vm::ADD:
            return "add";
        case vm::SUB:
            return "sub";
        case vm::NEG:
            return "neg";
        case vm::EQ:
            return "eq";
        case vm::GT:
            return "gt";
        case vm::LT:
            return "lt";
        case vm::AND:
            return "and";
        case vm::OR:
            return "or";
        default:
            return "not";
    }
}
