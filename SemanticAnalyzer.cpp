#include "SemanticAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token> &tokens, const std::unordered_map<std::string, int> &variables) : 
_variables(_variables)
{
    toRPN(toExpandedTokens(tokens));
}

std::vector<ExpandedToken> SemanticAnalyzer::toExpandedTokens(const std::vector<Token> &tokens)
{
    std::vector<ExpandedToken> exp;
    int sizeWithoutEnd = tokens.size() - 1;
    for (int i = 0; i < sizeWithoutEnd; ++i) {
        if (tokens.at(i).type == TokenType::INT) {
            exp.push_back({ExpandedToken::Type::INT, tokens.at(i).value});
        } else if (tokens.at(i).type == TokenType::VAR) {
            exp.push_back({ExpandedToken::Type::VAR, tokens.at(i).value});
        } else if (tokens.at(i).type == TokenType::ASSIGN) {
            exp.push_back({ExpandedToken::Type::ASSIGN, std::nullopt});
        } else if (tokens.at(i).type == TokenType::INCREMENT) {
            unsigned short n = getIncrementMultiplier(i, tokens);
            if (tokens.at(i - 1).type == TokenType::VAR && i > 0) {
                exp.push_back({ExpandedToken::Type::POSTFIX_INCREMENT, std::nullopt, n});
            }
            else {
                exp.push_back({ExpandedToken::Type::PREFIX_INCREMENT, std::nullopt, n});
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

std::queue<ExpandedToken> SemanticAnalyzer::toRPN(const std::vector<ExpandedToken> &expTokens)
{

    std::queue<ExpandedToken> output;
    std::stack<ExpandedToken> opStack;

    for (const auto& expToken : expTokens) {
        std::string temp; 
        if (!output.empty() && output.back().type == ExpandedToken::Type::VAR && expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
            temp = output.back().value.value();
        }
        if (expToken.value.has_value())
        {
            output.push(expToken);
        }
        else
        {
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(expToken))
            {
                output.push(opStack.top());
                opStack.pop();
            }
            if (expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
                    output.push({ExpandedToken::Type::VAR, temp});
                }
            opStack.push(expToken);
        }
    }
    while (!opStack.empty())
    {
        output.push(opStack.top());
        opStack.pop();
    }
    return output;
}

const SemanticAnalyzer::operatorPrecedence SemanticAnalyzer::getPrecedence(const ExpandedToken& op) const
{
    switch (op.type) {
    case ExpandedToken::Type::POSTFIX_INCREMENT:
        return operatorPrecedence::Zero;
        break;
    case ExpandedToken::Type::ASSIGN:
        return operatorPrecedence::One;
        break;
    case ExpandedToken::Type::PREFIX_INCREMENT:
        return operatorPrecedence::Two;
        break; 
    };
}

void SemanticAnalyzer::evaluate(const std::queue<Token> &tokens)
{
    // std::stack<Token> stack;
    // while(!tokens.empty()) 
    // {
    //     const auto& token = tokens.front();
    //     if (isOperand(token))
    //     {
    //         stack.push(token);
    //     }
    //     else
    //     {
            
    //     }
    //     stack.pop();
    // }    
}

// if value presents than operand other way operator
// const bool SemanticAnalyzer::isOperand(const Token& token) const
// {
//     return token.type == TokenType::INT || token.type == TokenType::VAR;
// }

void SemanticAnalyzer::printAnalyzeResults()
{
    for (const auto& pair : _variables) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
}