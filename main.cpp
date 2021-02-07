#include <iostream>
#include "./include/Compiler.hpp"
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc < 2)
        throw logic_error("Please enter correct program path");
    srand(time(nullptr));
    auto* c = new Compiler(argc, argv);
    return c->compile();
}
