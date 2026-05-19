#pragma once
#include "Token.h"

#include <map>
#include <string>
#include <any>

class Enviroment{
private:
    std::map<std::string, std::any> values;
    Enviroment* enclosing;

public:
    Enviroment();
    Enviroment(Enviroment* enclosing);
    void define(std::string name, std::any value);
    std::any get(Token name);
    void assign(Token name, std::any value);
};