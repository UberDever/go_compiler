//
// Created by uberdever on 30.05.2020.
//

#ifndef GO_COMPILER_COMPILER_HPP
#define GO_COMPILER_COMPILER_HPP

#include "Parser.hpp"
#include "Translator.hpp"

class Compiler
{
    Translator* translator;
    Parser* parser;
public:
    Compiler(int, char*[]);
    ~Compiler() {}

    int compile();
};

#endif //GO_COMPILER_COMPILER_HPP
