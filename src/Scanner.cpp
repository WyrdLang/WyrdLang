#include "Scanner.h"
#include "WyrdLang.h"
#include <iostream>
#include <any>

Scanner::Scanner(std::string _sourceData){
    source = _sourceData;
}

std::vector<Token> Scanner::scanTokens(){
    while (!isAtEnd()){
        start = current;
        scanToken();
    }

    tokens.push_back(Token(END_OF_FILE, "", std::any(), line));
    return tokens;
}

void Scanner::scanToken(){
    char character = advance();
    switch (character)
    {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
    
    case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
    case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
    case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
    case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;

    case '/':
        if(match('/')){
            while(peek() != '\n' && !isAtEnd()){
                advance();
            }
        }
        else{
            addToken(SLASH);
        }
    break;

    case ' ':
    case '\r':
    case '\t':
    break;

    case '"': string(); break;

    default:
        if(isDigit(character)){
            number();
        }
        else if (isAlpha(character)){
            identifier();
        }
        else{
            WyrdLang::error(line, "Unexpected character.");
        }
    break;
    }
}



// Overloaded Function addToken
void Scanner::addToken(TokenType _type){
    addToken(_type, std::any());
}

void Scanner::addToken(TokenType _type, std::any _literal){
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(_type, text, _literal, line));
}

bool Scanner::isAtEnd(){
    return current >= source.length();
}

bool Scanner::isAlpha(char _character){
    return (_character >= 'a' && _character <= 'z') ||
           (_character >= 'A' && _character <= 'Z') ||
            _character == '_';
}

bool Scanner::isDigit(char _character){
    return _character >= '0' && _character <= '9';
} 

bool Scanner::isAlphaNumeric(char _character){
    return isAlpha(_character) || isDigit(_character);
}

char Scanner::peek(){
    if(isAtEnd()){
        return '\0';
    }
    else{
        return source[current];
    }
}

char Scanner::peekNext(){
    if(current + 1 >= source.length()){
        return '\0';
    }
    else{
        return source[current +  1];
    }
}

char Scanner::advance(){
    return source[current++];
}

bool Scanner::match(char _expected){
    if(isAtEnd()){
        return false;
    }
    if(source[current] != _expected){
        return false;
    }
    current++;
    return true;
}

void Scanner::string(){
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()){
        WyrdLang::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string value = source.substr(start + 1, (current - 1) - (start + 1));
    addToken(STRING, value);
}

void Scanner::number(){
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())){
        advance();  // consume the '.'
        while (isDigit(peek())) advance();
    }

    std::string text = source.substr(start, current - start);
    float value = std::stof(text);
    addToken(NUMBER, value);
}

void Scanner::identifier(){
    while (isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current - start);

    TokenType type;
    std::unordered_map<std::string, TokenType>::iterator details = keywords.find(text);
    if (details != keywords.end()){
        type = details->second;
    }
    else{
        type = IDENTIFIER;
    }

    addToken(type);
}