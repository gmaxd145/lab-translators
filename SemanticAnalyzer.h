#pragma once

#include "Token.h"
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(const std::vector<Token>& tokens, const std::unordered_map<std::string, int>& variables);

    void printAnalyzeResults();

private:
    std::unordered_map<std::string, int> _variables;

    enum class operatorPrecedence
    {
        Zero,
        One
    };
    void toRPN(const std::vector<Token> &tokens);
    const operatorPrecedence getPrecedence(const Token& op) const;

    void evaluate(const std::queue<Token> &tokens);

    const bool isOperand(const Token& token) const;
};