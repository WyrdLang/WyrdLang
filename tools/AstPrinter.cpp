#include "AstPrinter.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Expr* expression = new Binary(
        new Unary(Token{MINUS, "-", std::any{}, 1}, new Literal(123.0)),
        Token{STAR, "*", std::any{}, 1},
        new Grouping(new Literal(45.67))
    );

    std::cout << AstPrinter{}.print(expression) << "\n";
    
    delete expression;
    
    return 0;
}