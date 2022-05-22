//
// Created by robin on 2022/5/16.
//

#ifndef SYNTAXANALYZER_VMWRITER_H
#define SYNTAXANALYZER_VMWRITER_H

#include <string>
#include <set>
#include "fstream"
#include <map>
#include <vector>

using namespace std;

namespace vmwriter
{
    enum Segment { CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP };
    enum Command { ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };

};
namespace vm=vmwriter;

class VMWriter {
public:
    // Constructor
    VMWriter(string filename);

    // Methods
    void writePush(vm::Segment segment, int index);

    void writePop(vm::Segment segment, int index);

    void writeArithmetic(vm::Command command);

    void writeLabel(string label);

    void writeGoto(string label);

    void writeIf(string label);

    void writeCall(string name, int nArgs);

    void writeFunction(string name, int nLocals);

    void writeReturn(void);

    void close(void);

    string segment2String(vm::Segment seg);

    string command2String(vm::Command com);

    ifstream fin;

    string line;

    ofstream fout;
};


#endif //SYNTAXANALYZER_VMWRITER_H
