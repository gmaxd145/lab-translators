#pragma once
// <E> ::= <G>V | <G>V = <F>  | V<L>
// <F>:: = <G>V = <F> | I | V<L> | <G>V
// <G>::= ++<G> | e
// <L> ::= ++<L> | ++


#include "Token.h"
#include <vector>

class SyntaxAnalyzer {
public:
    explicit SyntaxAnalyzer(const std::vector<Token>& tokens);

private:
    const std::vector<Token>& tokens;
    size_t currentIndex;

    void error(const std::string &msg);

    void E();
    void F();
    void G();
    void L();

    const Token& getCurrentToken() const;
    const std::vector<Token>& getTokens() const;    
    void error();

};