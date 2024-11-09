#include "string"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <iostream>
#include <unordered_map>

int main() try {
    std::string input = "a = ++c++";
    LexicalAnalyzer lexicalAnalyzer(input);
    const auto& tokens = lexicalAnalyzer.getTokens();
    SyntaxAnalyzer syntaxAnalyzer(tokens);
    std::unordered_map<std::string, int> variables = {{"a", 5}, {"b", 5}, {"c", 5}};
    SemanticAnalyzer semanticAnalyzer(tokens, variables);
    // semanticAnalyzer.printAnalyzeResults();

    return 0;
}
catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    // ac++=c++