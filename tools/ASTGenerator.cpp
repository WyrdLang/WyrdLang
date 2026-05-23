#include "Utility.h"

#include <iostream>
#include <fstream>
#include <vector>

const std::string baseFileName = "Expr";

//forward decleration
void ASTDefinition(std::string outputPath, std::string baseFileName, std::vector<std::string> expressionTypes);
void visitorDefinition(std::ofstream& fileWriter, std::vector<std::string>& expressionTypes);
void typeDefinition(std::ofstream&, std::string expressionName, std::string expressionContent);

int main(int argc, char* argv[]){
    // todo: input validation : should have atleast one parameter
    std::string outputPath = argv[1];
    std::vector<std::string> exprTypes;
    exprTypes = {
        "Binary   : Expr* left, Token op, Expr* right",
        "Grouping : Expr* expression",
        "Literal  : std::any value",
        "Unary    : Token op, Expr* right"
    };

    ASTDefinition(outputPath, baseFileName, exprTypes);
    std::cout << baseFileName << ".h Generated Successfully!!!" << std::endl;
}


void ASTDefinition(std::string outputPath, std::string baseFileName, std::vector<std::string> expressionTypes){
    std::string outputFilePath = outputPath + "/" + baseFileName + ".h";
    std::ofstream fileWriter(outputFilePath);

    fileWriter << "#pragma once\n" << std::endl;
    fileWriter << "#include \"Token.h\"\n" << std::endl;
    fileWriter << "#include <any>\n" << std::endl;

    for (int i = 0; i < expressionTypes.size(); i++){
        std::string exprClassName = trim(split(expressionTypes[i], ": ")[0]);

        fileWriter << "class " << exprClassName << ";" << std::endl;
    }
    fileWriter << std::endl;

    // visitor code
    visitorDefinition(fileWriter, expressionTypes);

    // The base class
    fileWriter << "class " << baseFileName << "{" << std::endl;
    fileWriter << "public:" << std::endl;
    fileWriter << "\tvirtual std::any Accept(Visitor* visitor) = 0;" << std::endl;
    fileWriter << "\tvirtual ~" << baseFileName << "(){}" << std::endl;
    fileWriter << "};" << std::endl;
    fileWriter << std::endl;

    // different AST classes
    for(int i = 0; i < expressionTypes.size(); i++){
        std::string expressionName = trim(split(expressionTypes[i], ": ")[0]);
        std::string expressionContent = trim(split(expressionTypes[i], ": ")[1]);

        typeDefinition(fileWriter, expressionName, expressionContent);
    }    


    fileWriter.close();
}


void visitorDefinition(std::ofstream& fileWriter, std::vector<std::string>& expressionTypes){
    fileWriter << "class Visitor{" << std::endl;
    fileWriter << "public:" << std::endl;

    for (int i = 0; i < expressionTypes.size(); i++){
        std::string visitExprName = trim(split(expressionTypes[i], ": ")[0]);
        
        fileWriter << "\tvirtual std::any visit" << baseFileName << visitExprName;
        fileWriter << "(" << visitExprName << "* expr) = 0;" << std::endl;
    }

    fileWriter << "};" << std::endl;
    fileWriter << std::endl;
}

void typeDefinition(std::ofstream& fileWriter, std::string expressionName, std::string expressionContent){
    fileWriter << "class " << expressionName << ": public " << baseFileName << "{" << std::endl;
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

    fileWriter << "\tstd::any Accept(Visitor* visitor) override{" << std::endl;
    fileWriter << "\t\treturn visitor->visit" << baseFileName << expressionName;
    fileWriter << "(this);" << std::endl;
    fileWriter << "\t}" << std::endl; 

    fileWriter << "};" << std::endl;
    fileWriter << std::endl;
}