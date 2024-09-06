#ifndef input_h
#define input_h

#include <cstdio>
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

    Span line_span(size_t line);
};

#endif
