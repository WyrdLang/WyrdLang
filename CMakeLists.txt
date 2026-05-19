cmake_minimum_required(VERSION 3.10)

project(WyrdLang VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(wyrd
    src/Enviroment.cpp
    src/Interpreter.cpp
    src/Parser.cpp
    src/Utility.cpp
    src/WyrdFunction.cpp
    src/WyrdLang.cpp
    src/Scanner.cpp
    src/Token.cpp
)

target_include_directories(wyrd PRIVATE include)