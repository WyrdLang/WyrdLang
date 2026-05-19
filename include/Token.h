#pragma once

#include "TokenType.h"
#include <string>
#include <any>


class Token
{
public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    // constructor for implementation in Token.cpp
    Token(TokenType _type, std::string _lexeme, std::any _literal, int _line);

    // Default constructor needed for copy operations 
    Token() = default; 

    // Copy constructor (explicitly defined)
    Token(const Token& other) = default; 
    
    std::string toString();
};
