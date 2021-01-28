//
// Created by uberdever on 30.05.2020.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <stack>

#ifndef GO_COMPILER_UTILITY_HPP
#define GO_COMPILER_UTILITY_HPP

using namespace std;

namespace utility
{
    constexpr int HASH_KEY_LEN = 50;
    constexpr int OPERATOR_LEN = 10;
    constexpr int HASH_TABLE_VARS = 101;
    constexpr int HASH_TABLE_LITS = 101;
    constexpr int HASH_TABLE_PKG = 23;
    constexpr const char* CFG_OPERATORS_PATH = "../cfg/operators";
    constexpr const char* CFG_KEYWORDS_PATH = "../cfg/keywords";
    constexpr const char* CFG_PREDECLIDEN_PATH = "../cfg/predeclIden";
    constexpr const char* AST_LOG_PATH = "../log/";

    //asm consts
    constexpr const char* asmHead = ".586\n"
                                    ".model flat, stdcall\n\n"
                                    "include <\\masm32\\include\\msvcrt.inc>\n"
                                    "include <\\masm32\\include\\kernel32.inc>\n"
                                    "includelib <\\masm32\\lib\\msvcrt.lib>\n"
                                    "includelib <\\masm32\\lib\\kernel32.lib>\n";
    constexpr const char* dataStart = "data segment\n";
    constexpr const char* dataEnd = "data ends\n";
    constexpr const char* textStart = "text segment\n";
    constexpr const char* textEnd = "text ends\n";
    constexpr const char* labelStart = "__start:\n";
    constexpr const char* labelEnd = "end __start\n";
    constexpr const char* procProlog = "   enter ";
    constexpr const char* procEpilogue = "   leave\n";
    constexpr const char* funcRet = "   ret\n";
    constexpr const char* exit = "   GenPush 0\n"
                                 "   call ExitProcess\n";
    constexpr const char* tab = "   ";

    constexpr const char* eax = "eax";
    constexpr const char* ebx = "ebx";
    constexpr const char* ecx = "ecx";
    constexpr const char* edx = "edx";
    constexpr const char* null = "0";
    constexpr const char* one = "1";
    constexpr const char* nOne = "-1";
    template <typename T>
    struct hNode
    {
        T data;
        char key[HASH_KEY_LEN];
        bool isEmpty;
        bool isLast;

        hNode() : data{}, isEmpty{true}, isLast{false}, key{""} {}
        ~hNode() = default;

        hNode<T>& operator=(const T& rhs)
        {
            data = rhs;
            return *this;
        }
    };

    uint32_t hash(const char* data, size_t len);

    template <typename T>
    struct hashT
    {
        const size_t size;
        hNode<T>* table;

        explicit hashT(const uint32_t _size) : size{_size}, table{nullptr} { table = new hNode<T>[size]; }
        ~hashT() {delete[] table;}

        hNode<T>& operator[](const char* _key) //do NOT use this for retrieving the element
        {
            char key[HASH_KEY_LEN] {};
            strncat(key, _key, HASH_KEY_LEN - 1);
            uint32_t h = hash(key, strlen(key)) % size;
            uint32_t ind = 0; uint32_t probe = h;
            while (!table[probe].isEmpty) {
                table[probe].isLast = false;
                ind++;
                probe = (h + (ind * ind)) % size;
                if (probe == h)
                {
                    std::cout << "hashTable is full" << std::endl;
                    break;
                }
            }
            strncat(table[probe].key, key, HASH_KEY_LEN - 1);
            table[probe].isEmpty = false;
            table[probe].isLast = true;
            return table[probe];
        }

        T* search(const char* _key)
        {
            char key[HASH_KEY_LEN] {};
            strncat(key, _key, HASH_KEY_LEN - 1);
            uint32_t h = hash(key, strlen(key)) % size;
            if (table[h].isEmpty) return nullptr;
            uint32_t ind = 0;
            uint32_t probe = h;
            while (strcmp(table[probe].key, key))
            {
                if (table[probe].isLast)
                    return nullptr;
                ind++;
                probe = (h + (ind * ind)) % size;
            }
            //std::cout << table[probe].key << " -> " << h << std::endl;
            return &table[probe].data;
        }

        void clean()
        {
            for (int i = 0; i < size; i++)
            {
                delete table[i]->data;
                table[i]->isEmpty = true;
            }
        }
    };

    inline bool isDelimiter(char sym)
    {
        return  sym == '\r' || sym == '\n' ||
                sym == '\t' || sym == '\b' ||
                sym == '\f' || sym == '\v' ||
                sym == ' ';
    }

    inline bool isOperator(char sym)
    {
        return sym == ':' || sym == ';' ||
               sym == ',' || sym == '.' ||
               sym == '{' || sym == '}' ||
               sym == '(' || sym == ')' ||
               sym == '[' || sym == ']' ||
               sym == '*' || sym == '/' ||
               sym == '+' || sym == '-' ||
               sym == '&' || sym == '|' ||
               sym == '=' || sym == '!' ||
               sym == '<' || sym == '>' ||
               sym == '^' || sym == '%' ||
               sym == '_';
    }

    inline bool isNumber(char sym)
    {
        return isdigit(sym) || sym == '.' ||
                 sym == 'a' || sym == 'A' ||
                 sym == 'b' || sym == 'B' ||
                 sym == 'c' || sym == 'C' ||
                 sym == 'd' || sym == 'D' ||
                 sym == 'e' || sym == 'E' ||
                 sym == 'f' || sym == 'F' ||
                 sym == 'x' || sym == 'X' ||
                 sym == 'o' || sym == 'O';
    }

    inline bool isCondOp(string& str)
    {
        return str == "==" || str == "!=" ||
               str == ">=" || str == "<=" ||
               str == ">" || str == "<" ;
    }

    inline bool isBinOp(string& str)
    {
        return str == "&&" || str == "||" ||
               isCondOp(str) ||
               str == "+" || str == "-" ||
               str == "*" || str == "/";
    }
}


#endif //GO_COMPILER_UTILITY_HPP
