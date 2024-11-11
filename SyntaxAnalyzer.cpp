#include "SyntaxAnalyzer.h"
#include <stdexcept>

const std::vector<Token>& SyntaxAnalyzer::getTokens() const {
    return tokens;
}
SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0)
{
    E();
    if (getCurrentToken().type != Token::Type::END) {
        throw std::runtime_error("Syntax error.");
    }
}

void SyntaxAnalyzer::error()
{   
    std::runtime_error("Syntax error on position " + 
                        std::to_string(currentIndex) + "!");
}

void SyntaxAnalyzer::E() {
    G();
    if (getCurrentToken().type == Token::Type::VAR) {
        ++currentIndex;
        if (getCurrentToken().type == Token::Type::ASSIGN) {
            ++currentIndex;
            F();
        }
    }
    if (getCurrentToken().type == Token::Type::INCREMENT)
    {
        L();
    }
}

void SyntaxAnalyzer::F() 
{
    switch(getCurrentToken().type) {
        case Token::Type::INCREMENT:
            G();
            if (getCurrentToken().type == Token::Type::VAR) {
                ++currentIndex;
                if (getCurrentToken().type == Token::Type::ASSIGN) {
                    ++currentIndex;
                    F();
                }
            }
            break;
        case Token::Type::VAR:
            ++currentIndex;
            if (getCurrentToken().type == Token::Type::ASSIGN) {
                ++currentIndex;
                F();
            } else {
                while (getCurrentToken().type  == Token::Type::INCREMENT) {
                    ++currentIndex;
                }
            }
            break;
        case Token::Type::INT:
            ++currentIndex;
            break;
    }
}

void SyntaxAnalyzer::G() {
    while (getCurrentToken().type  == Token::Type::INCREMENT) {
        ++currentIndex;
    }
}

void SyntaxAnalyzer::L()
{
    G();
}

const Token& SyntaxAnalyzer::getCurrentToken() const 
{
    return tokens[currentIndex];
}