//
// Created by uberdever on 30.05.2020.
//

#include "../include/Lexan.hpp"

utility::hashT<uint32_t> Lexan::keywords{101};
utility::hashT<uint32_t> Lexan::operators{101};
utility::hashT<uint32_t> Lexan::predeclIden{101};
Token Lexan::nullToken("null_token", TOKEN_TYPE::KEYWORD);
vector<Token> Lexan::tokenStream{};

void Lexan::split()
{
    string lexeme;
    /*, tmpState;*/
    ifstream program{filePath};

    if (!program.is_open())
        throw logic_error("Program file path incorrect");

    auto splitOperators = [&](string &lex) {
        stringstream ss{lex};
        string tmp;
        tmp.push_back(ss.peek());
        uint32_t *p = operators.search(tmp.c_str());
        if (!p)
            throw logic_error("Wrong operator " + lex);
        while (!ss.eof())
        {
            do
            {
                ss.get();
                tmp.push_back(ss.peek());
                p = operators.search(tmp.c_str());
            } while (p);
            tmp.pop_back();
            pushToken(tmp, TOKEN_TYPE::OPERATOR);
            tmp.push_back(ss.peek());
        }
        lex.clear();
    };

    while (!program.eof())
    {
        char sym = program.get();

        if (sym == -1)
        {
            if (lexeme == "}")
            {
                pushToken(lexeme, TOKEN_TYPE::OPERATOR);
                string lex;
                lex.push_back(';');
                pushToken(lex, TOKEN_TYPE::OPERATOR);
            }
            break;
        }
        bool isOp = utility::isOperator(sym);
        bool isDel = utility::isDelimiter(sym);
        bool isNum = utility::isNumber(sym);
        bool isAlp = isalpha(sym) || sym == '_';
        bool isStr = sym == '"' || sym == '\'';
        bool isCom = sym == '/';

        auto changeState = [&]() {
            if (isAlp)
            {
                curState = STATE::IDENTIFIER;
                return;
            }
            if (isdigit(sym))
            {
                curState = STATE::NUMBER;
                return;
            }
            if (isOp)
            {
                curState = STATE::OPERATOR;
                return;
            }
            if (isStr)
            {
                curState = STATE::STRING;
                return;
            }
            if (isDel)
            {
                curState = STATE::DEFAULT;
                return;
            }
            std::cout << sym << std::endl;
            throw logic_error("Incorrect symbol " + to_string(sym));
        };

        auto insertSemicolon = [&]() {
            if (sym == '\n')
            {
                string lex;
                lex.push_back(';');
                pushToken(lex, TOKEN_TYPE::OPERATOR);
            }
        };

        if (isCom)
        {
            if (program.peek() == '/')
            {
                char tmp = program.get();
                while (tmp != '\n')
                {
                    tmp = program.get();
                    if (tmp == -1)
                        throw logic_error("Line comment parse error!");
                };
                continue;
            }
            if (program.peek() == '*')
            {
                char tmp = program.get();
                while (true)
                {
                    tmp = program.peek();
                    if (tmp == -1)
                        throw logic_error("General comment parse error!");
                    if (tmp == '*')
                    {
                        tmp = program.get();
                        if (program.peek() == '/')
                        {
                            program.get();
                            break;
                        }
                        else
                        {
                            program.seekg(-1, ios_base::cur);
                        }
                    }
                    program.get();
                }
                continue;
            }
        }

        switch (curState)
        {
        case STATE::DEFAULT:
        {
            changeState();
            break;
        }
        case STATE::IDENTIFIER:
        {
            if (isDel || isOp)
            {
                if (!lexeme.empty())
                {
                    auto *p = keywords.search(lexeme.c_str());

                    if (p)
                    {
                        bool isSemicolon = lexeme == "break" || lexeme == "continue" || lexeme == "return" || lexeme == "fallthrough";
                        pushToken(lexeme, TOKEN_TYPE::KEYWORD);
                        if (isSemicolon)
                        {
                            insertSemicolon();
                        }
                    }
                    else
                    {
                        pushToken(lexeme, TOKEN_TYPE::IDENTIFIER);
                        insertSemicolon();
                    }
                }
                curState = STATE::DEFAULT;
                if (isOp)
                {
                    curState = STATE::OPERATOR;
                    lexeme.push_back(sym);
                }
                continue;
            }
            if (isStr)
                throw logic_error("Expected opening quotes: " + lexeme);
            break;
        }
        case STATE::OPERATOR:
        {
            if (isStr)
            {
                pushToken(lexeme, TOKEN_TYPE::OPERATOR);
                curState = STATE::STRING;
                continue;
            }
            if (sym == '.')
            {
                if (isdigit(program.peek()))
                {
                    splitOperators(lexeme);
                    curState = STATE::NUMBER;
                    lexeme.push_back(sym);
                    continue;
                }
            }
            if (!isOp)
            {
                if (!lexeme.empty())
                {
                    auto isInc = [&](std::string str) { return (str.back() == '+' && str[str.length() - 1] == '+') || (str.back() == '-' && str[str.length() - 1] == '-'); };
                    bool isSemicolon = lexeme.back() == ']' || lexeme.back() == ')' || lexeme.back() == '}';
                    try
                    {
                        isSemicolon |= isInc(lexeme);
                    }
                    catch (const std::out_of_range &oor)
                    {
                        std::cerr << "String of operators out of range: " << oor.what() << '\n';
                    }
                    splitOperators(lexeme);
                    if (isSemicolon)
                    {
                        insertSemicolon();
                    }
                }
                changeState();
            }
            break;
        }
        case STATE::NUMBER:
        {
            if (!isNum)
            {
                if (!lexeme.empty())
                {
                    pushToken(lexeme, TOKEN_TYPE::NUMBER);
                }
                insertSemicolon();
                changeState();
            }
            break;
        }
        case STATE::STRING:
        {
            if (sym == '\n' || sym == -1)
                throw logic_error("Expected closing quotes: " + lexeme);
            if (isStr)
            {
                pushToken(lexeme, TOKEN_TYPE::LITERAL);
                sym = program.peek();
                insertSemicolon();
                curState = STATE::DEFAULT;
                continue;
            }
            lexeme.push_back(sym);
            continue;
        }
        }
        if (!isDel && !isStr)
            lexeme.push_back(sym);
    }
}

