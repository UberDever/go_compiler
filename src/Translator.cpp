//
// Created by uberdever on 28.06.2020.
//

#include "../include/Translator.hpp"

#include <utility>

RuntimeVars* Translator::run = new RuntimeVars;

static auto asmVar = [](Node* node, RuntimeVars* run) {return node->lex + "_" + run->getVars()->search(node->lex.c_str())->type
                                                       + "_" + to_string(run->getVars()->search(node->lex.c_str())->level) /*+ "_" + to_string(run->getVars()->search(node->lex.c_str())->id)*/+ string("[ebp]");};

Translator::Translator(AST * _ast, string _path) {
    ast = _ast;
    path = std::move(_path);
    out.open(path + ".asm");
    if (!out.is_open()) throw logic_error("Cannot open " + path + ".asm " + "for writing");
    asmCur = &asmData;
}

void Translator::semAn() {
    std::cout << std::endl;
    std::cout << "BEGIN SEMANTIC ANALYSIS" << std::endl;
    //ast->print();
    //run->print();
    ast->traverseTree(appendVars);
    //ast->traverseTree(checkVars);
    ast->traverseTree(opValid);
    ast->traverseTree(srcHead);
    ast->traverseTree(checkImports);
    //ast->print();
    //run->print();
    ast->log(path);
    run->log(path);
    std::cout << "FILE ANALYSED" << std::endl;
}

void Translator::appendVars(Node* node, Info& info) {
    auto static calcType = [&](Node* node)
    {
        if (node->lex == "true" || node->lex == "false")
            return "bool";
        for (char i : node->lex)
        {
            if (i == '.')
                return "float32";
        }
        return "int";
    };
    if (node->lex == "var")
    {
        Node* var_desc = node->l;
        if (var_desc->r)
            info.curType = var_desc->r->lex;
        else
            info.curType = calcType(node->r);
        addVar(var_desc->l, info);
        if (countVars(var_desc->l, info) != countExprs(node->r, info))
            throw logic_error("Initializer list in variable declaration has wrong number of arguments");
        return;
    }
    if (node->lex == ":=")
    {
        if (node->r)
            info.curType = calcType(node->r);
        addVar(node->l, info);
        if (countVars(node->l, info) != countExprs(node->r, info))
            throw logic_error("Initializer list in short variable declaration has wrong number of arguments");
        return;
    }
    if (node->type == TOKEN_TYPE::LITERAL)
    {
        run->appendLit(node->lex);
        return;
    }
}

void Translator::addVar(Node* node, Info& info) {
    if (!node) return;
    addVar(node->l, info);
    if (!node->l && !node->r)
    {
        node->isDecl = true;
        node->depth = info.depth;
        run->appendVar(node->lex, info.curType, info.depth, 0);
    }
    addVar(node->r, info);
}

int Translator::countVars(Node *node, Info &info) {
    int count = 0;
    countVar(node, info, count);
    return count;
}

void Translator::countVar(Node *node, Info &info, int &count) {
    if (!node) return;
    countVar(node->l, info, count);
    if (!node->l && !node->r)
    {
        count++;
    }
    countVar(node->r, info, count);
}

int Translator::countExprs(Node *node, Info &info) {
    int count = 0;
    countExpr(node, info, count);
    return count;
}

void Translator::countExpr(Node *node, Info &info, int &count) {
    if (!node) return;
    if (node->lex == "()")
        return;
    if (node->lex == "expr_list")
    {
        count++;
    }
    countExpr(node->l, info, count);
    countExpr(node->r, info, count);
}

void Translator::checkVars(Node *node, Info &info) {
    if (node->type == TOKEN_TYPE::IDENTIFIER)
    {
        if (!node->l)
        {
            checkVar(node, info);
        }
    }
}

void Translator::checkVar(Node *node, Info &info) {
    if (!node) return;
    checkVar(node->l, info);
    if (!node->l && !node->r)
    {
        if (!run->searchVar(node->lex, info.depth, 0))
            throw logic_error("Variable " + node->lex + " must be declared first");
    }
    checkVar(node->r, info);
}

