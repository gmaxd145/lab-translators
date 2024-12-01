#include "string"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <iostream>
#include <unordered_map>
#include <cassert>


int main() {
    // std::unordered_map<std::string, int> variables;
    // variables["ab"] = 1;
    // variables["b"] = 2;
    // variables["c"] = 3;
    // variables["d"] = 4;
    // std::cout << "Input variables:" << std::endl;
    // for (const auto& pair : variables) {
    //     std::cout << pair.first << " - " << pair.second << std::endl;
    // }
    // std::string input = "ab=b++";
    std::string input;
    std::cin >> input;
    std::cout << "Input expr: " << input << std::endl;
    LexicalAnalyzer lexicalAnalyzer(input);
    const auto& tokens = lexicalAnalyzer.getTokens();
    SyntaxAnalyzer syntaxAnalyzer(tokens);
    SemanticAnalyzer semanticAnalyzer(tokens);
    std::cout << "Output:" << std::endl;
    semanticAnalyzer.printAnalyzeResults();
    
    return 0;
}