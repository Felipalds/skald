.PHONY: main

main:
	g++ -g -Wall -Wextra -o skald src/*.cpp

fast:
	g++ -Wall -Wextra -O3 -o skald src/*.cpp
