#include "SemanticAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <set>

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token> &tokens, const std::unordered_map<std::string, int> &variables)
{
    _variables = variables;
    if (hasDuplicateKeys(_variables)) throw std::invalid_argument("Redefinition of variables!");
    evaluate(toRPN(toExpandedTokens(tokens)));
}

bool SemanticAnalyzer::hasDuplicateKeys(const std::unordered_map<std::string, int>& myMap) {
    std::set<std::string> uniqueKeys;
    for (const auto& [key, value] : myMap) {
        if (!uniqueKeys.insert(key).second) {
            return true;
        }
    }
    return false;
}

std::vector<ExpandedToken> SemanticAnalyzer::toExpandedTokens(const std::vector<Token> &tokens)
{
    std::vector<ExpandedToken> exp;
    int sizeWithoutEnd = tokens.size() - 1;
    for (int i = 0; i < sizeWithoutEnd; ++i) {
        if (tokens.at(i).type == Token::Type::INT) {
            exp.push_back({ExpandedToken::Type::INT, tokens.at(i).value});
        } else if (tokens.at(i).type == Token::Type::VAR) {
            exp.push_back({ExpandedToken::Type::VAR, tokens.at(i).value});
        } else if (tokens.at(i).type == Token::Type::ASSIGN) {
            exp.push_back({ExpandedToken::Type::ASSIGN, std::nullopt});
        } else if (tokens.at(i).type == Token::Type::INCREMENT) {
            unsigned short n = getIncrementMultiplier(i, tokens);
            if (tokens.at(i - 1).type == Token::Type::VAR && i > 0) {
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
    for (; tokens.at(i + 1).type == Token::Type::INCREMENT && i < sizeWithoutEnd - 1; ++i) {
        ++n;
    }
    return n;
}

std::queue<ExpandedToken> SemanticAnalyzer::toRPN(const std::vector<ExpandedToken> &expTokens)
{

    std::queue<ExpandedToken> output;
    std::stack<ExpandedToken> opStack;
    for (const auto& expToken : expTokens) {
        // stores vars for postfix increment a = b++ -> ab=b++ (second b in exapmle stores in postfixQueue) 
        std::queue<std::string> postfixQueue; 
        if (!output.empty() && output.back().type == ExpandedToken::Type::VAR && 
            expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
            postfixQueue.push(output.back().value.value()); 
        }
        // only operands has values
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
                    output.push({ExpandedToken::Type::VAR, postfixQueue.front()});
                    postfixQueue.pop();
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

void SemanticAnalyzer::evaluate(std::queue<ExpandedToken>   expTokens)
{
    std::stack<ExpandedToken> stack;
    while(!expTokens.empty()) 
    {
        const auto& expToken = expTokens.front();
        if (expToken.value.has_value()) {
            stack.push(expToken);
        } else {
            switch (expToken.type) {
            case ExpandedToken::Type::POSTFIX_INCREMENT:
                _variables.at(stack.top().value.value()) += expToken.multiplier;
                stack.pop();
                break;
            case ExpandedToken::Type::PREFIX_INCREMENT:
                _variables.at(stack.top().value.value()) += expToken.multiplier;
                break;
            case ExpandedToken::Type::ASSIGN:
                auto t1 = stack.top();
                //stack.pop();
                auto t2 = stack.top();
                stack.pop();
                _variables.at(t2.value.value()) = _variables.at(t1.value.value());
                break; 
            };
        }
        expTokens.pop();
    }    
}

void SemanticAnalyzer::printAnalyzeResults()
{
    for (const auto& pair : _variables) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
}