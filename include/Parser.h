#pragma once

#include "Token.h"
#include "Expr.h"
#include "Stmt.h"

#include <vector>
#include <string>
#include <stdexcept> 

class Parser{
private:
    std::vector<Token> tokens;
    int current = 0;

    class ParseError : public std::runtime_error {
    public:
        ParseError(std::string& message) : std::runtime_error(message) {} 
    };
    
    Expr* expression();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();
    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    ParseError error(Token token, std::string message);
    void synchronize();
    Stmt* statement();
    Stmt* printStatement();
    Stmt* expressionStatement();
    Stmt* declaration();
    Stmt* varDeclaration();
    Expr* assignment();
    std::vector<Stmt*> block();
    Stmt* ifStatement();
    Expr* _or();
    Expr* _and();
    Stmt* whileStatement();
    Stmt* forStatement();
    Function* function(std::string kind);

    Expr* call();
    Expr* finishCall(Expr* callee);

    Stmt* returnStatement();

    //multiple token types
    template<typename... T>
    bool match(TokenType first, T... rest);


public:
    Parser(std::vector<Token> tokens);
    std::vector<Stmt*> parse();
};