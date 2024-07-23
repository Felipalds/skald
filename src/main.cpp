#include <cstdio>
#include "input.h"
#include "lexer.h"

void usage() {
    fprintf(stderr, "USAGE: skald /path/to/src.skald\n");
}

void no_such_file(const char *exe, const char *path) {
    fprintf(stderr, "%s: cannot open '%s': No such file or directory\n", exe, path);
}

int main(int argc, const char **argv) {
    if (argc <= 1) {
        usage();
        return 1;
    }

    const char *src_path = argv[1];
    FILE *src_file = fopen(src_path, "rb");
    if (src_file == NULL) {
        no_such_file(argv[0], src_path);
        return 1;
    }

    Src src(src_file);
    
    /* algo assim
     * LexResult lex_result = lex(src)
     * if (lex_result.is_err) ...
     * report_err(lex_result.err.span) ...
     */

    return 0;
}
