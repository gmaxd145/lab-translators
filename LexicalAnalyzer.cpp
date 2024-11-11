#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(const std::string& source) 
    : source(source), currentIndex(0) {
    tokenize();
}

const std::vector<Token>& LexicalAnalyzer::getTokens() const {
    return tokens;
}

void LexicalAnalyzer::tokenize() {
    std::regex tokenRegex(R"((\d+)|([a-zA-Z]+)|(=)|(\+\+))");
    auto begin = std::sregex_iterator(source.begin(), source.end(), tokenRegex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        if ((*it)[1].matched) {
            tokens.emplace_back(Token::Type::INT, (*it)[1].str());
        } else if ((*it)[2].matched) {
            tokens.emplace_back(Token::Type::VAR, (*it)[2].str());
        } else if ((*it)[3].matched) {
            tokens.emplace_back(Token::Type::ASSIGN, (*it)[3].str());
        } else if ((*it)[4].matched) {
            tokens.emplace_back(Token::Type::INCREMENT, (*it)[4].str());
        }
    }
    tokens.emplace_back(Token::Type::END, "");  // End token
}