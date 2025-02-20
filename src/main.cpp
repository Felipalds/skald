#include "input.h"
#include "lexer.h"
#include "parser.h"
#include <cstdio>
#include <cstring>

void usage() {
    fprintf(stderr, "USAGE: skald /path/to/src.skald\n");
}

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

    src.print();
    printf("\n\n");

    for (Token token : lexer.tokens) {
        token.print(src);
        printf("\n");
    }
    printf("\n");
    bool lex_errors = !lexer.errors.empty();

    for (LexErr err : lexer.errors) {
        err.print(src);
    }
    printf("\n");

    if (lex_errors) {
        return 1;
    }

    Table table;
    Parser parser;
    parser.parse(lexer.tokens, table);
    bool parse_errors = !parser.errors.empty();

    if (parse_errors) {
        for (ParseErr err : parser.errors) {
            err.print(src, table);
        }
        return 1;
    }

    if (src_file != stdin) {
        fclose(src_file);
    }
    return 0;
}
