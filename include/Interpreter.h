#pragma once

#include "Expr.h"
#include "WyrdLang.h"
#include "Stmt.h"
#include "Enviroment.h"

#include <any>
#include <string>
#include <iostream>
#include <vector>

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    void checkNumberOperand(Token oper, std::any operand);
    void checkNumberOperands(Token oper, std::any left,std::any right);
    bool isTruthy(std::any object);
    bool isEqual(std::any a, std::any b);
    std::string stringify(std::any object);
    std::any evaluate(Expr* expr);

    void execute(Stmt* stmt);
    
    Enviroment* globals = new Enviroment();

public:
    Enviroment* enviroment = globals; 
    Interpreter();
    void interpret(std::vector<Stmt*> statements);
    std::any visitExprLiteral(Literal* expr) override;
    std::any visitExprUnary(Unary* expr) override;
    std::any visitExprGrouping(Grouping* expr) override;
    std::any visitExprBinary(Binary* expr) override;
    std::any visitStmtExpression(Expression* stmt) override;
    std::any visitStmtPrint(Print* stmt) override;
    std::any visitStmtVar(Var* stmt) override;
    std::any visitExprVariable(Variable* expr) override;
    std::any visitExprAssign(Assign* expr) override;
    std::any visitStmtBlock(Block* stmt) override;
    void executeBlock(std::vector<Stmt*> statements, Enviroment* enviroment);
    std::any visitStmtIf(If* stmt) override;
    std::any visitExprLogical(Logical* expr) override;
    std::any visitStmtWhile(While* stmt) override;
    std::any visitExprCall(Call* expr) override;
    std::any visitStmtFunction(Function* stmt) override;
    std::any visitStmtReturn(Return* stmt) override;
};
