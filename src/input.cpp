#include "input.h"
#include <cstdio>
#include <cstdlib>

#define SRC_BUF_SIZE 1024

Src::Src(FILE *src_file) {
    char buf[SRC_BUF_SIZE];
    lines.push_back(0);
    printf("LINE:   1 | ");
    for (;;) {
        size_t read_bytes =
            fread(&buf[0], sizeof(char), SRC_BUF_SIZE, src_file);
        if (read_bytes == 0) {
            lines.push_back(bytes.size());
            printf("\n================================\n");
            return;
        }
        for (size_t i = 0; i < read_bytes; i++) {
            bool newline = false;
            if (buf[i] == '\n') {
                // nao adicionar indice -1 se arquivo começa com '\n'
                lines.push_back(i + 1);
                newline = true;
            }
            printf("%c", buf[i]);
            if (newline)
                printf("LINE: %3zu | ", lines.size());
            bytes.push_back(buf[i]);
        }
    }
}

// span da linha `line`, _sem_ incluir '\n' no final
Span Src::line_span(size_t line) {
    Span span = {0, 0, line};
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i] > line) {
            span.first = lines[i - 1];
            span.second = lines[i] - 1;
        }
    }
    return span;
}
