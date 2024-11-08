#include "SemanticAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token> &tokens, const std::unordered_map<std::string, int> &variables) : 
_variables(_variables)
{
    auto expandedTokens= toExpandedTokens(tokens);
}

std::vector<ExpandedToken> SemanticAnalyzer::toExpandedTokens(const std::vector<Token> &tokens)
{
    std::vector<ExpandedToken> exp;
    int sizeWithoutEnd = tokens.size() - 1;
    for (int i = 0; i < sizeWithoutEnd; ++i) {
        if (tokens.at(i).type == TokenType::INT) {
            exp.push_back({ExpandedTokenType::INT, tokens.at(i).value});
        } else if (tokens.at(i).type == TokenType::VAR) {
            exp.push_back({ExpandedTokenType::VAR, tokens.at(i).value});
        } else if (tokens.at(i).type == TokenType::ASSIGN) {
            exp.push_back({ExpandedTokenType::ASSIGN, std::nullopt});
        } else if (tokens.at(i).type == TokenType::INCREMENT) {
            unsigned short n = getIncrementMultiplier(i, tokens);
            if (tokens.at(i - 1).type == TokenType::VAR && i > 0) {
                exp.push_back({ExpandedTokenType::POSTFIX_INCREMENT, std::nullopt, n});
            }
            else {
                exp.push_back({ExpandedTokenType::PREFIX_INCREMENT, std::nullopt, n});
            }
        }
    }
    return exp;
}

unsigned short SemanticAnalyzer::getIncrementMultiplier(int& i, const std::vector<Token> &tokens)
{
    unsigned short n = 1;
    int sizeWithoutEnd = tokens.size() - 1;
    for (; tokens.at(i + 1).type == TokenType::INCREMENT && i < sizeWithoutEnd - 1; ++i) {
        ++n;
    }
    return n;
}

// std::queue<ToExpandedTokenken> SemanticAnalyzer::toRPN(const std::vector<ExpandedToken> &tokens)
// {

//     std::queue<Token> output;
//     std::stack<Token> opStack;

//     for (const auto& token : exp) {
//         if (isOperand(token))
//         {
//             output.push(token);
//         }
//         else
//         {
//             while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(token))
//             {
//                 if ()
//                 output.push(opStack.top());
//                 opStack.pop();
//             }
//             opStack.push(token);
//         }
//     }
//     while (!opStack.empty())
//     {
//         output.push(opStack.top());
//         opStack.pop();
//     }
// }

const SemanticAnalyzer::operatorPrecedence SemanticAnalyzer::getPrecedence(const Token& op) const
{
    if (op.type == TokenType::ASSIGN) return operatorPrecedence::Zero;
    else return operatorPrecedence::One;
}

void SemanticAnalyzer::evaluate(const std::queue<Token> &tokens)
{
    std::stack<Token> stack;
    while(!tokens.empty()) 
    {
        const auto& token = tokens.front();
        if (isOperand(token))
        {
            stack.push(token);
        }
        else
        {
            
        }
        stack.pop();
    }    
}

const bool SemanticAnalyzer::isOperand(const Token& token) const
{
    return token.type == TokenType::INT || token.type == TokenType::VAR;
}

void SemanticAnalyzer::printAnalyzeResults()
{
    for (const auto& pair : _variables) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
}