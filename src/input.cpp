#include "input.h"
#include <cstdio>
#include <cinttypes>

Src::Src(FILE *src_file) {
    char buf[SRC_BUF_SIZE];
    lines.push_back(0);
    for (;;) {
        size_t read_bytes = fread(&buf[0], sizeof(char), SRC_BUF_SIZE, src_file);
        if (read_bytes == 0) {
            return;
        }
        for (size_t i = 0; i < read_bytes; i++) {
            if (buf[i] == '\n') {
                lines.push_back(i + 1);
            }
        }
        bytes.insert(bytes.end(), &buf[0], &buf[read_bytes]);
    }
}

size_t Src::which_line(size_t index) {
    for (size_t i = 0; i < lines.size(); i++) {
        if (index < lines[i]) {
            return i;
        }
    }
    return lines.size();
}
