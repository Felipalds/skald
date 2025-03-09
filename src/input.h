#ifndef input_h
#define input_h

#include <cstdio>
#include <string>
#include <vector>

struct Span {
    size_t first, second, line;
};

struct Src {
    std::vector<char> bytes;
    std::vector<size_t> lines;

    // implementado em output.cpp
    void print();
    // implementado em output.cpp
    void print_span(Span span);

    Src(FILE *src_file);

    std::string get_lexeme(Span span);

    Span line_span(size_t line);
};

#endif
