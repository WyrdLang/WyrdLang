#pragma once

#include <string>

class WyrdLang{
private:
    static bool hadError;
public:
    void runFile(std::string filePath);
    void runPrompt();
    void run(std::string source);
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
};