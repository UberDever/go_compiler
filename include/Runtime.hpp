//
// Created by uberdever on 24.06.2020.
//

#ifndef GO_COMPILER_RUNTIME_HPP
#define GO_COMPILER_RUNTIME_HPP

#include "utility.hpp"

struct var
{
    std::string type;
    std::size_t level {0};
    std::size_t id {0};

public:
    var() = default;
    var(std::string _type, std::size_t _context, std::size_t _id) : type{_type}, level{_context}, id{_id} {}
};

class RuntimeVars
{
    static utility::hashT<var> vars;
    static utility::hashT<string*> lits;
    static utility::hashT<std::size_t> packages;
    ofstream logOut;
    size_t varCount {0};
public:
    RuntimeVars() = default;
    void appendPackage(string& str);
    void appendVar(string &name, string &type, std::size_t, std::size_t);
    void appendLit(string &str);

    bool searchPackage(string& str);
    bool searchVar(string &name, std::size_t context, std::size_t id);

    void print();
    void log(const string& path);
    inline int varCnt() {return varCount;}
    inline utility::hashT<var>* getVars() {return &vars;}
    inline utility::hashT<string*>* getLits() {return &lits;}
};

#endif //GO_COMPILER_RUNTIME_HPP
