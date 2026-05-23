#include "Utility.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

std::vector<std::string> split(std::string source, std::string delimeter){
    std::vector<std::string> splittedOutput;
    
    size_t start = 0;
    size_t end = source.find(delimeter);

    while (end != std::string::npos){
        splittedOutput.push_back(source.substr(start, end - start));

        start = end + delimeter.length();
        end = source.find(delimeter, start);
    }
    splittedOutput.push_back(source.substr(start,end - start));

    return splittedOutput;
}

std::string trim(std::string source){

    size_t start = 0;

    // finding the first no space
    while(start < source.size() && (source[start] == ' ' || source[start] == '\t' || source[start] == '\n')){
        start++;
    }

    // all spaces
    if (start == source.size()){
        return ""; 
    }

    size_t end = source.size() - 1;

    // finding the last no space
    while(end > start && (source[end] == ' ' || source[end] == '\t' || source[end] == '\n')){
        end--;
    }

    return source.substr(start, end - start + 1);
}

std::string toLower(std::string source){
    std::string loweredString;
    
    for(int i = 0; i < source.length();i++){
        loweredString.push_back(std::tolower(source[i]));
    }
    
    return loweredString;
}