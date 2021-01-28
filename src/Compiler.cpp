//
// Created by uberdever on 30.05.2020.
//

#include "../include/Compiler.hpp"

Compiler::Compiler(int argc, char* argv[]) {
    string name = "test.go";
    parser = new Parser(/*argv[argc - 1]*/name);
    name.pop_back();name.pop_back();name.pop_back();
    translator = new Translator(parser->getAST(), name);
}

int Compiler::compile() {
    parser->parse();
    translator->semAn();
    translator->generate();
    return 0;
}

