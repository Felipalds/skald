.PHONY: debug release

debug:
	g++ -D DEBUG -g -Wall -Wextra -Werror -o skald src/*.cpp

release:
	g++ -Wall -Wextra -O3 -o skald src/*.cpp
