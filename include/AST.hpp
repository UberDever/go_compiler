//
// Created by uberdever on 07.06.2020.
//

#ifndef GO_COMPILER_AST_HPP
#define GO_COMPILER_AST_HPP

#include "utility.hpp"
#include "Token.hpp"

struct Node
{
    string lex;
    TOKEN_TYPE type {TOKEN_TYPE::KEYWORD};
    Node* l {nullptr};
    Node* r {nullptr};
    size_t depth {0};
    bool isDecl {false};

    Node(string _lex, TOKEN_TYPE _type = TOKEN_TYPE::KEYWORD, Node* _l = nullptr, Node* _r = nullptr) : lex{_lex}, type{_type}, l{_l}, r{_r} {};
};

struct Info
{
    int depth {0};
    size_t id {0};
    string curType{};
};

class AST
{
    ofstream logOut;
    Node* _head {nullptr};

    void printRecursive(Node*, int tabs);

public:
    inline Node*& head()
    {
        return _head;
    }
    AST() = default;
    void print();
    void log(const string& path);

    void traverseTree(void (*f)(Node *, Info &)) const;

    static void CLR(Node* node, void (*f)(Node*, Info&), Info&);
    static void LCR(Node* node, void (*f)(Node*, Info&), Info&);
    /*inline void left()
    {
        ptrStack.push(&(*cur)->l);
    }
    inline void right()
    {
        ptrStack.push(&(*cur)->r);
    }
    inline void assembleNode()
    {
        cur = ptrStack.top();
        ptrStack.pop();
        *cur = tmpL; tmpL = nullptr;
    }
    inline void assembleNode(string _lex, TOKEN_TYPE _type = TOKEN_TYPE::KEYWORD)
    {
        cur = ptrStack.top();
        ptrStack.pop();
        *cur = new node(_lex, _type);
        (*cur)->l = tmpL; tmpL = nullptr;
        (*cur)->r = tmpR; tmpR = nullptr;
    }
    inline void newNode(string _lex, TOKEN_TYPE _type = TOKEN_TYPE::KEYWORD)
    {
        if (!tmpL)
        {
            tmpL = new node(_lex, _type);
        } else if (!tmpR)
        {
            tmpR = new node(_lex, _type);
        }
        else throw logic_error("AST is binary");
    };*/
};

#endif //GO_COMPILER_AST_HPP
