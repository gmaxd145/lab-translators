#include "SyntaxAnalyzer.h"
#include <stdexcept>
#include <iostream>

const std::vector<Token>& SyntaxAnalyzer::getTokens() const {
    return tokens;
}
SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0)
{
    E();
    if (getCurrentToken().type != Token::Type::END) {
        error("Excepted end");
    }
}

void SyntaxAnalyzer::error(const std::string& msg)
{   std::string expr;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        if (it - tokens.begin() == currentIndex) expr.append("(");
        if ((*it).type == Token::Type::INCREMENT) expr.append("++");
        else if ((*it).type == Token::Type::ASSIGN) expr.append("=");
        else if ((*it).type == Token::Type::VAR || (*it).type == Token::Type::INT) expr.append((*it).value);
        if (it - tokens.begin() == currentIndex) expr.append(")");
    }
    throw std::runtime_error("Syntax error: " + msg + " " + expr);
}

void SyntaxAnalyzer::E() {
    // G();
    // if (getCurrentToken().type == Token::Type::VAR) {
    //     ++currentIndex;
    //     if (getCurrentToken().type == Token::Type::ASSIGN) {
    //         ++currentIndex;
    //         F();
    //     }
    // }
    // else if (getCurrentToken().type == Token::Type::INCREMENT)
    // {
    //     L();
    // }
    // else error("Excepted var/increment");
    if (getCurrentToken().type == Token::Type::VAR) {
        ++currentIndex;
        if (getCurrentToken().type == Token::Type::ASSIGN) {
            ++currentIndex;
            F();
        }
        else if (getCurrentToken().type == Token::Type::INCREMENT) {
            L();
        }
        else error("Excepted assign/increment");
    }
    else if (getCurrentToken().type == Token::Type::INCREMENT) {
        G();
        if (getCurrentToken().type == Token::Type::VAR) {
            ++currentIndex;
            if (getCurrentToken().type == Token::Type::ASSIGN) {
                ++currentIndex;
                F();
            }
        }
        else error("Excepted var");
    }
    else error("Excepted var/increment");
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
                } else if (getCurrentToken().type != Token::Type::END) error("Excepted end/assign.");
            }
            else error("Excepted var");
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
        default:
            error("Excepted int/var/increment");
    }
}

void SyntaxAnalyzer::G() {
    while (getCurrentToken().type  == Token::Type::INCREMENT) {
        ++currentIndex;
    }
}

void SyntaxAnalyzer::L()
{
    // if (getCurrentToken().type != Token::Type::INCREMENT) error("Excepted increment");
    G();
}

const Token& SyntaxAnalyzer::getCurrentToken() const 
{
    return tokens[currentIndex];
}