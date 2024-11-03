#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include <regex>

// Forward declaration of Token and TokenType
enum class TokenType {
    INT,
    VAR,
    ASSIGN,
    INCREMENT,
    END
};

class Token {
public:
    Token(TokenType type, const std::string& value) : type(type), value(value) {}

    TokenType getType() const { return type; }
    const std::string& getValue() const { return value; }

private:
    TokenType type;
    std::string value;
};

class LexicalAnalyzer {
public:
    explicit LexicalAnalyzer(const std::string& source);
    
    const std::vector<Token>& getTokens() const;

private:
    std::string source;
    size_t currentIndex;
    std::vector<Token> tokens;

    void tokenize();
};

#endif // LEXICALANALYZER_H