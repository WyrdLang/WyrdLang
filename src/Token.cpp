#include "Token.h"
#include <string>
#include <sstream>

Token::Token(TokenType _type, std::string _lexeme, std::any _literal, int _line){
    type = _type;
    lexeme = _lexeme;
    literal = _literal;
    line = _line;
}

std::string Token::toString() {
    std::ostringstream returnVal;
    returnVal << type << " " << lexeme << " ";
    
    if (!literal.has_value()) {
        returnVal << "null";
    } else if (literal.type() == typeid(std::string)) {
        returnVal << std::any_cast<std::string>(literal);
    } else if (literal.type() == typeid(double)) {
        returnVal << std::any_cast<double>(literal);
    } else if (literal.type() == typeid(float)) {
        returnVal << std::any_cast<float>(literal);
    } else if (literal.type() == typeid(int)) {
        returnVal << std::any_cast<int>(literal);
    } else if (literal.type() == typeid(bool)) {
        returnVal << (std::any_cast<bool>(literal) ? "true" : "false");
    } else {
        returnVal << "UnknownLiteralType";
    }

    return returnVal.str();
}