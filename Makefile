.PHONY: main test

CPP=src/lexer.cpp src/output.cpp

main:
	g++ -g -o skald src/main.cpp $(CPP)

test:
	g++ -g -o test src/tests.cpp $(CPP)
	./test

