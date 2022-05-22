cmake_minimum_required(VERSION 3.22)
project(SyntaxAnalyzer)

set(CMAKE_CXX_STANDARD 14)

add_executable(SyntaxAnalyzer JackCompiler.cpp VMWriter.cpp VMWriter.h SymbolTable.cpp SymbolTable.h JackToken.cpp JackToken.h CompilationEngine.cpp CompilationEngine.h)
