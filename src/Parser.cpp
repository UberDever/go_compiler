//
// Created by uberdever on 30.05.2020.
//

#include "../include/Parser.hpp"

Parser::Parser(const string& filePath)  {
    lexan = new Lexan(filePath);
    ast = new AST();

    lexan->split();
    Lexan::print();
}

void Parser::parse() {
    std::cout << std::endl;
    std::cout << "BEGIN PARSING" << std::endl;
    ast->head() = sourceFile();
    if (!lexan->isEmpty())
        throw logic_error("Not all file has been parsed");
    std::cout << "FILE PARSED" << std::endl;
}

Node *Parser::sourceFile() {
    Node* pkg = package();
    Node* tld = topLvlDecl();
    return new Node("src", TOKEN_TYPE::KEYWORD, pkg, tld);
}

Node *Parser::package() {
    lexan->eat(Token("package", TOKEN_TYPE::KEYWORD));
    string name = lexan->token().lex;
    Node* tmp = new Node(name, TOKEN_TYPE::KEYWORD);
    lexan->semicolon();
    return new Node("head", TOKEN_TYPE::KEYWORD, tmp, import());
}

Node *Parser::import() {
    if (!lexan->match(Token("import", TOKEN_TYPE::KEYWORD)))
        return nullptr;
    lexan->next();
    Node* tmp = new Node(lexan->peek().lex, TOKEN_TYPE::LITERAL);
    lexan->eatType(TOKEN_TYPE::LITERAL);
    lexan->semicolon();
    return new Node("import", TOKEN_TYPE::KEYWORD, tmp, import());
}

Node *Parser::topLvlDecl() {
    Node* tmp = tld();
    if (tmp) return new Node("tld", TOKEN_TYPE::KEYWORD, tmp, topLvlDecl());
    return nullptr;
}

Node *Parser::tld() {
    if (lexan->isEmpty())
        return nullptr;
    Node* var = varDecl();
    if (var)
    {
        lexan->semicolon();
        return var;
    }
    Node* func = funcDecl();
    if (func)
    {
        lexan->semicolon();
        return func;
    }
    return nullptr;
}

Node *Parser::varDecl() {
    if (!lexan->wait(Token("var", TOKEN_TYPE::KEYWORD)))
        return nullptr;
    Node* var = varSpec();
    Node* exL {nullptr};
    if (lexan->wait(Token("=", TOKEN_TYPE::OPERATOR)))
        exL = exprList();
    return new Node("var", TOKEN_TYPE::KEYWORD, var, exL);
}

Node *Parser::funcDecl() {
    if (!lexan->wait(Token("func", TOKEN_TYPE::KEYWORD)))
        return nullptr;
    Node* sign = signature();
    Node* blk = block();
    return new Node("func", TOKEN_TYPE::KEYWORD, sign, blk);
}

Node *Parser::varSpec() {
    Node* id = idList();
    return new Node("var_desc", TOKEN_TYPE::KEYWORD, id, type());
}

Node *Parser::type() {
    string ty = lexan->peek().lex;
    if (Lexan::searchID(ty))
    {
        lexan->next();
        return new Node(ty, TOKEN_TYPE::KEYWORD);
    }
    if (lexan->match(Token("[", TOKEN_TYPE::OPERATOR)))
    {
        lexan->next();
        Node* len = expr();
        lexan->eat(Token("]", TOKEN_TYPE::OPERATOR));
        return new Node("arr", TOKEN_TYPE::KEYWORD, len, type());
    }
    return nullptr;
}

Node *Parser::signature() {
    string name = lexan->peek().lex;
    lexan->eatType(TOKEN_TYPE::IDENTIFIER);
    Node* tmp = par();
    return new Node(name, TOKEN_TYPE::KEYWORD, tmp, type());
}

Node *Parser::block() {
    if (!lexan->wait(Token("{", TOKEN_TYPE::OPERATOR)))
        return nullptr;
    Node* tmp = stmtList();
    lexan->eat(Token("}", TOKEN_TYPE::OPERATOR));
    return new Node("{}", TOKEN_TYPE::KEYWORD, tmp);
}

