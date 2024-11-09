#pragma once

#include "Token.h"
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>
#include <optional>

struct ExpandedToken
{
    enum class Type 
{
    VAR,
    INT,
    ASSIGN,
    PREFIX_INCREMENT,
    POSTFIX_INCREMENT
};

    Type type;
    std::optional<std::string> value;
    unsigned short multiplier;

    ExpandedToken(Type t, const std::optional<std::string> v, unsigned short m = 1) : type(t), value(v), multiplier(m) {}
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer(const std::vector<Token>& tokens, const std::unordered_map<std::string, int>& variables);

    void printAnalyzeResults();

private:
    std::unordered_map<std::string, int> _variables;

    enum class operatorPrecedence
    {
        Zero,
        One,
        Two
    };
    std::vector<ExpandedToken> toExpandedTokens(const std::vector<Token> &tokens);
    unsigned short getIncrementMultiplier(int& position, const std::vector<Token> &tokens);

    std::queue<ExpandedToken> toRPN(const std::vector<ExpandedToken> &expTokens);

    const operatorPrecedence getPrecedence(const ExpandedToken &op) const;

    void evaluate(const std::queue<Token> &tokens);

    // alternative: if value presents than operand other way operator
    // const bool isOperand(const Token& token) const;
};