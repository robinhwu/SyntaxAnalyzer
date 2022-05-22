#include <iostream>
#include <dirent.h>
#include <vector>
#include "JackToken.h"
#include "CompilationEngine.h"

using namespace std;

void preProcess(string filename) {
    vector<int> vct = {};
    ifstream fin;
    ofstream fout(filename + ".txt");
    bool isRemark = false;

    string line;
    fin.open(filename + ".jack");
    if (!fin.is_open())
        cerr << "Error: Could not open " << filename << ".\n";
    while(!fin.eof()) {
        getline(fin, line);
        if (!isRemark) {
            if (line.find("/**") != string::npos) {
                if (line.find("*/") != string::npos) {
                    line = "";
                } else {
                    line = "";
                    isRemark = true;
                }
            } else if (line.find("//") != string::npos) {
                line = line.substr(0, line.find("//"));
            }
//            if (line.find(";") != string::npos) {
//                line = line.substr(0, line.find(";")) + " " + line.substr(line.find(";"));
//            }
            if (!line.empty() && line[0] != '/') {
                fout << line << endl;
            }
        } else {
            if (line.find("*/") != string::npos) {
                line = "";
                isRemark = false;
            }
        }
    }
    fout.close();
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        struct dirent *d;
        DIR *dr;
        dr = opendir(".");
        if(dr!=NULL)
        {
            string fileName = argv[1];
            fileName = fileName.substr(fileName.find_last_of("/\\") + 1);
            for(d=readdir(dr); d!=NULL; d=readdir(dr))
            {
                string currentFile = string(d->d_name);
                if (currentFile.size() >= 5 && currentFile.substr(currentFile.size()-5) == ".jack") {
                    currentFile = currentFile.substr(0,currentFile.size()-5);
                    preProcess(currentFile);
                    JackToken jackAnalyzer(currentFile);
                    while (jackAnalyzer.hasMoreTokens()) {
                        jackAnalyzer.advance();
//                        jackAnalyzer.advance();
                    }
                    VMWriter VM(currentFile);
                    SymbolTable ST;
                    CompilationEngine CE(currentFile,&VM,&ST);
                }
            }
            closedir(dr);
        }
        else
            cout<<"\nError Occurred!";
        cout<<endl;
    } else {
        string currentFile = argv[1];
        currentFile = currentFile.substr(0,currentFile.size()-5);
        preProcess(currentFile);
        JackToken jackAnalyzer(currentFile);
        while (jackAnalyzer.hasMoreTokens()) {
            jackAnalyzer.advance();
        }
        VMWriter VM(currentFile);
        SymbolTable ST;
        CompilationEngine CE(currentFile,&VM,&ST);
    }
    return 0;
}