Node *Parser::idList() {
    if (!lexan->matchType(TOKEN_TYPE::IDENTIFIER))
        return nullptr;
    string name = lexan->token().lex;
    Node* tmp {nullptr};
    if (lexan->wait(Token(",", TOKEN_TYPE::OPERATOR)))
        tmp = idList();
    return new Node("id", TOKEN_TYPE::KEYWORD, new Node(name, TOKEN_TYPE::IDENTIFIER), tmp);
}

Node *Parser::stmtList() {
    Node* tmp = stmt();
    if (tmp) return new Node("stmt_list", TOKEN_TYPE::KEYWORD, tmp, stmtList());
    return nullptr;
}

Node *Parser::exprList() {
    Node* tmp = expr();
    if (tmp) {
        if (lexan->wait(Token(",", TOKEN_TYPE::OPERATOR))) {
            return new Node("expr_list", TOKEN_TYPE::KEYWORD, tmp, exprList());
        } else { return new Node("expr_list", TOKEN_TYPE::KEYWORD, tmp); }
    }
    return tmp;
}

Node *Parser::stmt() {
    while (lexan->wait(Token(";", TOKEN_TYPE::OPERATOR)));
    if (lexan->match(Token("null_token", TOKEN_TYPE::KEYWORD))) throw logic_error("Error while parsing a statement");
    Node* var = varDecl();
    if (var)
        return var;
    Node* blk = block();
    if (blk)
        return blk;
    Node* ifstmt = ifStmt();
    if (ifstmt)
        return ifstmt;
    Node* forstmt = forStmt();
    if (forstmt)
        return forstmt;
    Node* ret = returnStmt();
    if (ret)
        return ret;
    Node* con = continueStmt();
    if (con)
        return con;
    Node* brk = breakStmt();
    if (brk)
        return brk;
    Node* simp = simpleStmt(); //simp
    if (simp)
        return simp;
    return nullptr;
}

Node *Parser::simpleStmt() {
    if (lexan->wait(Token(";", TOKEN_TYPE::OPERATOR))) return nullptr;
    int commas {0};
    bool allId {true};
    bool isEq {false};
    bool isColEq {false};
    bool isIncDec {false};
    analyzeStmt(commas, allId, isEq, isColEq, isIncDec);
    if (allId && isColEq && !isEq && !isIncDec)
        return shvd();
    if (!commas && isIncDec)
        return incdec();
    if (isEq && !isColEq)
        return assign();
    return expr();
}

Node *Parser::par() {
    lexan->eat(Token("(", TOKEN_TYPE::OPERATOR));
    Node* tmp = parList();
    lexan->eat(Token(")", TOKEN_TYPE::OPERATOR));
    return tmp;
}

Node *Parser::shvd() {
    Node* tmp = idList();
    if (tmp)
    {
        lexan->eat(Token(":=", TOKEN_TYPE::OPERATOR));
        return new Node(":=", TOKEN_TYPE::OPERATOR, tmp, exprList());
    }
    return nullptr;
}

Node *Parser::assign() {
    Node* tmp = exprList();
    if (tmp)
    {
        lexan->eat(Token("=", TOKEN_TYPE::OPERATOR));
        return new Node("=", TOKEN_TYPE::OPERATOR, tmp, exprList());
    }
    return nullptr;
}

Node *Parser::incdec() {
    Node* tmp = expr();
    if (tmp)
    {
        string incdec = lexan->peek().lex;
        if (lexan->match(Token("++", TOKEN_TYPE::OPERATOR))) { lexan->next(); }
        else { lexan->eat(Token("--", TOKEN_TYPE::OPERATOR)); }
        return new Node(incdec, TOKEN_TYPE::OPERATOR, tmp);
    }
    return nullptr;
}

Node *Parser::expr() {
    Node* tmp = binaryOp();
    if (lexan->match(Token("||", TOKEN_TYPE::OPERATOR)))
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, tmp, expr());
    }
    return tmp;
}

