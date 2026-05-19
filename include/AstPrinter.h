#pragma once

#include "Expr.h"

#include <any>
#include <string>
#include <sstream>

class AstPrinter: public ExprVisitor{
public:
    std::string print(Expr* expr){
        return std::any_cast<std::string>(expr->Accept(this));
    }

    std::any visitExprBinary(Binary* expr) override {
        return parenthesize(expr->op.lexeme, expr->left, expr->right);
    }

    std::any visitExprGrouping(Grouping* expr) override{
        return parenthesize("group", expr->expression);
    }

    std::any visitExprUnary(Unary* expr) override {
        return parenthesize(expr->op.lexeme, expr->right);
    }

    std::any visitExprLiteral(Literal* expr) override{
        auto& value_type = expr->value.type();

        if (value_type == typeid(nullptr)) {
            return std::string("nil");
        } else if (value_type == typeid(std::string)) {
            return std::any_cast<std::string>(expr->value);
        } else if (value_type == typeid(double)) {
            return std::to_string(std::any_cast<double>(expr->value));
        } else if (value_type == typeid(int)) {
            return std::to_string(std::any_cast<int>(expr->value));
        } else if (value_type == typeid(float)) {
            return std::to_string(std::any_cast<float>(expr->value));
        } else if (value_type == typeid(bool)) {
            return std::string(std::any_cast<bool>(expr->value) ? "true" : "false");
        }

        // Debug: print the actual type name
        return std::string("Error in visitLiteralExpr: literal type not recognized: " + std::string(value_type.name()));
    }

private:
    template <class... E>
    std::string parenthesize(std::string name, E*... expr){
        std::ostringstream builder;

        builder << "(" << name;
        ((builder << " " << print(expr)), ...);
        builder << ")";

        return builder.str();
    }
};