
#include "WyrdLang.h"
#include "sysexits.h"
#include "Scanner.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool WyrdLang::hadError = false;

int main(int argc, char* argv[]){
    WyrdLang wyrdLang;

    if (argc > 2){
        std::cerr << "Usage: wyrd [script]" << std::endl;
        exit(EX_USAGE);
    }
    else if (argc == 2){
        wyrdLang.runFile(argv[1]);
    }
    else{
        wyrdLang.runPrompt();
    }

    return 0;
}   

void WyrdLang::runFile(std::string filePath){
    std::ifstream inputFile(filePath);
    std::string fileData;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(EX_NOINPUT);
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    
    fileData = buffer.str();

    std::cout << "File Loaded Successfully\n" << std::endl;

    run(fileData);
    
    if(hadError){
        exit(EX_DATAERR);
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
    Scanner scanner {source};
    std::vector<Token> tokens = scanner.scanTokens();

    for (int i = 0; i < tokens.size(); i++){
        std::cout << tokens[i].toString() << std::endl;
    }
}

void WyrdLang::error(int line, std::string message){
    report(line, "", message);
}

void WyrdLang::report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}