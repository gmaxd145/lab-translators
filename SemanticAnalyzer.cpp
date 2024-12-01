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
    for (const auto& token : tokens) {
        if (token.type == Token::Type::VAR) {
            _variables[token.value] = 0;
        }
    }
    // _variables = variables;
    // if (hasDuplicateKeys(_variables)) throw std::invalid_argument("Redefinition of variables!");
    auto expTokens = toExpandedTokens(tokens);
    toRPN(expTokens);
}

// bool SemanticAnalyzer::hasDuplicateKeys(const std::unordered_map<std::string, int>& myMap) {
//     std::set<std::string> uniqueKeys;
//     for (const auto& [key, value] : myMap) {
//         if (!uniqueKeys.insert(key).second) {
//             return true;
//         }
//     }
//     return false;
// }

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



// a=++b=c++;
// 1 step
// ++b = c++
// a = b
// 2 step
// b++c=c++
// ab=
void SemanticAnalyzer::toRPN(const std::vector<ExpandedToken> &expTokens)
{
    // 1 step: parsing to assignment expressions
    std::vector<std::vector<ExpandedToken>> exprs;
    std::vector<ExpandedToken> expr;
    int assigmentIndex{};
    auto exprEnd = expTokens.rbegin();
    for (auto it = expTokens.rbegin(); it != expTokens.rend(); ++it) {
        if ((*it).type == ExpandedToken::Type::ASSIGN) ++assigmentIndex;
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
    // 2 step: to rpn
    std::vector<std::vector<ExpandedToken>> rpnExprs;
    std::stack<ExpandedToken> opStack;
    std::optional<ExpandedToken> postfixToken;
    int postfixMultiplier; 
    int rpnExprsIndex{};
    for (const auto& expr : exprs) {
        rpnExprs.emplace_back(std::vector<ExpandedToken>());
        for (const auto& expToken : expr) {
            // rpn
            // postfix increment handler 
            if (expToken.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
                postfixToken = rpnExprs.at(rpnExprsIndex).back();
                postfixMultiplier = expToken.multiplier;
            }
            if (expToken.value.has_value()) rpnExprs.at(rpnExprsIndex).push_back(expToken);
            else if (expToken.type != ExpandedToken::Type::POSTFIX_INCREMENT) {
                while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(expToken))
                {
                    rpnExprs.at(rpnExprsIndex).push_back(opStack.top());
                    opStack.pop();
                }
                opStack.push(expToken);
            }
        }
        while (!opStack.empty()) {
            rpnExprs.at(rpnExprsIndex).push_back(opStack.top());
            opStack.pop();
        }
        if (postfixToken.has_value()) {
            rpnExprs.at(rpnExprsIndex).push_back(*postfixToken);
            rpnExprs.at(rpnExprsIndex).push_back({ExpandedToken::Type::POSTFIX_INCREMENT, 
                                             std::nullopt, postfixMultiplier});
        }
        ++rpnExprsIndex;
        postfixToken.reset();
    }
    // 3 step: evaluate
    for (const auto& rpnExpr : rpnExprs) {
        for (const auto& tk : rpnExpr) {
            if (tk.value.has_value()) {
                opStack.push(tk);        
            }
            else {
                if (tk.type == ExpandedToken::Type::POSTFIX_INCREMENT) {
                    _variables.at(opStack.top().value.value()) += opStack.top().multiplier;
                }
                if (tk.type == ExpandedToken::Type::PREFIX_INCREMENT) {
                    _variables.at(opStack.top().value.value()) += opStack.top().multiplier;
                }
                if (tk.type == ExpandedToken::Type::ASSIGN) {
                    int secondVar{};
                    if (opStack.top().type == ExpandedToken::Type::INT) secondVar = stoi(opStack.top().value.value());
                    else secondVar = _variables.at(opStack.top().value.value());
                    opStack.pop();
                    _variables.at(opStack.top().value.value()) = secondVar;
                    opStack.pop();
                }

            }
        }   
    }
}

//TODO: change switch to if
const SemanticAnalyzer::operatorPrecedence SemanticAnalyzer::getPrecedence(const ExpandedToken& op) const
{
    switch (op.type) {
    case ExpandedToken::Type::ASSIGN:
        return operatorPrecedence::Zero;
    case ExpandedToken::Type::PREFIX_INCREMENT:
        return operatorPrecedence::One;
    };
    throw std::runtime_error("getPrecedence");
}

void SemanticAnalyzer::printAnalyzeResults()
{
    for (const auto& pair : _variables) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
}