Lexan::Lexan(const string &_filePath)
{
    setupLexemes();

    filePath = _filePath;
}

void Lexan::pushToken(string &lex, TOKEN_TYPE type)
{
    switch (type)
    {
    case TOKEN_TYPE::NUMBER:
    {
        if (lex[0] == '0')
        {
            for (int i = 1; i < lex.length(); i++)
            {
                if (!utility::isNumber(lex[i]))
                    throw logic_error("Wrong number format: " + lex);
            }
        }
        else
        {
            for (int i = 1; i < lex.length(); i++)
            {
                if (!isdigit(lex[i]) && lex[i] != '.')
                    throw logic_error("Wrong number format: " + lex);
            }
        }
        break;
    }
    default:
        break;
    }
    tokenStream.emplace_back(lex, type);
    lex.clear();
}

void Lexan::setupLexemes()
{
    ifstream inOp(utility::CFG_OPERATORS_PATH);

    if (!inOp.is_open())
        throw logic_error("Operators file doesn't open");

    char sym;
    string tmp;

    while (!inOp.eof())
    {
        sym = inOp.get();
        if (utility::isDelimiter(sym) || !sym || sym == -1)
        {
            if (!tmp.empty())
            {
                operators[tmp.c_str()] = utility::hash(tmp.c_str(), strlen(tmp.data()));
                //std::cout << "op: " << tmp.c_str() << std::endl;
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(sym);
        }
    }

    ifstream inKw(utility::CFG_KEYWORDS_PATH);
    tmp.clear();

    if (!inKw.is_open())
        throw logic_error("Keywords file doesn't open");

    while (!inKw.eof())
    {
        sym = inKw.get();
        if (utility::isDelimiter(sym) || !sym || sym == -1)
        {
            if (!tmp.empty())
            {
                keywords[tmp.c_str()] = utility::hash(tmp.c_str(), strlen(tmp.data()));
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(sym);
        }
    }

    ifstream inPi(utility::CFG_PREDECLIDEN_PATH);
    tmp.clear();

    if (!inPi.is_open())
        throw logic_error("Predeclared identifiers file doesn't open");

    while (!inPi.eof())
    {
        sym = inPi.get();
        if (utility::isDelimiter(sym) || !sym || sym == -1)
        {
            if (!tmp.empty())
            {
                predeclIden[tmp.c_str()] = utility::hash(tmp.c_str(), strlen(tmp.data()));
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(sym);
        }
    }
}

void Lexan::print()
{
    for (auto &i : tokenStream)
    {
        std::cout << i.lex << " (" << Token::tokenTypes[(int)i.type] << ")" << std::endl;
    }
}

void Lexan::log(const string &path)
{
    auto foundPos = path.find("../", 0);
    if (foundPos != string::npos)
        logOut.open("../logs/" + path.substr(foundPos + 1) + "_lexer.txt");
    else
        logOut.open("logs/" + path + "_lexer.txt");
    if (!logOut.is_open())
        throw logic_error("Cannot write log file from RUN");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(logOut.rdbuf());
    print();
    std::cout.rdbuf(coutbuf);
}

Token &Lexan::token()
{
    auto &tmp = peek();
    next();
    return tmp;
}

Token &Lexan::peek() const
{
    if (isEmpty())
        return nullToken;
    return tokenStream[curToken];
}

bool Lexan::match(const Token &token, int relPos) const
{
    size_t pos = curToken + relPos;
    if (pos < tokenStream.size())
        return (token.type == tokenStream[pos].type && token.lex == tokenStream[pos].lex);
    else
        return (token.type == nullToken.type && token.lex == nullToken.lex);
}

void Lexan::eat(Token const &token)
{
    if (!match(token))
        throw logic_error("Wrong token: " + peek().lex + " (" + Token::tokenTypes[(int)peek().type] + "), expected " + token.lex + " (" + Token::tokenTypes[(int)token.type] + ")");
    next();
}

bool Lexan::wait(Token const &token)
{
    if (match(token))
    {
        next();
        return true;
    }
    return false;
}

bool Lexan::matchType(TOKEN_TYPE type, int relPos) const
{
    static string tokenTypes[(int)TOKEN_TYPE::END + 1] = {"Identifier", "Keyword", "Operator", "Literal", "Number"};

    size_t pos = curToken + relPos;
    if (pos < tokenStream.size())
        return (type == tokenStream[pos].type);
    else
        return (type == nullToken.type);
}

void Lexan::eatType(TOKEN_TYPE type)
{
    if (!matchType(type))
        throw logic_error("Wrong token: " + tokenStream[curToken].lex + " (" + Token::tokenTypes[(int)tokenStream[curToken].type] + "), expected (" + Token::tokenTypes[(int)type] + ")");
    next();
}

bool Lexan::waitType(TOKEN_TYPE type)
{
    if (matchType(type))
    {
        next();
        return true;
    }
    return false;
}

bool Lexan::searchKW(string &str)
{
    return keywords.search(str.c_str());
}

bool Lexan::searchID(string &str)
{
    return predeclIden.search(str.c_str());
}

bool Lexan::isEmpty() const
{
    return curToken >= tokenStream.size();
}

void Lexan::semicolon()
{
    eat(Token(";", TOKEN_TYPE::OPERATOR));
}

void Lexan::prev()
{
    if (curToken > 0)
        curToken--;
}
