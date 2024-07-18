.PHONY: main

SRC=$(wildcard src/*.cpp)

main:
	gcc -g -o skald $(SRC)
