#include "string"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <iostream>
#include <unordered_map>
#include <cassert>

int main() {
    std::unordered_map<std::string, int> variables;
    int a = variables["a"] = 1;
    int b = variables["b"] = 2;
    int c = variables["c"] = 3;
    int d = variables["d"] = 4;
    a=++b=c=d++;
    std::cout << "c++: a=" << a << " b=" << b << " c=" << c << " d=" << d << std::endl;
    std::string input = "a=++b=c=d++";
    LexicalAnalyzer lexicalAnalyzer(input);
    const auto& tokens = lexicalAnalyzer.getTokens();
    SyntaxAnalyzer syntaxAnalyzer(tokens);
    SemanticAnalyzer semanticAnalyzer(tokens, variables);
    std::cout << "Translator:" << std::endl;
    semanticAnalyzer.printAnalyzeResults();
    
    return 0;
}

    // ac++=c++