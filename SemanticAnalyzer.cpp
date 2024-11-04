#include "SemanticAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token> &tokens, const std::unordered_map<std::string, int> &variables) : 
_variables(_variables)
{
    
}

void SemanticAnalyzer::toRPN(const std::vector<Token> &tokens)
{
    std::queue<Token> output;
    std::stack<Token> opStack;

    for (const auto& token : tokens) {
        if (isOperand(token))
        {
            output.push(token);
        }
        else
        {
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(token))
            {
                output.push(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }
}

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