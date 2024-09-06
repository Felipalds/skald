.PHONY: main test

CPP=src/lexer.cpp src/output.cpp src/input.cpp

main:
	g++ -g -Wall -Wextra -o skald src/main.cpp $(CPP)

test:
	g++ -g -Wall -Wextra -o test src/tests.cpp $(CPP)
	./test

