#include "string"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <iostream>
#include <unordered_map>

int main() {
    std::string input = "++a = b";
    LexicalAnalyzer lexicalAnalyzer(input);
    const auto& tokens = lexicalAnalyzer.getTokens();
    SyntaxAnalyzer syntaxAnalyzer(tokens);
    std::unordered_map<std::string, int> variables = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};
    SemanticAnalyzer semanticAnalyzer(tokens, variables);
    semanticAnalyzer.printAnalyzeResults();

    return 0;
}

    // ac++=c++