#include "Interpreter.h"
#include "RuntimeError.h"
#include "WyrdCallable.h"
#include "ClockCallable.h"
#include "WyrdFunction.h"
#include "ReturnException.h"

Interpreter::Interpreter() {
    globals->define("clock", std::make_any<WyrdCallable*>(new ClockCallable()));
}

void Interpreter::interpret(std::vector<Stmt*> statements){
    try{
        for(size_t i = 0; i < statements.size(); i++){
            execute(statements[i]);
        }
    }
    catch(RuntimeError error){
        WyrdLang::runtimeError(error);
    }
}

std::any Interpreter::evaluate(Expr* expr) {
    return expr->Accept(this);
}

void Interpreter::execute(Stmt* stmt){
    if(stmt != nullptr) {
        stmt->Accept(this);
    }
}

void Interpreter::executeBlock(std::vector<Stmt*> statements, Enviroment* enviroment){

    Enviroment* previous = this->enviroment;

    try{
        this->enviroment = enviroment;

        for(size_t i = 0; i < statements.size(); i++){
            execute(statements[i]);
        }
        
        this->enviroment = previous;
    }
    catch (...){
        this->enviroment = previous;
        throw;
    }
}


std::any Interpreter::visitStmtBlock(Block* stmt){
    Enviroment* env = new Enviroment(this->enviroment);
    executeBlock(stmt->statements, env);
    return {};
}

std::any Interpreter::visitStmtExpression(Expression* stmt){
    evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitStmtFunction(Function* stmt){
    WyrdFunction* function = new WyrdFunction(stmt, enviroment);
    enviroment->define(stmt->name.lexeme, std::make_any<WyrdCallable*>(function));
    return std::any();
}

std::any Interpreter::visitStmtReturn(Return* stmt){
    std::any value = std::any();
    if(stmt->value != NULL){
        value = evaluate(stmt->value);
    }

    throw ReturnException(value);
}

std::any Interpreter::visitStmtIf(If* stmt){
    if(isTruthy(evaluate(stmt->condition))){
        execute(stmt->thenBranch);
    }
    else if (stmt->elseBranch != NULL){
        execute(stmt->elseBranch);
    }
    return {};
}

std::any Interpreter::visitStmtPrint(Print* stmt){
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << "\n";
    return {};
}

std::any Interpreter::visitStmtVar(Var* stmt){
    std::any value;
    if(stmt->initializer != NULL){
        value = evaluate(stmt->initializer);
    }

    enviroment->define(stmt->name.lexeme, value);
    return {};
}

std::any Interpreter::visitStmtWhile(While* stmt){
    while (isTruthy(evaluate(stmt->condition))){
        execute(stmt->body);
    }
    return {};
}

std::any Interpreter::visitExprAssign(Assign* expr){
    std::any value = evaluate(expr->value);
    enviroment->assign(expr->name, value);
    return value;
}

std::any Interpreter::visitExprGrouping(Grouping* expr) {
    return evaluate(expr->expression);
}

std::any Interpreter::visitExprLiteral(Literal* expr) {
    return expr->value;
}

std::any Interpreter::visitExprLogical(Logical* expr){
    std::any left = evaluate(expr->left);
    
    if(expr->op.type == TokenType::EITHER){
        if(isTruthy(left)) return left;
    }
    else {
        if (!isTruthy(left)) return left;
    }

    std::any right = evaluate(expr->right);
    
    return right;
}

std::any Interpreter::visitExprUnary(Unary* expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
    case BANG:
        return !isTruthy(right);
    case MINUS:
        checkNumberOperand(expr->op, right);
        return - std::any_cast<float>(right);
    }

    return {};
}

std::any Interpreter::visitExprVariable(Variable* expr){
    return enviroment->get(expr->name);
}

std::any Interpreter::visitExprBinary(Binary* expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type){
    case GREATER:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) > std::any_cast<float> (right));
    case GREATER_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) >= std::any_cast<float> (right));
    case LESS:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) < std::any_cast<float> (right));
    case LESS_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) <= std::any_cast<float> (right));
    case MINUS:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) - std::any_cast<float> (right));
    case SLASH:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) / std::any_cast<float> (right));
    case STAR:
        checkNumberOperands(expr->op, left, right);
        return (std::any_cast<float> (left) * std::any_cast<float> (right));
    case PLUS:
        if (left.type() == typeid(float) && right.type() == typeid(float)){
            return (std::any_cast<float> (left) + std::any_cast<float> (right));
        }
        if (left.type() == typeid(std::string) && right.type() == typeid(std::string)){
            return (std::any_cast<std::string> (left) + std::any_cast<std::string> (right));
        }
        throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
    case EQUAL_EQUAL: 
        return isEqual(left, right);
    case BANG_EQUAL: 
        return !isEqual(left, right);

    }

    return {};
}

std::any Interpreter::visitExprCall(Call* expr){
    std::any callee = evaluate(expr->callee);

    std::vector<std::any> Arguments;
    for(size_t i = 0; i < expr->arguments.size(); i++){
        Arguments.push_back(evaluate(expr->arguments[i]));
    }

    if (callee.type() != typeid(WyrdCallable*)) {
        throw RuntimeError(expr->paren, "Can only call functions and classes.");
    }

    WyrdCallable* function = std::any_cast<WyrdCallable*>(callee);

    if(Arguments.size() != static_cast<size_t>(function->arity())){
        throw RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + 
                          " arguments but got " + std::to_string(Arguments.size()) + ".");
    }

    return function->call(*this, Arguments);
}

void Interpreter::checkNumberOperand(Token oper, std::any operand){
    if (operand.type() == typeid(float)) return;
    throw RuntimeError(oper, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token oper, std::any left, std::any right){
    if (left.type() == typeid(float) && right.type() == typeid(float)) return;
    throw RuntimeError(oper, "Operands must be a numbers.");
}

bool Interpreter::isTruthy(std::any object){
    if (!object.has_value()) return false;
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object);
    }
    return true;
}

bool Interpreter::isEqual(std::any a, std::any b){
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value() || !b.has_value()) return false;
    
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
        return true;
    }
    if (a.type() == typeid(nullptr)) return false;

    if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    if (a.type() == typeid(float) && b.type() == typeid(float)) {
        return std::any_cast<float>(a) == std::any_cast<float>(b);
    }
    if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
}

std::string Interpreter::stringify(std::any object){
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(float)) {
        std::string text = std::to_string(std::any_cast<float>(object));
        if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0') {
            text = text.substr(0, text.length() - 2);
        }
        return text;
    }

    if (object.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(object));
        if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0') {
            text = text.substr(0, text.length() - 2);
        }
        return text;
    }

    if (object.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(object));
    }

    if (object.type() == typeid(long)) {
        return std::to_string(std::any_cast<long>(object));
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }
    
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }

    if (object.type() == typeid(WyrdCallable*)) {
        return std::any_cast<WyrdCallable*>(object)->toString();
    }

    return "Error in stringify: object type not recognized.";
}