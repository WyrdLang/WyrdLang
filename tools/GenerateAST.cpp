/*

For stmt.h,
add the below line manually on top of the header file:

#include "Expr.h"

*/

#include "Utility.h"

#include <iostream>
#include <fstream>
#include <vector>

const std::string baseFileName = "Expr";
const std::string stmtFileName = "Stmt";

//forward decleration
void ASTDefinition(std::string outputPath, std::string baseFileName, std::vector<std::string> expressionTypes);
void visitorDefinition(std::ofstream& fileWriter, std::vector<std::string>& expressionTypes, std::string baseFileName);
void typeDefinition(std::ofstream& fileWriter, std::string expressionName, std::string expressionContent, std::string FileName);

int main(int argc, char* argv[]){
    // todo: input validation : should have atleast one parameter
    std::string outputPath = argv[1];
    std::vector<std::string> exprTypes;
    std::vector<std::string> stmtTypes;

    exprTypes = {
        "Assign   : Token name, Expr* value",
        "Binary   : Expr* left, Token op, Expr* right",
        "Call     : Expr* callee, Token paren, std::vector<Expr*> arguments",
        "Grouping : Expr* expression",
        "Literal  : std::any value",
        "Logical  : Expr* left, Token op, Expr* right",
        "Unary    : Token op, Expr* right",
        "Variable : Token name"
    };

    stmtTypes = {
        "Block      : std::vector<Stmt*> statements",
        "Expression : Expr* expression",
        "Function   : Token name, std::vector<Token> params, std::vector<Stmt*> body",
        "If         : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
        "Print      : Expr* expression",
        "Return     : Token keyword, Expr* value",
        "Var        : Token name, Expr* initializer",
        "While      : Expr* condition, Stmt* body"
    };

    ASTDefinition(outputPath, baseFileName, exprTypes);
    ASTDefinition(outputPath, stmtFileName, stmtTypes);
    std::cout << baseFileName << ".h & " << stmtFileName << ".h Generated Successfully!!!" << std::endl;
}


void ASTDefinition(std::string outputPath, std::string FileName, std::vector<std::string> expressionTypes){
    std::string outputFilePath = outputPath + "/" + FileName + ".h";
    std::ofstream fileWriter(outputFilePath);

    fileWriter << "#pragma once\n" << std::endl;
    fileWriter << "#include \"Token.h\"\n" << std::endl;
    fileWriter << "#include <vector>" << std::endl;
    fileWriter << "#include <any>\n" << std::endl;

    if (FileName == "Stmt"){
        fileWriter << "#include \"Expr.h\"\n" << std::endl;
    }

    for (int i = 0; i < expressionTypes.size(); i++){
        std::string exprClassName = trim(split(expressionTypes[i], ": ")[0]);

        fileWriter << "class " << exprClassName << ";" << std::endl;
    }
    fileWriter << std::endl;

    // visitor code
    visitorDefinition(fileWriter, expressionTypes, FileName);

    // The base class
    fileWriter << "class " << FileName << "{" << std::endl;
    fileWriter << "public:" << std::endl;
    fileWriter << "\tvirtual std::any Accept(" << FileName << "Visitor* visitor) = 0;" << std::endl;
    fileWriter << "\tvirtual ~" << FileName << "(){}" << std::endl;
    fileWriter << "};" << std::endl;
    fileWriter << std::endl;

    // different AST classes
    for(int i = 0; i < expressionTypes.size(); i++){
        std::string expressionName = trim(split(expressionTypes[i], ": ")[0]);
        std::string expressionContent = trim(split(expressionTypes[i], ": ")[1]);

        typeDefinition(fileWriter, expressionName, expressionContent, FileName);
    }    


    fileWriter.close();
}


void visitorDefinition(std::ofstream& fileWriter, std::vector<std::string>& expressionTypes, std::string FileName){
    fileWriter << "class " << FileName << "Visitor{" << std::endl;
    fileWriter << "public:" << std::endl;

    for (int i = 0; i < expressionTypes.size(); i++){
        std::string visitExprName = trim(split(expressionTypes[i], ": ")[0]);
        
        fileWriter << "\tvirtual std::any visit" << FileName << visitExprName;
        fileWriter << "(" << visitExprName << "* expr) = 0;" << std::endl;
    }

    fileWriter << "};" << std::endl;
    fileWriter << std::endl;
}

void typeDefinition(std::ofstream& fileWriter, std::string expressionName, std::string expressionContent, std::string FileName){
    fileWriter << "class " << expressionName << ": public " << FileName << "{" << std::endl;
    fileWriter << "public:" << std::endl;

    std::vector<std::string> fieldsContent = split(expressionContent, ", ");
    for (int i = 0; i < fieldsContent.size(); i++){
        fileWriter << "\t" << fieldsContent[i] << ";" << std::endl;
    }
    fileWriter << std::endl;

    fileWriter << "\t" << expressionName << "(" << expressionContent << "){" << std::endl;
    for (int i = 0; i < fieldsContent.size(); i++){
        std::string varName = split(fieldsContent[i], " ")[1];
        fileWriter << "\t\tthis->" << varName << " = " << varName << ";" << std::endl;
    }
    fileWriter << "\t}" << std::endl;
    fileWriter << std::endl;

    fileWriter << "\tstd::any Accept(" << FileName << "Visitor* visitor) override{" << std::endl;
    fileWriter << "\t\treturn visitor->visit" << FileName << expressionName;
    fileWriter << "(this);" << std::endl;
    fileWriter << "\t}" << std::endl; 

    fileWriter << "};" << std::endl;
    fileWriter << std::endl;
}