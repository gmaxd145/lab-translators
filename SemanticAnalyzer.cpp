#include "SemanticAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>
// a = ++b = c++ -> c++ = b++ = a

constexpr int maxExpLen = 5;
// a = ++b = c++;

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Token> &tokens, const std::unordered_map<std::string, int>& variables)
{
    _variables = variables;
    if (hasDuplicateKeys(_variables)) throw std::invalid_argument("Redefinition of variables!");
    auto expTokens = toExpandedTokens(tokens);
    toRPN(expTokens);
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

// TODO: refactor to iterators
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
            if (i > 0 && tokens.at(i - 1).type == Token::Type::VAR) {
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

// TODO: make this lambda function
ExpandedToken SemanticAnalyzer::getFirstVarFromLastExpr(const std::vector<std::vector<ExpandedToken>>& exprs) {
    for (auto tk : exprs.back()) {
        if (tk.type == ExpandedToken::Type::VAR) return tk;
    }
    throw std::runtime_error("getFirstVarFromLastExpr");
}



// a=++b=1;
// 1 step
// ++b = 1
// a = b
// 2 step
// b++1=
// ab=
std::queue<ExpandedToken> SemanticAnalyzer::toRPN(const std::vector<ExpandedToken> &expTokens)
{
    // 1 step: parsing to assignment expressions
    std::vector<std::vector<ExpandedToken>> exprs;
    std::vector<ExpandedToken> expr;
    int assigmentIndex{};
    auto exprEnd = expTokens.rbegin();
    for (auto it = expTokens.rbegin(); it != expTokens.rend(); ++it) {
        // if ((*it).value.has_value()) std::cout << (*it).value.value() << std::endl;
        if ((*it).type == ExpandedToken::Type::ASSIGN) ++assigmentIndex;
        // int index = std::distance(expTokens.rbegin(), it);
        std::cout << "index: " << std::distance(expTokens.rbegin(), it) << ", value: " << (*it).value.value_or("") << ", assigmentIndex: " << assigmentIndex << std::endl;
        if (assigmentIndex == 2 || it == --expTokens.rend()) {
            if (it == --expTokens.rend()) std::copy(exprEnd, it + 1, std::back_inserter(expr));
            else std::copy(exprEnd, it, std::back_inserter(expr));
            std::reverse(expr.begin(), expr.end());
            assigmentIndex = 1;
            exprEnd = it;
            if (!exprs.empty()) expr.push_back(getFirstVarFromLastExpr(exprs));
            exprs.push_back(expr);
            expr.clear();
        }
    }
    int a = 1;
    // 2 step
    // std::queue<ExpandedToken> output;
    // std::stack<ExpandedToken> opStack;
    // for (const auto& expToken : expTokens) {
    //     // stores vars for postfix increment a = b++ -> ab=b++ (second b in exapmle stores in postfixQueue) 
    //     std::queue<std::string> postfixQueue; 
    //     if (!output.empty() && output.back().type == ExpandedToken::Type::VAR && 
    //         expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
    //         postfixQueue.push(output.back().value.value()); 
    //     }
    //     // only operands has values
    //     if (expToken.value.has_value())
    //     {
    //         output.push(expToken);
    //     }
    //     else
    //     {
    //         while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(expToken))
    //         {
    //             output.push(opStack.top());
    //             opStack.pop();
    //         }
    //         if (expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
    //                 output.push({ExpandedToken::Type::VAR, postfixQueue.front()});
    //                 postfixQueue.pop();
    //             }
    //         opStack.push(expToken);
    //     }
    // }
    // while (!opStack.empty())
    // {
    //     output.push(opStack.top());
    //     opStack.pop();
    // }
    // return output;
    return std::queue<ExpandedToken>{};
}

//TODO: change switch to if
const SemanticAnalyzer::operatorPrecedence SemanticAnalyzer::getPrecedence(const ExpandedToken& op) const
{
    switch (op.type) {
    case ExpandedToken::Type::POSTFIX_INCREMENT:
        return operatorPrecedence::Zero;
    case ExpandedToken::Type::ASSIGN:
        return operatorPrecedence::One;
    case ExpandedToken::Type::PREFIX_INCREMENT:
        return operatorPrecedence::Two;
    };
    throw std::runtime_error("getPrecedence");
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