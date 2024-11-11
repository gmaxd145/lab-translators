#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <regex>

class LexicalAnalyzer {
public:
    explicit LexicalAnalyzer(const std::string& source);
    
    const std::vector<Token>& getTokens() const;

private:
    std::string source;
    size_t currentIndex;
    std::vector<Token> tokens;

    void tokenize();
};