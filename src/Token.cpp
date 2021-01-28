//
// Created by uberdever on 30.05.2020.
//

#include "../include/Token.hpp"

#include <utility>

string Token::tokenTypes[(int)TOKEN_TYPE::END + 1] = {"Identifier", "Keyword", "Operator", "Literal", "Number"};

Token::Token(const string &_lex, TOKEN_TYPE _type) {
    lex = _lex;
    type = _type;
}

Token::Token(string _lex) {
    lex = std::move(_lex);
    type = TOKEN_TYPE::IDENTIFIER;
}
