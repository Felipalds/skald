#ifndef input_h
#define input_h

#include <cstdio>
#include <vector>

#define SRC_BUF_SIZE 1024

struct Span {
    size_t first, second;
};

struct Src {
    std::vector<char> bytes;
    std::vector<size_t> lines;

    Src(FILE *src_file);
    Span which_line(size_t index, size_t &line);
};

 #endif
