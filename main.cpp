#include "string"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "iostream"

int main() try {
    std::string input = "a = b = ++++c";
    LexicalAnalyzer lexicalAnalyzer(input);
    const auto& tokens = lexicalAnalyzer.getTokens();
    SyntaxAnalyzer syntaxAnalyzer(tokens);
    SemanticAnalyzer semanticAnalyzer(input);
    semanticAnalyzer.analyze();

    return 0;
}
catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }