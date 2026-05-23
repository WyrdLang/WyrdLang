generateAST: compile
	./astGenerator ../include
	rm ./astGenerator

compile:
	g++ --std=c++17 -I ../include/ ../src/Utility.cpp ASTGenerator.cpp -o astGenerator