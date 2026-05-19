#pragma once

#include "WyrdCallable.h"
#include "Enviroment.h"
#include "Stmt.h"

#include <vector>
#include <any>
#include <string>

class WyrdFunction : public WyrdCallable {
private:
    Function* declaration;
    Enviroment* closure;
    
public:
    WyrdFunction(Function* declaration, Enviroment* closure);
    
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    int arity() const override;
    std::string toString() const override;
};