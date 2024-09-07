#include "input.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#define SRC_BUF_SIZE 1024

// lê arquivo, lembrando posicao do inicio das linhas
Src::Src(FILE *src_file) {
    char buf[SRC_BUF_SIZE];
    lines.push_back(0);
    for (;;) {
        size_t num_read_bytes =
            fread(&buf[0], sizeof(char), SRC_BUF_SIZE, src_file);

        if (num_read_bytes == 0) {
            lines.push_back(bytes.size());
            return;
        }

        for (size_t i = 0; i < num_read_bytes; i++) {
            if (buf[i] == '\n') {
                lines.push_back(i + 1);
            }
            bytes.push_back(buf[i]);
        }
    }
}

// span da linha `line`, _sem_ incluir '\n' no final
Span Src::line_span(size_t line) {
    Span span = {SIZE_MAX, SIZE_MAX, SIZE_MAX};
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i] > line) {
            span.first = lines[i - 1];
            span.second = lines[i] - 1;
            break;
        }
    }
    return span;
}
