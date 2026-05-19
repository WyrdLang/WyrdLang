#pragma once

#include "Token.h"

#include <vector>
#include <any>

class Assign;
class Binary;
class Call;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

class ExprVisitor{
public:
	virtual std::any visitExprAssign(Assign* expr) = 0;
	virtual std::any visitExprBinary(Binary* expr) = 0;
	virtual std::any visitExprCall(Call* expr) = 0;
	virtual std::any visitExprGrouping(Grouping* expr) = 0;
	virtual std::any visitExprLiteral(Literal* expr) = 0;
	virtual std::any visitExprLogical(Logical* expr) = 0;
	virtual std::any visitExprUnary(Unary* expr) = 0;
	virtual std::any visitExprVariable(Variable* expr) = 0;
};

class Expr{
public:
	virtual std::any Accept(ExprVisitor* visitor) = 0;
	virtual ~Expr(){}
};

class Assign: public Expr{
public:
	Token name;
	Expr* value;

	Assign(Token name, Expr* value){
		this->name = name;
		this->value = value;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprAssign(this);
	}
};

class Binary: public Expr{
public:
	Expr* left;
	Token op;
	Expr* right;

	Binary(Expr* left, Token op, Expr* right){
		this->left = left;
		this->op = op;
		this->right = right;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprBinary(this);
	}
};

class Call: public Expr{
public:
	Expr* callee;
	Token paren;
	std::vector<Expr*> arguments;

	Call(Expr* callee, Token paren, std::vector<Expr*> arguments){
		this->callee = callee;
		this->paren = paren;
		this->arguments = arguments;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprCall(this);
	}
};

class Grouping: public Expr{
public:
	Expr* expression;

	Grouping(Expr* expression){
		this->expression = expression;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprGrouping(this);
	}
};

class Literal: public Expr{
public:
	std::any value;

	Literal(std::any value){
		this->value = value;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprLiteral(this);
	}
};

class Logical: public Expr{
public:
	Expr* left;
	Token op;
	Expr* right;

	Logical(Expr* left, Token op, Expr* right){
		this->left = left;
		this->op = op;
		this->right = right;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprLogical(this);
	}
};

class Unary: public Expr{
public:
	Token op;
	Expr* right;

	Unary(Token op, Expr* right){
		this->op = op;
		this->right = right;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprUnary(this);
	}
};

class Variable: public Expr{
public:
	Token name;

	Variable(Token name){
		this->name = name;
	}

	std::any Accept(ExprVisitor* visitor) override{
		return visitor->visitExprVariable(this);
	}
};

