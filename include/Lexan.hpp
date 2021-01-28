//
// Created by uberdever on 30.05.2020.
//

#ifndef GO_COMPILER_LEXAN_HPP
#define GO_COMPILER_LEXAN_HPP

#include "Token.hpp"

enum class STATE
{
    DEFAULT,
    IDENTIFIER,
    OPERATOR,
    STRING,
    NUMBER
};

class Lexan
{
    static utility::hashT<uint32_t> keywords;
    static utility::hashT<uint32_t> operators;
    static utility::hashT<uint32_t> predeclIden;
    static vector<Token> tokenStream;
    static Token nullToken;

    string filePath;
    STATE curState = STATE::DEFAULT;
    size_t curToken {0};

    //size_t chainPtr{0};
    //size_t backPtr{0};

    static void setupLexemes();
    static void pushToken (string& lex, TOKEN_TYPE type);

public:
    Lexan(const string& _filePath);
    ~Lexan() = default;
    void split();
    static void print();

    Token& token();
    Token& peek() const;
    inline void next() {curToken++;};
    void prev();

    void eat(Token const& token);
    bool match(Token const& token, int relPos = 0) const;
    bool wait(Token const& token);
    void eatType(TOKEN_TYPE type);
    bool matchType(TOKEN_TYPE type, int relPos = 0) const;
    bool waitType(TOKEN_TYPE type);

    void semicolon();
    static bool searchKW(string& str);
    static bool searchID(string& str);
    bool isEmpty() const;
};

#endif //GO_COMPILER_LEXAN_HPP