Node *Parser::binaryOp() {
    Node* tmp = relOp();
    if (lexan->match(Token("&&", TOKEN_TYPE::OPERATOR)))
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, tmp, binaryOp());
    }
    return tmp;
}

Node *Parser::relOp() {
    Node* tmp = addOp();
    if (lexan->match(Token("==", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("!=", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("<", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("<=", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token(">", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token(">=", TOKEN_TYPE::OPERATOR)))
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, tmp, addOp());
    }
    return tmp;
}

Node *Parser::addOp() {
    Node* tmp = mulOp();
    if (lexan->match(Token("+", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("-", TOKEN_TYPE::OPERATOR))/* ||
        lexan->match(Token("|", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("^", TOKEN_TYPE::OPERATOR))*/)
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, tmp, addOp());
    }
    return tmp;
}

Node *Parser::mulOp() {
    Node* tmp = unaryOp();
    if (lexan->match(Token("*", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("/", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("%", TOKEN_TYPE::OPERATOR))/* ||
        lexan->match(Token("<<", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token(">>", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("&", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("&^", TOKEN_TYPE::OPERATOR))*/)
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, tmp, mulOp());
    }
    return tmp;
}

Node *Parser::unaryOp() {
    if (lexan->match(Token("-", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("+", TOKEN_TYPE::OPERATOR))/* ||
        lexan->match(Token("!", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("^", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("*", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("&", TOKEN_TYPE::OPERATOR)) ||
        lexan->match(Token("<-", TOKEN_TYPE::OPERATOR))*/)
    {
        return new Node(lexan->token().lex, TOKEN_TYPE::OPERATOR, unary());
    }
    return unary();
}

Node *Parser::unary() {
    Node* tmp = primary();
    if (lexan->wait(Token("[", TOKEN_TYPE::OPERATOR)))
    {
        Node* ind = expr();
        lexan->eat(Token("]", TOKEN_TYPE::OPERATOR));
        return new Node("[]", TOKEN_TYPE::KEYWORD, tmp, ind);
    }
    if (lexan->wait(Token("(", TOKEN_TYPE::OPERATOR)))
    {
        Node* expL = exprList();
        lexan->eat(Token(")", TOKEN_TYPE::OPERATOR));
        return new Node("()", TOKEN_TYPE::KEYWORD, tmp, expL);
    }
    return tmp;
}

Node *Parser::primary() {
    return operand();
}

Node *Parser::operand() {
    string& name = lexan->peek().lex;
    if (lexan->matchType(TOKEN_TYPE::IDENTIFIER))
    {
        if (lexan->match(Token(".", TOKEN_TYPE::OPERATOR), 1))
        {
            lexan->next(); lexan->next();
            string& iden = lexan->token().lex;
            if (!lexan->match(Token("(", TOKEN_TYPE::OPERATOR)))
                throw logic_error(name + "." + iden + " must be a function call");
            return new Node(name, TOKEN_TYPE::KEYWORD, new Node (iden, TOKEN_TYPE::KEYWORD));
        }
        if (Lexan::searchID(name))
        {
            return new Node(lexan->token().lex, TOKEN_TYPE::KEYWORD);
        }
    }
    if (lexan->wait(Token("(", TOKEN_TYPE::OPERATOR)))
    {
        Node* tmp = expr();
        lexan->eat(Token(")", TOKEN_TYPE::OPERATOR));
        return tmp;
    }
    if (lexan->matchType(TOKEN_TYPE::OPERATOR))
        return nullptr;
    return new Node(name, lexan->token().type);
}

Node *Parser::forStmt() {
    if (!lexan->wait(Token("for", TOKEN_TYPE::KEYWORD)))
        return nullptr;
    int ptr{0};
    Node* forclause {nullptr};
    while (!lexan->match(Token("{", TOKEN_TYPE::OPERATOR), ptr))
    {
        if (lexan->match(Token(";", TOKEN_TYPE::OPERATOR), ptr))
        {
            forclause = forClause();
            break;
        }
        ptr++;
    }
    if (!forclause)
        forclause = expr();
    return new Node("for", TOKEN_TYPE::KEYWORD, forclause, block());
}

Node *Parser::ifStmt() {
    if (!lexan->wait(Token("if", TOKEN_TYPE::KEYWORD)))
        return nullptr;
    int ptr{0};
    Node* simp = {nullptr};
    while (!lexan->match(Token("{", TOKEN_TYPE::OPERATOR), ptr))
    {
        if (lexan->match(Token(";", TOKEN_TYPE::OPERATOR), ptr))
        {
            simp = simpleStmt();
            lexan->semicolon();
            break;
        }
        ptr++;
    }
    Node* exp = expr();
    Node* cond = new Node("cond", TOKEN_TYPE::KEYWORD, exp, simp);
    Node* ifstmt = new Node("if", TOKEN_TYPE::KEYWORD, cond, block());
    if (lexan->wait(Token("else", TOKEN_TYPE::KEYWORD)))
    {
        Node* tmp {nullptr};
        if (lexan->match(Token("if", TOKEN_TYPE::KEYWORD)))
            tmp = ifStmt();
        else
            tmp = block();
        return new Node("else", TOKEN_TYPE::KEYWORD, ifstmt, tmp);
    }
    return new Node("else", TOKEN_TYPE::KEYWORD, ifstmt);
}

Node *Parser::forClause() {
    Node* init = simpleStmt();
    lexan->wait(Token(";", TOKEN_TYPE::OPERATOR));
    Node* cond = expr();
    lexan->wait(Token(";", TOKEN_TYPE::OPERATOR));
    Node* post = simpleStmt();
    return new Node("for_clause", TOKEN_TYPE::KEYWORD, new Node("border", TOKEN_TYPE::KEYWORD, init, post), cond);
}

Node *Parser::parList() {
    Node* tmp = parDecl();
    if (tmp)
        if (lexan->wait(Token(",", TOKEN_TYPE::OPERATOR)))
            return new Node("par_list", TOKEN_TYPE::KEYWORD, tmp, parList());
    return tmp;
}

Node *Parser::parDecl() {
    Node* tmp = idList();
    return new Node("par", TOKEN_TYPE::KEYWORD, tmp, type());
}

Node *Parser::returnStmt() {
    if (lexan->wait(Token("return", TOKEN_TYPE::KEYWORD)))
        return new Node("return", TOKEN_TYPE::KEYWORD, exprList());
    return nullptr;
}

Node *Parser::breakStmt() {
    if (lexan->wait(Token("break", TOKEN_TYPE::KEYWORD)))
        return new Node("break", TOKEN_TYPE::KEYWORD);
    return nullptr;
}

Node *Parser::continueStmt() {
    if (lexan->wait(Token("continue", TOKEN_TYPE::KEYWORD)))
        return new Node("continue", TOKEN_TYPE::KEYWORD);
    return nullptr;
}

inline void Parser::analyzeStmt(int& commas, bool& allId, bool& isEq, bool& isColEq, bool& isIncDec) {
    int ptr {0};
    while (!lexan->match(Token(";", TOKEN_TYPE::OPERATOR), ptr))
    {
        if (lexan->match(Token(",", TOKEN_TYPE::OPERATOR), ptr))
        {
            commas++;
            ptr++;
            continue;
        }
        if (lexan->matchType(TOKEN_TYPE::IDENTIFIER, ptr))
        {
            ptr++;
            continue;
        }
        if (lexan->match(Token("=", TOKEN_TYPE::OPERATOR), ptr))
        {
            isEq = true; allId = false;
            break;
        }
        if (lexan->match(Token(":=", TOKEN_TYPE::OPERATOR), ptr))
        {
            isColEq = true;
            break;
        }
        if (lexan->match(Token("++", TOKEN_TYPE::OPERATOR), ptr) || lexan->match(Token("--", TOKEN_TYPE::OPERATOR), ptr))
        {
            allId = false;
            isIncDec = true;
            break;
        }
        allId = false;
        ptr++;
    }
}




