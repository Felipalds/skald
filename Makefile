.PHONY: main test

main:
	g++ -g -o skald src/*.cpp

test:
	g++ -g -o test tests/*.cpp src/!(main).cpp
	./test

