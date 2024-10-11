/******************************************************************************
E ::= T {+ T} 
T ::= F {* F}
F ::= (E) | i
*******************************************************************************/

// #include <stdio.h>
// #include <stdlib.h>
// int e();
// int t();
// int 
// int f();

// int symbol;
// void scan()
// {
//     symbol = getchar();
// }

// void error(const char* err)
// {
//     printf("%s\n", err);
//     printf("%c\n", symbol);
//     exit(1);
// }

// int main()
// {
//     scan();
//     int res = e();
//     if (symbol != '\n')
//         error("still some input exists");
    
//     printf("%s\n", "success");
//     printf("result = %d\n", res);
//     return 0;
// }

// int e()
// {
//     int res = t();
//     while (symbol == '+')
//     {
//         scan();
//         res += t();
//     }

//     return res;
// }

// int t()
// {
//     int res = f();
    
//     while (symbol == '*')
//     {
//         scan();
//         res *= f();
//     }
    
//     return res;
// }

// int f()
// {
//     int res;
//     if (symbol == '(')
//     {
//         scan();
//         res = e();
//         if (symbol != ')')
//             error("Non-matching parentheses");
//     }
//     else if (symbol < '0' || symbol >'9')
//     {
//         error("Unknown symbol");
//     }
//     else
//     {
//         res = symbol - '0';
//     }
    
//     scan();
//     return res;
// }


#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
// #include <cstdarg>

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

class Scanner {
public:
    explicit Scanner(const std::string& source) : source(source), currentIndex(0) {
        tokenize();
    }

    const std::vector<Token>& getTokens() const {
        return tokens;
    }

private:
    std::string source;
    size_t currentIndex;
    std::vector<Token> tokens;

    void tokenize() {
        std::regex tokenRegex(R"((\d+)|([a-zA-Z]+)|(=)|(\+\+))");
        auto begin = std::sregex_iterator(source.begin(), source.end(), tokenRegex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            if ((*it)[1].matched) {
                tokens.emplace_back(TokenType::INT, (*it)[1].str());
            } else if ((*it)[2].matched) {
                tokens.emplace_back(TokenType::VAR, (*it)[2].str());
            } else if ((*it)[3].matched) {
                tokens.emplace_back(TokenType::ASSIGN, (*it)[3].str());
            } else if ((*it)[4].matched) {
                tokens.emplace_back(TokenType::INCREMENT, (*it)[4].str());
            }
        }
        tokens.emplace_back(TokenType::END, "");  // End token
    }
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}

    void parse() {
        E();
        if (currentToken().type != TokenType::END) {
            throw std::runtime_error("Unexpected token at end of input.");
        }
        std::cout << "Input is valid." << std::endl;
    }

private:
    const std::vector<Token>& tokens;
    size_t currentIndex;

    const Token& currentToken() const {
        return tokens[currentIndex];
    }

    // std::string tokenTypeToStr()
    // {
    //     std::string typeInStr;
    //     switch (currentToken().type)
    //     {
    //     case TokenType::ASSIGN:
    //         typeInStr = "assign";
    //         break;
    //     case TokenType::END:
    //         typeInStr = "end";
    //         break;
    //     case TokenType::VAR:
    //         typeInStr = "var";
    //         break;    
    //     case TokenType::INT:
    //         typeInStr = "int";
    //         break;    
    //     case TokenType::INCREMENT:
    //         typeInStr = "increment";
    //         break;    
    //     }
    //     return typeInStr;
    // }

    void error()
    {   
        std::runtime_error("Syntax error on position " + 
                            std::to_string(currentIndex) + "!");
    }

    void E() {
        // change if to switch
        if (currentToken().type == TokenType::INT)
        {
            C();
        }
        else if (currentToken().type == TokenType::VAR)
        {
            T();
            if (currentToken().type == TokenType::ASSIGN) {
                E();
            }
        }
        else
        {
            error();
        }
        ++currentIndex;
    }

    void T() {
        if (currentToken().type == TokenType::VAR)
        {
            // add to ast var
        }
        T_;
        ++currentIndex; 
    }

    void T_() {
        if (currentToken().type != TokenType::INCREMENT)
        {
            return;
        }
        T_;
        ++currentIndex;
    }

    void C()
    {
        if (currentToken().type == TokenType::INT)
        {
            //add to ast
        }
        else
        {
            G();
        }
        ++currentIndex;
    }

    void G()
    {
        if (currentToken().type == TokenType::VAR)
        {
            // add to ast
        }
        else
        {
            // add to asd ++ op
            G();
        }
        ++currentIndex;
    }
};

int main() try {
    std::string input = "a = b = ++++c";  // Example input
    Scanner scanner(input);
    const auto& tokens = scanner.getTokens();
    Parser parser(tokens);
    parser.parse();
    return 0;
}
catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }