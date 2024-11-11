#pragma once

#include <string>

struct Token 
{
    enum class Type 
    {
    VAR,
    INT,
    ASSIGN,
    INCREMENT,
    END
    };

    Type type;
    std::string value;

    Token(Type t, const std::string& v) : type(t), value(v) {}
};