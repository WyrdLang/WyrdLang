#include "Enviroment.h"
#include "RuntimeError.h"


Enviroment::Enviroment(){
    enclosing = NULL;
}

Enviroment::Enviroment(Enviroment* enclosing){
    this->enclosing = enclosing;
}


std::any Enviroment::get(Token name){
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
      return elem->second;
    }
    
    if(enclosing != NULL) return enclosing->get(name);

    throw RuntimeError(name,"Undefined variable '" + name.lexeme + "'.");
}

void Enviroment::assign(Token name, std::any value){
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
        elem->second = value;
        return;
    }

    if(enclosing != NULL){
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Enviroment::define(std::string name, std::any value){
    values[name] = std::move(value);
}

