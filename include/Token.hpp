//
// Created by uberdever on 30.05.2020.
//

#ifndef GO_COMPILER_TOKEN_HPP
#define GO_COMPILER_TOKEN_HPP

#include "utility.hpp"

enum class TOKEN_TYPE : size_t
{
    IDENTIFIER      = 0,
    KEYWORD         = 1,
    OPERATOR        = 2,
    LITERAL         = 3,
    NUMBER          = 4,

    END             = 4
};

class Token
{
public:
    static string tokenTypes[(int)TOKEN_TYPE::END + 1];

    string lex;
    TOKEN_TYPE type;

public:
    Token(const string& _lex, TOKEN_TYPE type);
    Token(string _lex);

};

#endif //GO_COMPILER_TOKEN_HPP
