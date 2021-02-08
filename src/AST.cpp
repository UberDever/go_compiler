//
// Created by uberdever on 07.06.2020.
//

#include "../include/AST.hpp"

void AST::print()
{
    printRecursive(_head, 0);
}

void AST::printRecursive(Node *nd, int tabs)
{
    static constexpr int treeEdgeSymbols = 3;
    if (nd)
    {
        if (nd != _head)
        {
            for (int i = 0; i < (tabs - 1); i++)
                std::cout << "    ";
            std::cout << "!";
            for (int i = 0; i < treeEdgeSymbols; i++)
                std::cout << "-";
            std::cout << ">";
        }
        if (nd->isDecl)
            std::cout << "decl: ";
        std::cout << '\'' << nd->lex << '\'' << " (" << Token::tokenTypes[(int)nd->type] << ")";
        if (nd->depth)
            std::cout << " [" << nd->depth << "]";
        if (nd->l)
            std::cout << " L";
        if (nd->r)
            std::cout << " R";
        std::cout << std::endl;

        printRecursive(nd->l, tabs + 1);
        printRecursive(nd->r, tabs + 1);
    }
}

void AST::log(const string &path)
{
    auto foundPos = path.find("../", 0);
    if (foundPos != string::npos)
        logOut.open("../logs/" + path.substr(foundPos + 1) + "_ast.txt");
    else
        logOut.open("logs/" + path + "_ast.txt");
    if (!logOut.is_open())
        throw logic_error("Cannot write log file from AST");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(logOut.rdbuf());
    print();
    std::cout.rdbuf(coutbuf);
}

void AST::traverseTree(void (*f)(Node *, Info &)) const
{
    static Info info;
    CLR(_head, f, info);
    info.depth = 0;
}

void AST::CLR(Node *node, void (*f)(Node *, Info &), Info &info)
{
    if (node)
    {
        int id = info.id;
        if (node->lex == "{}" || node->lex == "for" || node->lex == "if")
        {
            info.depth++;
            info.id++;
        }
        (*f)(node, info);
        CLR(node->l, f, info);
        CLR(node->r, f, info);
        if (node->lex == "{}" || node->lex == "for" || node->lex == "if")
        {
            info.depth--;
            info.id = id;
        }
        return;
    }
}

void AST::LCR(Node *node, void (*f)(Node *, Info &), Info &info)
{
    if (node)
    {
        LCR(node->l, f, info);
        (*f)(node, info);
        LCR(node->r, f, info);
    }
}