void Translator::opValid(Node * node, Info & info) {
    if (node->lex == "for")
    {

    }
    if (utility::isBinOp(node->lex))
    {
        if (!(node->l || node->r)) throw logic_error("Operator " + node->lex + " has 0 operand, expected 2");
        if (node->lex == "-")
            return;
        if (!node->l || !node->r) throw logic_error("Operator " + node->lex + " has 1 operand, expected 2");
    }
}

void Translator::srcHead(Node *node, Info &info) {
    if (node->lex == "head")
    {
        if (!node->l) throw logic_error("Package name required");
        if (node->l->lex != "main") throw logic_error("Package must be named main");
        if (node->r)
            appendImports(node->r, info);
    }
    if (node->lex == "func")
    {
        if (node->l)
            if (node->l->lex != "main")
                throw logic_error("Function must be named main");
    }
}


void Translator::appendImports(Node *node, Info &info) {
    if (!node) return;
    appendImports(node->l, info);
    if (node->lex != "import")
    {
        run->appendPackage(node->lex);
    }
    appendImports(node->r, info);
}


void Translator::checkImports(Node *node, Info &info) {
    if (node->lex == "()" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (node->l)
        {
            if (!run->searchPackage(node->l->lex)) throw logic_error("Package " + node->l->lex +" must be imported first");
        }
    }
}


/**********************************************************************************************************************************************/

void Translator::generate() {
    using namespace utility;
    std::cout << "\nBEGIN CODE GENERATION" << std::endl;

    sqrtOp();
    localVars();
    printProc();
    stringLits();
    asmBlock();

    out << asmHead << "\n";
    out << dataStart << "\n";
    out << asmData << "\n";
    out << dataEnd << "\n";
    out << textStart << "\n";
    out << asmPreMain << "\n";
    out << labelStart << "\n";
    out << procProlog << run->varCnt() * 4 + 4 << ", 0" << "\n";
    out << asmMain << "\n";
    out << procEpilogue << "\n";
    out << funcRet << "\n";
    out << textEnd << "\n";
    out << labelEnd << "\n";

    out.close();

    std::cout << "CODE GENERATED" << std::endl;
}

void Translator::stringLits() {
    setPtr(DATA);
    auto* lits = run->getLits();
    for (int i = 0; i < utility::HASH_TABLE_LITS; i++)
    {
        if (!lits->table[i].isEmpty)
            raw(string(utility::tab) +
                                   "LIT" +
            to_string(utility::hash(lits->table[i].data->c_str(), lits->table[i].data->length())) +
                                " db \"" +
                    *lits->table[i].data +
                                    "\", 0\n"
            );
    }
    setPtr(MAIN);
}

void Translator::sqrtOp() {
    setPtr(DATA);
    raw(string(utility::tab) + "sqrtOp dd 0\n");
    setPtr(MAIN);
}

void Translator::localVars() {
    utility::hashT<var>* vars = run->getVars();
    for (int i = 0; i < utility::HASH_TABLE_VARS; i++)
    {
        if (!vars->table[i].isEmpty)
        {
            asmPreMain.append(vars->table[i].key);
            asmPreMain.append("_");
            asmPreMain.append(vars->table[i].data.type);
            asmPreMain.append("_");
            asmPreMain.append(to_string(vars->table[i].data.level));
            //asmPreMain.append("_");
            //asmPreMain.append(to_string(vars->table[i].data.id));
            asmPreMain.append(" = ");
            asmPreMain.append("-");
            asmPreMain.append(to_string(stackBytes));
            asmPreMain.append("\n");
            stackBytes += 4;
        }
    }
}

void Translator::printProc() {
    setPtr(DATA);
    raw(string(utility::tab) + "print_0 db \"%d\", 0\n");
    raw(string(utility::tab) + "print_1 db \"%s\", 0\n");
    raw(string(utility::tab) + "print_2 db \"%d \", 0\n");
    raw(string(utility::tab) + "print_3 db \"%s \", 0\n");
    raw(string(utility::tab) + "print_nl db 10, 0\n");

    setPtr(PREMAIN);
    raw("\nprint PROC\n");
    raw(string(utility::tab) + "enter 0, 0\n");
    mov(utility::ecx, "[ebp + 8]");
    mov(utility::eax, "[ebp + 12]");
    raw(string(utility::tab) + "invoke crt_printf, " + utility::ecx + ", eax\n");
    raw(string(utility::tab) + "leave\n");
    raw(string(utility::tab) + "ret 4\n");
    raw("print ENDP\n");

    setPtr(MAIN);
}

