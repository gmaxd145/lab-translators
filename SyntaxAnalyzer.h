// E::= T | T = E | C 
// T::= VT’
// T’::=++T’|e
// C::= G | I
// G::= V | ++G


#include "Token.h"
#include <vector>

class SyntaxAnalyzer {
public:
    explicit SyntaxAnalyzer(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}

    void analyze();

private:
    const std::vector<Token>& tokens;
    size_t currentIndex;
    
    void E();
    void T();
    void T_();
    void C();
    void G();

    const Token& getCurrentToken() const;
    const std::vector<Token>& getTokens() const;    
    void error();

};