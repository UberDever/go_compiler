cmake_minimum_required(VERSION 3.16)
project(go_compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(go_compiler main.cpp include/utility.hpp include/Compiler.hpp src/Compiler.cpp include/Token.hpp src/Token.cpp include/Lexan.hpp src/Lexan.cpp include/Parser.hpp src/Parser.cpp src/utility.cpp include/AST.hpp src/AST.cpp include/Runtime.hpp src/Runtime.cpp include/Translator.hpp src/Translator.cpp)