void Translator::asmBlock() {
    _asmBlock(ast->head());
}

void Translator::_asmBlock(Node *node) {
    if (!node) return;

    if (node->lex == "var" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (node->l)
            asmVarDecl(node->l->l, node->r);
    }
    if (node->lex == "--" && node->type == TOKEN_TYPE::OPERATOR)
    {
        push(asmVar(node->l, run));
        pop(utility::eax);
        dec(utility::eax);
        mov(asmVar(node->l, run), utility::eax);
        return;
    }
    if (node->lex == "++" && node->type == TOKEN_TYPE::OPERATOR)
    {
        push(asmVar(node->l, run));
        pop(utility::eax);
        inc(utility::eax);
        mov(asmVar(node->l, run), utility::eax);
        return;
    }
    if (node->lex == "=" && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmVarDecl(node->l, node->r);
        //asmExpr(node->r);
        //pop(utility::eax);
        //mov(asmVar(node->l, run), utility::eax);
        return;
    }
    if (node->lex == ":=" && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmVarDecl(node->l, node->r);
        return;
    }
    if (node->lex == "()" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (node->l->lex == "fmt")
        {
            Node* printFunc =  node->l;
            if (printFunc->l) {
                if (node->l->l->lex == "Print") { printType = INT; }
                else if (node->l->l->lex == "Println") { printType = INT_NL; }
            }
            mov(utility::ecx, "0");
            asmPrint(node->r);
            if (printType >= INT_NL)
            {
                push("offset print_nl");
                //pop(utility::ecx);
                raw(utility::tab + string("call print\n"));
            }
        }
        else if (node->l->lex == "math")
            asmSqrt(node->r);
        return;
    }
    if (node->lex == "for" && node->type == TOKEN_TYPE::KEYWORD)
    {
        int lab = rand() % (int)1e5;
        string start = "LOOPSTART_" + to_string(lab);
        string end = "LOOPEND_" + to_string(lab);
        string inc = "LOOPINC_" + to_string(lab);
        forLabels.push(lab);
        if (node->l)
        {
            if (node->l->lex == "for_clause")
            {
                Node* border = node->l->l;
                Node* cond = node->l->r;

                _asmBlock(border->l);
                label(start);
                if (cond)
                {
                    asmExpr(cond);
                    pop(utility::eax);
                    cmp(utility::eax, "0");
                    je(end);
                }
                _asmBlock(node->r);
                label(inc);
                _asmBlock(border->r);
                jmp(start);
                label(end);
            } else
            {
                label(start);
                asmExpr(node->l);
                pop(utility::eax);
                cmp(utility::eax, "0");
                je(end);
                _asmBlock(node->r);
                jmp(start);
                label(end);
            }
        } else
        {
            label(start);
            _asmBlock(node->r);
            jmp(start);
            label(end);
        }
        if (!forLabels.empty())
            forLabels.pop();
        return;
    }
    if (node->lex == "else" && node->type == TOKEN_TYPE::KEYWORD)
    {
        asmIf(node);
        return;
    }
    if (node->lex == "break" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (forLabels.empty())
            throw logic_error("Break is out of the loop");
        jmp("LOOPEND_" + to_string(forLabels.top()));
        forLabels.pop();
        return;
    }
    if (node->lex == "continue" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (forLabels.empty())
            throw logic_error("Continue is out of the loop");
        jmp("LOOPINC_" + to_string(forLabels.top()));
        forLabels.pop();
        return;
    }
    if (node->lex == "return" && node->type == TOKEN_TYPE::KEYWORD)
    {
        raw(utility::tab + string("leave\n"));
        raw(utility::tab + string("ret\n"));
    }
    _asmBlock(node->l);
    _asmBlock(node->r);
}


