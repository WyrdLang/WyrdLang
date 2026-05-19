generateAST: compile
	./astGenerator ../include
	rm ./astGenerator

compile:
	g++ --std=c++17 -I ../include/ ../src/Utility.cpp GenerateAST.cpp -o astGenerator