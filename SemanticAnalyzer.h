#pragma once

#include "Token.h"
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>
#include <optional>

enum class ExpandedTokenType 
{
    VAR,
    INT,
    ASSIGN,
    PREFIX_INCREMENT,
    POSTFIX_INCREMENT
};

struct ExpandedToken
{
    ExpandedTokenType type;
    std::optional<std::string> value;
    unsigned short multiplier;

    ExpandedToken(ExpandedTokenType t, const std::optional<std::string> v, unsigned short m = 1) : type(t), value(v), multiplier(m) {}
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
    const operatorPrecedence getPrecedence(const Token &op) const;

    void evaluate(const std::queue<Token> &tokens);

    const bool isOperand(const Token& token) const;
};