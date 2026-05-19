
#include "WyrdLang.h"
#include "sysexits.h"
#include "Scanner.h"
#include "Parser.h"
#include "Expr.h"
#include "AstPrinter.h"

#include <iostream>
#include <fstream>

bool WyrdLang::hadError = false;
bool WyrdLang::hadRuntimeError = false;

int main(int argc, char* argv[]){
    WyrdLang wyrd;
    if (argc > 2){
        std::cerr << "Usage: wyrd [script]" << std::endl;
        exit(EX_USAGE);
    }
    else if (argc == 2){
        wyrd.runFile(argv[1]);
    }
    else{
        wyrd.runPrompt();
    }

    return 0;
}   


void WyrdLang::runFile(std::string filePath){
    std::ifstream inputFile(filePath);
    
    if (!inputFile.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        exit(EX_NOINPUT);
    }
    
    std::string fileData;
    std::string line;

    while(getline(inputFile, line)){
        fileData += line + "\n";
    }
    
    std::cout << "File Loaded Successfully" << std::endl;
    run(fileData);
    
    if(hadError){
        exit(EX_DATAERR);
    }
    if (hadRuntimeError){
        exit(EX_SOFTWARE);
    }
}

void WyrdLang::runPrompt(){
    while (true) {
        std::cout << "> ";
        std::string command;
        if (!std::getline(std::cin, command)) break;

        run(command);
        hadError = false;
    }
}


void WyrdLang::run(std::string source){
    Scanner scammer {source};
    std::vector<Token> tokens = scammer.scanTokens();
    Parser parser {tokens};
    std::vector<Stmt*> statements = parser.parse();

    if(hadError) return;

    Interpreter interpreter {};
    interpreter.interpret(statements);
}

void WyrdLang::error(int line, std::string message){
    report(line, "", message);
}

void WyrdLang::report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

void WyrdLang::error(Token token, std::string message){
    if(token.type == TokenType::END_OF_FILE){
        report(token.line, " at end", message);
    }
    else{
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void WyrdLang::runtimeError(RuntimeError error){
    std::cerr << error.what() << "\n[line " << error.token.line << "]\n" << std::endl;
    hadRuntimeError = true;
}