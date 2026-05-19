# include "Parser.h"
# include "WyrdLang.h"

#include <memory>

Parser::Parser(std::vector<Token> tokens){
    this->tokens = tokens;
}

std::vector<Stmt*> Parser::parse(){
    std::vector<Stmt*> statements;

    while(!isAtEnd()){
        Stmt* stmt = declaration();
        if(stmt != nullptr) {
            statements.push_back(stmt);
        }
    }
    return statements;
}


Expr* Parser::expression(){
    return assignment();
}

Stmt* Parser::declaration(){
    try{
        if(match(TokenType::SPELL)) return function("spell");
        if(match(TokenType::ENCHANT)) return varDeclaration();
        return statement();
    }
    catch(ParseError error){
        synchronize();
        return nullptr;
    }
}

Function* Parser::function(std::string kind){
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;

    if(!check(TokenType::RIGHT_PAREN)){
        do{
            if(parameters.size() >= 255){
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        }
        while(match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<Stmt*> body = block();
    return new Function(name, parameters, body);
}

Stmt* Parser::statement(){
    if(match(TokenType::CYCLE)) return forStatement();
    if(match(TokenType::WHEN)) return ifStatement();
    if(match(TokenType::CAST)) return printStatement();
    if(match(TokenType::MANIFEST)) return returnStatement();
    if(match(TokenType::ASLONGAS)) return whileStatement();
    if(match(TokenType::LEFT_BRACE)) return new Block(block());

    return expressionStatement();
}

Stmt* Parser::returnStatement(){
    Token keyword = previous();
    Expr* value = nullptr;

    if(!check(TokenType::SEMICOLON)){
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return new Return(keyword, value);
}

Stmt* Parser::forStatement(){
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer;

    if(match(TokenType::SEMICOLON)){
        initializer = nullptr;
    }
    else if (match(TokenType::ENCHANT)){
        initializer = varDeclaration();
    }
    else{
        initializer = expressionStatement();
    }

    Expr* condition = nullptr;
    if(!check(TokenType::SEMICOLON)){
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = nullptr;
    if(!check(TokenType::RIGHT_PAREN)){
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    Stmt* body = statement();

    if (increment != nullptr) {
        std::vector<Stmt*> blockStatements;
        blockStatements.push_back(body);
        blockStatements.push_back(new Expression(increment));
        
        body = new Block(blockStatements);
    }

    if (condition == nullptr) condition = new Literal(true);
    body = new While(condition, body);

    if (initializer != nullptr) {
        std::vector<Stmt*> blockStatements;
        blockStatements.push_back(initializer);
        blockStatements.push_back(body);
        
        body = new Block(blockStatements);
    }

    return body;
}

Stmt* Parser::ifStatement(){
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();
    Stmt* elseBranch = NULL;
    if(match(TokenType::OTHERWISE)){
        elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
}

Stmt* Parser::printStatement(){
    Expr *expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return new Print(expr);
}

Stmt* Parser::varDeclaration(){
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    Expr* initializer = NULL;
    if(match(TokenType::EQUAL)){
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}

Stmt* Parser::whileStatement(){
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    Expr* condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    Stmt* body = statement();

    return new While(condition, body);
}

Stmt* Parser::expressionStatement(){
    Expr *expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}

std::vector<Stmt*> Parser::block(){
    std::vector<Stmt*> statements;

    while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

Expr* Parser::assignment(){
    Expr* expr = _or();

    if(match(TokenType::EQUAL)){
        Token equals = previous();
        Expr* value = assignment();

        if(Variable* e = dynamic_cast<Variable*>(expr)){
            Token name = e->name;
            return new Assign(name, value);
        }
        error(equals, "Invalid assignment target.");
    }
    return expr;
}

Expr* Parser::_or(){
    Expr* expr = _and();

    while(match(TokenType::EITHER)){
        Token oper = previous();
        Expr* right = _and();
        expr = new Logical(expr, oper, right);
    }
    return expr;
}

Expr* Parser::_and(){
    Expr* expr = equality();

    while(match(TokenType::TOGETHER)){
        Token oper = previous();
        Expr* right = equality();
        expr = new Logical(expr, oper, right);
    }
    return expr;
}

Expr* Parser::equality(){
    Expr* expr = comparison();
    while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
        Token oper = previous();
        Expr* right = comparison();
        expr = new Binary(expr, oper, right);
    }
    return expr;
}

Expr* Parser::comparison(){
    Expr* expr = term();

    while(match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)){
        Token oper= previous();
        Expr* right = term();
        expr = new Binary(expr, oper, right);
    }
    return expr;
}

Expr* Parser::term(){
    Expr* expr = factor();
    
    while(match(TokenType::MINUS, TokenType::PLUS)){
        Token oper = previous();
        Expr* right = factor();
        expr = new Binary(expr, oper, right);
    }
    return expr;
}

Expr* Parser::factor(){
    Expr* expr = unary();
    
    while(match(TokenType::SLASH, TokenType::STAR)){
        Token oper = previous();
        Expr* right = unary();
        expr = new Binary(expr, oper, right);
    }
    return expr;
}

Expr* Parser::unary(){
    if(match(TokenType::BANG, TokenType::MINUS)){
        Token oper = previous();
        Expr* right = unary();
        return new Unary(oper, right);
    }

    return call();
}

Expr* Parser::call(){
    Expr* expr = primary();

    while(true){
        if(match(TokenType::LEFT_PAREN)){
            expr = finishCall(expr);
        }
        else{
            break;
        }
    }

    return expr;
}

Expr* Parser::finishCall(Expr* callee){
    std::vector<Expr*> arguments;

    if(!check(TokenType::RIGHT_PAREN)){
        do{
            if(arguments.size() >= 255){
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        }
        while(match(TokenType::COMMA));

    }
    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return new Call(callee, paren, arguments);
}

Expr* Parser::primary(){
    if(match(TokenType::NAY)){
        return new Literal(false);
    }
    if(match(TokenType::AYE)){
        return new Literal(true);
    }
    if(match(TokenType::EMPTINESS)){
        return new Literal(std::any());
    }

    if(match(TokenType::NUMBER, TokenType::STRING)){
        return new Literal(previous().literal);
    }

    if(match(TokenType::IDENTIFIER)){
        return new Variable(previous());
    }

    if(match(TokenType::LEFT_PAREN)){
        Expr* expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }
    throw error(peek(), "Expect expression.");
}

bool Parser::match(TokenType type){
    if(check(type)){
        advance();
        return true;
    }
    return false;
}

template<typename... T>
bool Parser::match(TokenType first, T... rest){
    // fold expression: check(first) OR check(any of rest...)
    if((check(first) || ... || check(rest))){
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message){
    if(check(type)){
        return advance();
    }

    throw error(peek(), message);
}

bool Parser::check(TokenType type){
    if(isAtEnd()){
        return false;
    }
    return peek().type == type;
}

Token Parser::advance(){
    if(!isAtEnd()){
        current++;
    }
    return previous();
}

bool Parser::isAtEnd(){
    bool result = peek().type == END_OF_FILE;
    return result;
}

Token Parser::peek(){
    return tokens[current];
}

Token Parser::previous(){
    return tokens[current - 1];
}

Parser::ParseError Parser::error(Token token, std::string message){
    WyrdLang::error(token, message);
    return ParseError(message);
}

void Parser::synchronize(){
    advance();
    
    while(!isAtEnd()){
        if(previous().type == TokenType::SEMICOLON) return;
        
        switch(peek().type){
            case TokenType::CLAN:
            case TokenType::SPELL:
            case TokenType::ENCHANT:
            case TokenType::CYCLE:
            case TokenType::WHEN:
            case TokenType::ASLONGAS:
            case TokenType::CAST:
            case TokenType::MANIFEST:
                return;
        }
        advance();
    }
}