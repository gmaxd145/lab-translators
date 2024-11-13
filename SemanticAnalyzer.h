#pragma once

#include "Token.h"
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>
#include <optional>

//idea: make exptoken base class and make childs operator and operand

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

    bool hasDuplicateKeys(const std::unordered_map<std::string, int> &myMap);
    
    enum class operatorPrecedence
    {
        Zero,
        One
    };
    std::vector<ExpandedToken> toExpandedTokens(const std::vector<Token> &tokens);
    unsigned short getIncrementMultiplier(int& position, const std::vector<Token> &tokens);

    std::queue<ExpandedToken> toRPN(const std::vector<ExpandedToken> &expTokens);
    ExpandedToken getFirstVarFromLastExpr(const std::vector<std::vector<ExpandedToken>>& exprs);

    const operatorPrecedence getPrecedence(const ExpandedToken &op) const;

    void evaluate(std::queue<ExpandedToken> expTokens);
};