void Translator::asmVarDecl(Node *varList, Node* expList) {
    Node* varL = varList;
    _asmVarDecl(varL, expList);
}

void Translator::_asmVarDecl(Node *&var, Node *node) {
    if (!node) return;
    asmExpr(node->l);
    pop(utility::eax);
    mov(asmVar(var->l, run), utility::eax);
    var = var->r;
    _asmVarDecl(var, node->r);
}

void Translator::asmExpr(Node *node) {
    if (!node) return;

    if (node->type == TOKEN_TYPE::LITERAL)
    {
        push("offset ""LIT" + to_string(utility::hash(node->lex.c_str(), node->lex.length())));
        if (printType == INT || printType == INT_NL)
            printType = static_cast<PRINT>((int) printType + 1);
        return;
    }
    if (!node->isDecl && node->type == TOKEN_TYPE::IDENTIFIER)
    {
        push(asmVar(node, run));
        if (printType == STR || printType == STR_NL)
            printType = static_cast<PRINT>((int) printType - 1);
        return;
    }
    if (node->type == TOKEN_TYPE::NUMBER)
    {
        push(node->lex);
        if (printType == STR || printType == STR_NL)
            printType = static_cast<PRINT>((int) printType - 1);
        return;
    }
    if (node->lex == "-" && node->type == TOKEN_TYPE::OPERATOR && !node->r)
    {
        asmExpr(node->l);
        pop(utility::eax);
        imul(utility::eax, "-1");
        push(utility::eax);
        return;
    }
    if (!node->r && node->l)
    {
        asmExpr(node->l);
        return;
    }
    if (node->lex == "()" && node->type == TOKEN_TYPE::KEYWORD)
    {
        if (node->l->lex == "math")
            asmSqrt(node->r);
        return;
    }
    if (node->lex == "+" && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmExpr(node->l);
        asmExpr(node->r);
        pop(utility::eax);
        pop(utility::ebx);
        add(utility::eax, utility::ebx);
        push(utility::eax);
        return;
    }
    if (node->lex == "-" && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmExpr(node->l);
        asmExpr(node->r);
        pop(utility::ebx);
        pop(utility::eax);
        sub(utility::eax, utility::ebx);
        push(utility::eax);
        return;
    }
    if (node->lex == "*" && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmExpr(node->l);
        asmExpr(node->r);
        pop(utility::eax);
        pop(utility::ebx);
        imul(utility::eax, utility::ebx);
        push(utility::eax);
        return;
    }
    if ((node->lex == "/" && node->type == TOKEN_TYPE::OPERATOR) || (node->lex == "%" && node->type == TOKEN_TYPE::OPERATOR))
    {
        asmExpr(node->l);
        asmExpr(node->r);
        mov(utility::edx, "0");
        pop(utility::ebx);
        raw(string(utility::tab) + "cdq\n");
        pop(utility::eax);
        div(utility::ebx);
        if (node->lex == "/")
            push(utility::eax);
        else
            push(utility::edx);
        return;
    }
    if (utility::isCondOp(node->lex) && node->type == TOKEN_TYPE::OPERATOR)
    {
        asmCond(node);
        return;
    }
}

void Translator::asmCond(Node* node) {
    if (!node) return;

    int compares = rand() % (int)1e5;
    if (node->type == TOKEN_TYPE::NUMBER)
    {
        push(node->lex);
        return;
    }
    if (!node->isDecl && node->type == TOKEN_TYPE::IDENTIFIER)
    {
        string ifne = "CMPNE_" + to_string(compares);
        string end = "CMPEND_" + to_string(compares);
        cmp(asmVar(node, run), "0");
        jne(ifne);
        push("0");
        jmp(end);
        label(ifne);
        push("1");
        label(end);
        return;
    }
    asmExpr(node->l);
    pop(utility::ecx);
    asmExpr(node->r);
    pop(utility::edx);
    cmp(utility::ecx, utility::edx);
    string ifne = "CMPNE_" + to_string(compares);
    string end = "CMPEND_" + to_string(compares);
    if (node->lex == "<=")
        jg(ifne);
    else if (node->lex == "<")
        jge(ifne);
    else if (node->lex == ">=")
        jl(ifne);
    else if (node->lex == ">")
        jle(ifne);
    else if (node->lex == "!=")
        je(ifne);
    else if (node->lex == "==")
        jne(ifne);
    push("1");
    jmp(end);
    label(ifne);
    push("0");
    label(end);
}

