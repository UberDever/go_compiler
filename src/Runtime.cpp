//
// Created by uberdever on 24.06.2020.
//

#include "../include/Runtime.hpp"

utility::hashT<var> RuntimeVars::vars{utility::HASH_TABLE_VARS};
utility::hashT<string *> RuntimeVars::lits{utility::HASH_TABLE_LITS};
utility::hashT<std::size_t> RuntimeVars::packages{utility::HASH_TABLE_PKG};

bool RuntimeVars::searchVar(string &name, std::size_t context, std::size_t id)
{
    auto *p = vars.search(name.c_str());
    bool result = p;
    if (result)
    {
        result &= p->level <= context;
        result &= p->id != id;
    }
    return result;
}

bool RuntimeVars::searchPackage(string &str)
{
    return packages.search(str.c_str());
}

void RuntimeVars::appendVar(string &name, string &type, std::size_t context, std::size_t id)
{
    auto *p = vars.search(name.c_str());
    if (p)
        if (p->level == context && p->id == id)
            throw logic_error("Variable " + name + " is already declared");
    vars[name.c_str()] = var{type, context, id};
    varCount++;
}

void RuntimeVars::appendPackage(string &str)
{
    if (packages.search(str.c_str()))
        throw logic_error("Package " + str + " is already imported");
    packages[str.c_str()];
}

void RuntimeVars::appendLit(string &str)
{
    if (!lits.search(str.c_str()))
        lits[str.c_str()] = &str;
}

void RuntimeVars::print()
{
    for (int i = 0; i < packages.size; i++)
    {
        if (!packages.table[i].isEmpty)
        {
            std::cout << "Imported pkg " << packages.table[i].key << std::endl;
        }
    }
    std::cout << std::endl;
    for (int i = 0; i < vars.size; i++)
    {
        if (!vars.table[i].isEmpty)
        {
            std::cout << "Var " << vars.table[i].key << " with type " << vars.table[i].data.type << " on level " << vars.table[i].data.level /* <<  " with id " << vars.table[i].data.id*/ << std::endl;
        }
    }
}

void RuntimeVars::log(const string &path)
{
    auto foundPos = path.find("../", 0);
    if (foundPos != string::npos)
        logOut.open("../logs/" + path.substr(foundPos + 1) + "_runtime.txt");
    else
        logOut.open("logs/" + path + "_runtime.txt");
    if (!logOut.is_open())
        throw logic_error("Cannot write log file from RUN");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(logOut.rdbuf());
    print();
    std::cout.rdbuf(coutbuf);
}
