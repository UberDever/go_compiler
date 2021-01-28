//
// Created by uberdever on 30.05.2020.
//

#ifndef GO_COMPILER_PARSER_HPP
#define GO_COMPILER_PARSER_HPP

#include "Lexan.hpp"
#include "AST.hpp"
#include "Runtime.hpp"

class Parser
{
    Lexan* lexan {nullptr};
    AST* ast;
public:
    Parser(const string& filePath);
    ~Parser() {delete lexan;}

    inline AST* getAST() {
        return ast;
    }

    void parse();

    Node* sourceFile();
    Node* package();
    Node* import();

    Node* topLvlDecl();
    Node* tld();
        Node* varDecl();
            Node* varSpec();
            Node* type();
        Node* funcDecl();
            Node* signature();
                Node* par();
                    Node* parList();
                        Node* parDecl();
            Node* block();

    Node* forStmt();
        Node* forClause();

    Node* ifStmt();

    Node* stmtList();
        Node* stmt();
            Node* simpleStmt();
                Node* shvd();
                Node* assign();
                Node* incdec();
            Node* returnStmt();
            Node* breakStmt();
            Node* continueStmt();

    Node* exprList();
        Node* expr();
            Node* unary();
                Node* binaryOp();
                    Node* relOp();
                    Node* addOp();
                    Node* mulOp();
                Node* unaryOp();
            Node* primary();
                Node* operand();

    Node* idList();
    inline void analyzeStmt(int& commas, bool& allId, bool& isEq, bool& isColEq, bool& isIncDec);

};

#endif //GO_COMPILER_PARSER_HPP
