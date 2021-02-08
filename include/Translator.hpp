//
// Created by uberdever on 28.06.2020.
//

#ifndef GO_COMPILER_TRANSLATOR_HPP
#define GO_COMPILER_TRANSLATOR_HPP

#include "AST.hpp"
#include "Runtime.hpp"

class Translator
{
    enum PRINT {
        INT = 0, STR = 1, INT_NL = 2, STR_NL = 3
    };

    ofstream out;
    string path;
    AST* ast;
    static RuntimeVars* run;

    string asmData;
    string asmPreMain;
    string asmMain;

    string* asmCur {nullptr};
    int stackBytes {8};
    PRINT printType {INT};
    std::stack<int> forLabels;
    int unqLabelCount;
    int randNum;

public:
    Translator(AST*, string);
    void semAn();

    static void appendVars(Node*, Info&);
        static void addVar(Node*, Info&);
        static int countVars(Node *node, Info &info);
            static void countVar(Node *node, Info &info, int &count);
        static int countExprs(Node *node, Info &info);
            static void countExpr(Node *node, Info &info, int &count);
    static void checkVars(Node* node, Info& info);
        static void checkVar(Node*, Info&);

    static void opValid(Node*, Info&);
    static void srcHead(Node* node, Info& info);

    static void appendImports(Node* node, Info& info);
    static void checkImports(Node* node, Info& info);

    /************************************************************************************************************/

    enum asmPlace {
        DATA, MAIN, PREMAIN
    };

    void generate();

    void asmBlock();
        void _asmBlock(Node* node);

    void asmIf(Node* node);
        void _asmIf(Node* node, int unqLabel, size_t elseLabels);
    void asmVarDecl(Node *varList, Node* expList);
        void _asmVarDecl(Node *&var, Node *node);
    void asmExpr(Node *node);
    void asmCond(Node* node);

    void asmPrint(Node *node);
    void asmSqrt(Node *node);

    inline void stringLits();
    inline void sqrtOp();
    inline void localVars();
    inline void printProc();

    inline void push(const string& str);
    inline void pop(const string& str);
    inline void add(const string& lhs, const string& rhs);
    inline void sub(const string& lhs, const string& rhs);
    inline void imul(const string& lhs, const string& rhs);
    inline void div(const string& str);
    inline void mov(const string& lhs, const string& rhs);
    inline void raw(const string& str);
    inline void cmp(const string& lhs, const string& rhs);
    inline void jmp(const string& str);
    inline void je(const string& str);
    inline void jne(const string& str);
    inline void jl(const string& str);
    inline void jle(const string& str);
    inline void jg(const string& str);
    inline void jge(const string& str);
    inline void label(const string& str);
    inline void finit();
    inline void fild(const string& str);
    inline void fdiv(const string& lhs, const string& rhs);
    inline void fistp(const string& str);
    inline void inc(const string& str);
    inline void dec(const string& str);

    inline void setPtr(asmPlace place);

};

#endif //GO_COMPILER_TRANSLATOR_HPP
