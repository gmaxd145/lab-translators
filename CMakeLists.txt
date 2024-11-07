cmake_minimum_required(VERSION 3.5.0)
project(lab-translator VERSION 0.1.0 LANGUAGES C CXX)

add_executable(lab-translator main.cpp LexicalAnalyzer.cpp SyntaxAnalyzer.cpp SemanticAnalyzer.cpp)

