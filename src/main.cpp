#include "input.h"
#include "lexer.h"
#include <cstdio>
#include <cstring>

void usage() { fprintf(stderr, "USAGE: skald /path/to/src.skald\n"); }

void no_such_file(const char *exe, const char *path) {
  fprintf(stderr, "%s: cannot open '%s': No such file or directory\n", exe,
          path);
}

int main(int argc, const char **argv) {
    if (argc <= 1) {
        usage();
        return 1;
    }

    const char *src_path = argv[1];

    FILE *src_file;
    if (strcmp(src_path, "-") == 0) {
        src_file = stdin;
    } else {
        src_file = fopen(src_path, "rb");
    }
    if (src_file == NULL) {
        no_such_file(argv[0], src_path);
        return 1;
    }

    Src src(src_file);

    Lexer lexer(src);

    for (Token token: lexer.tokens) {
        token.printf_fmt(src);
        printf("\n");
    }

    for (LexErr err: lexer.errors) {
        size_t line_num;
        Span line = src.which_line(err.span.first, line_num);

        printf("%zu: ", line_num);
        if (err.kind == LexErr_BadChar) {
            printf("BAD CHAR: ");
        } else if (err.kind == LexErr_UnknownOp) {
            printf("BAD OPER: ");
        }
        for (size_t i = line.first; i < line.second; i++) {
            if (i > err.span.second) {
                printf("\033[0m");
            } else if (i >= err.span.first) {
                printf("\033[31m");
            }
            printf("%c", src.bytes[i]);
        }
    }

    return 0;
}
