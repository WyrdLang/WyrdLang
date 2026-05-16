#pragma once

#include "TokenType.h"
#include <string>
#include <any>

class Token
{
private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
public:
    Token(TokenType _type, std::string _lexeme, std::any _literal, int _line);
    std::string toString();
};
