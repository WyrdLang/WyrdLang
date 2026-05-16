#pragma once

#include "TokenType.h"
#include "Token.h"
#include <string>
#include <unordered_map>
#include <vector>

class Scanner{
private:
    std::unordered_map<std::string, TokenType> keywords = {
        {"together",   TOGETHER},
        {"clan", CLAN},
        {"otherwise",  OTHERWISE},
        {"nay", NAY},
        {"cycle",   CYCLE},
        {"spell",   SPELL},
        {"when",    WHEN},
        {"emptiness",   EMPTINESS},
        {"either",    EITHER},
        {"cast", CAST},
        {"manifest",MANIFEST},
        {"elder", ELDER},
        {"thine",  THINE},
        {"aye",  AYE},
        {"enchant",   ENCHANT},
        {"aslongas", ASLONGAS}
    };
    
    std::string source;
    std::vector<Token> tokens;
    int start {0};
    int current {0};
    int line {1};

    void scanToken();
    void addToken(TokenType _type);
    void addToken(TokenType _type, std::any _literal);
    bool isAtEnd();
    bool isAlpha(char _character);
    bool isDigit(char _character);
    bool isAlphaNumeric(char _character);
    char peek();
    char peekNext();
    char advance();
    bool match(char _expected);
    void identifier();
    void number();
    void string();

public:
    Scanner(std::string _sourceData);
    std::vector<Token> scanTokens();
    
};