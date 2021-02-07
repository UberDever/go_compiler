//
// Created by uberdever on 30.05.2020.
//

#include "../include/Compiler.hpp"

Compiler::Compiler(int argc, char *argv[])
{
    parser = new Parser(argv[argc - 1]);
    string name = argv[argc - 1];
    translator = new Translator(parser->getAST(), name.substr(0, name.length() - 3));
}

int Compiler::compile()
{
    parser->parse();
    translator->semAn();
    translator->generate();
    return 0;
}