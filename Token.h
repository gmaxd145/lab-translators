#include <string>

enum class TokenType {
    VAR,
    INT,
    ASSIGN,
    INCREMENT,
    END,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};