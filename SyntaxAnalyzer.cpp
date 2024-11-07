#include "SyntaxAnalyzer.h"
#include <stdexcept>

const std::vector<Token>& SyntaxAnalyzer::getTokens() const {
    return tokens;
}
SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0)
{
    E();
    if (getCurrentToken().type != TokenType::END) {
        throw std::runtime_error("Unexpected token at end of input.");
    }
}

void SyntaxAnalyzer::error()
{   
    std::runtime_error("Syntax error on position " + 
                        std::to_string(currentIndex) + "!");
}

void SyntaxAnalyzer::E() {
    if (getCurrentToken().type != TokenType::INT) 
    {
        T();
        if (getCurrentToken().type == TokenType::ASSIGN) 
        {
            ++currentIndex;
            E();
        }
    }
}

void SyntaxAnalyzer::T() 
{
    B();
    if (getCurrentToken().type == TokenType::VAR)
    {
        ++currentIndex;
        B();
    }
}

void SyntaxAnalyzer::B() {
    while (getCurrentToken().type  == TokenType::INCREMENT)
    {
        ++currentIndex;
    }
}

const Token& SyntaxAnalyzer::getCurrentToken() const 
{
    return tokens[currentIndex];
}