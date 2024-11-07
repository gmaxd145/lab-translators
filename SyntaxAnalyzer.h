#pragma once

// E::= T | T = E | I
// T::= GVG
// G::= ++G | e

#include "Token.h"
#include <vector>

class SyntaxAnalyzer {
public:
    explicit SyntaxAnalyzer(const std::vector<Token>& tokens);

private:
    const std::vector<Token>& tokens;
    size_t currentIndex;
    
    void E();
    void T();
    void G();
    void B();

    const Token& getCurrentToken() const;
    const std::vector<Token>& getTokens() const;    
    void error();

};