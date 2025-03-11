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

std::string Src::get_lexeme(Span span) {
    if (span.first >= bytes.size() || span.second >= bytes.size()) {
        printf("[span OOB]");
        return {};
    }
    std::string s;
    for (size_t i = span.first; i <= span.second; i++) {
        s += bytes[i];
    }
    return s;
}

// span da linha `line`, _sem_ incluir '\n' no final
Span Src::line_span(size_t line) {
    if (line > lines.size() - 1) {
        return {SIZE_MAX, SIZE_MAX, SIZE_MAX};
    }
    size_t start = lines[line];
    size_t end = lines[line + 1] - 2;
    return {start, end, line};
}
