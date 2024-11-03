#include "SyntaxAnalyzer.h"
#include <stdexcept>

const std::vector<Token>& SyntaxAnalyzer::getTokens() const {
    return tokens;
}
void SyntaxAnalyzer::analyze() {
    E();
    if (getCurrentToken().type != TokenType::END) {
        throw std::runtime_error("Unexpected token at end of input.");
    }
}

    // std::string tokenTypeToStr()
    // {
    //     std::string typeInStr;
    //     switch (currentToken().type)
    //     {
    //     case TokenType::ASSIGN:
    //         typeInStr = "assign";
    //         break;
    //     case TokenType::END:
    //         typeInStr = "end";
    //         break;
    //     case TokenType::VAR:
    //         typeInStr = "var";
    //         break;    
    //     case TokenType::INT:
    //         typeInStr = "int";
    //         break;    
    //     case TokenType::INCREMENT:
    //         typeInStr = "increment";
    //         break;    
    //     }
    //     return typeInStr;
    // }

void SyntaxAnalyzer::error()
{   
    std::runtime_error("Syntax error on position " + 
                        std::to_string(currentIndex) + "!");
}

void SyntaxAnalyzer::E() {
    // change if to switch
    if (getCurrentToken().type == TokenType::INT)
    {
        C();
    }
    else if (getCurrentToken().type == TokenType::VAR)
    {
        T();
        if (getCurrentToken().type == TokenType::ASSIGN) {
            E();
        }
    }
    else
    {
        error();
    }
    ++currentIndex;
}

void SyntaxAnalyzer::T() {
    if (getCurrentToken().type == TokenType::VAR)
    {
        // add to ast var
    }
    T_;
    ++currentIndex; 
}

void SyntaxAnalyzer::T_() {
    if (getCurrentToken().type != TokenType::INCREMENT)
    {
        return;
    }
    T_;
    ++currentIndex;
}

void SyntaxAnalyzer::C()
{
    if (getCurrentToken().type == TokenType::INT)
    {
        //add to ast
    }
    else
    {
        G();
    }
    ++currentIndex;
}

void SyntaxAnalyzer::G()
{
    if (getCurrentToken().type == TokenType::VAR)
    {
        // add to ast
    }
    else
    {
        // add to asd ++ op
        G();
    }
    ++currentIndex;
}

const Token& SyntaxAnalyzer::getCurrentToken() const 
{
    return tokens[currentIndex];
}