void Translator::asmIf(Node *node) {
    int unqLabel = rand() % (int)1e5;
    static string end = "IFEND_" + to_string(unqLabel);
    size_t elseLabels {0};
    _asmIf(node, unqLabel, elseLabels);
    label(end);
}

void Translator::_asmIf(Node *node, int unqLabel, size_t elseLabels) {
    if (!node) return;

    Node* ifStmt = node->l;
    if (node->lex == "else")
    {
        if (ifStmt->l)
        {
            Node* cond = ifStmt->l;
            if (cond->r)
            {
                _asmBlock(cond->r);
            }
            if (cond->l)
            {
                asmExpr(cond->l);
            } else
            {
                push("1");
            }
        }
        pop(utility::eax);
        cmp(utility::eax, "0");
        je("IFELSE_" + to_string(unqLabel) + to_string(elseLabels));
        _asmBlock(ifStmt->r);
        jmp("IFEND_" + to_string(unqLabel));
    } else
    {
        _asmBlock(node);
        return;
    }
    label("IFELSE_" + to_string(unqLabel) + to_string(elseLabels));
    elseLabels++;
    _asmIf(node->r, unqLabel, elseLabels);
}

void Translator::asmPrint(Node* node) {
    if (!node) return;
    if (node->lex == "expr_list")
    {
        asmPrint(node->l);
        asmPrint(node->r);
        return;
    }
    asmExpr(node);
    push("offset print_" + to_string(printType));
    raw(utility::tab + string("call print\n"));
}

void Translator::asmSqrt(Node *node) {
    if (!node) return;
    if (node->lex == "expr_list")
    {
        asmSqrt(node->l);
        asmSqrt(node->r);
        return;
    }
    asmExpr(node);
    pop("sqrtOp");
    finit();
    fild("sqrtOp");
    raw(string(utility::tab) + "fsqrt\n");
    fistp("sqrtOp");
    push("sqrtOp");
}

void Translator::push(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("push ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::pop(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("pop ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::div(const string &str) {
    asmCur->append(utility::tab);
    asmCur->append("idiv ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::add(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("add ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::sub(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("sub ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::imul(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("imul ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::mov(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("mov ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::cmp(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("cmp ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::jmp(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jmp ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::je(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("je ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::jne(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jne ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::jl(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jl ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::jle(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jle ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::jg(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jg ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::jge(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("jge ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::label(const string &str)
{
    asmCur->append(str);
    asmCur->append(":\n");
}

void Translator::inc(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("inc ");
    asmCur->append(str);
    asmCur->append("\n");
}

void Translator::dec(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("dec ");
    asmCur->append(str);
    asmCur->append("\n");
}

void Translator::finit()
{
    asmCur->append(utility::tab);
    asmCur->append("finit\n");
}
void Translator::fild(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("fild ");
    asmCur->append(str);
    asmCur->append("\n");
}
void Translator::fdiv(const string &lhs, const string &rhs)
{
    asmCur->append(utility::tab);
    asmCur->append("fdiv ");
    asmCur->append(lhs);
    asmCur->append(", ");
    asmCur->append(rhs);
    asmCur->append("\n");
}
void Translator::fistp(const string &str)
{
    asmCur->append(utility::tab);
    asmCur->append("fistp ");
    asmCur->append(str);
    asmCur->append("\n");
}

void Translator::setPtr(Translator::asmPlace place) {
    if (place == DATA)
    {
        asmCur = &asmData;
    } else if (place == PREMAIN)
    {
        asmCur = &asmPreMain;
    } else
    {
        asmCur = &asmMain;
    }
}

void Translator::raw(const string& str) {
    asmCur->append(str);
}
