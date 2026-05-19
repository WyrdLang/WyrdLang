#pragma once

#include "Token.h"

#include <vector>
#include <any>

#include "Expr.h"

class Block;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;

class StmtVisitor{
public:
	virtual std::any visitStmtBlock(Block* expr) = 0;
	virtual std::any visitStmtExpression(Expression* expr) = 0;
	virtual std::any visitStmtFunction(Function* expr) = 0;
	virtual std::any visitStmtIf(If* expr) = 0;
	virtual std::any visitStmtPrint(Print* expr) = 0;
	virtual std::any visitStmtReturn(Return* expr) = 0;
	virtual std::any visitStmtVar(Var* expr) = 0;
	virtual std::any visitStmtWhile(While* expr) = 0;
};

class Stmt{
public:
	virtual std::any Accept(StmtVisitor* visitor) = 0;
	virtual ~Stmt(){}
};

class Block: public Stmt{
public:
	std::vector<Stmt*> statements;

	Block(std::vector<Stmt*> statements){
		this->statements = statements;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtBlock(this);
	}
};

class Expression: public Stmt{
public:
	Expr* expression;

	Expression(Expr* expression){
		this->expression = expression;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtExpression(this);
	}
};

class Function: public Stmt{
public:
	Token name;
	std::vector<Token> params;
	std::vector<Stmt*> body;

	Function(Token name, std::vector<Token> params, std::vector<Stmt*> body){
		this->name = name;
		this->params = params;
		this->body = body;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtFunction(this);
	}
};

class If: public Stmt{
public:
	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

	If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch){
		this->condition = condition;
		this->thenBranch = thenBranch;
		this->elseBranch = elseBranch;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtIf(this);
	}
};

class Print: public Stmt{
public:
	Expr* expression;

	Print(Expr* expression){
		this->expression = expression;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtPrint(this);
	}
};

class Return: public Stmt{
public:
	Token keyword;
	Expr* value;

	Return(Token keyword, Expr* value){
		this->keyword = keyword;
		this->value = value;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtReturn(this);
	}
};

class Var: public Stmt{
public:
	Token name;
	Expr* initializer;

	Var(Token name, Expr* initializer){
		this->name = name;
		this->initializer = initializer;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtVar(this);
	}
};

class While: public Stmt{
public:
	Expr* condition;
	Stmt* body;

	While(Expr* condition, Stmt* body){
		this->condition = condition;
		this->body = body;
	}

	std::any Accept(StmtVisitor* visitor) override{
		return visitor->visitStmtWhile(this);
	}
};

