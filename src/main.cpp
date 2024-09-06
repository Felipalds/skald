#include "input.h"
#include "lexer.h"
#include <cstdio>

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
  FILE *src_file = fopen(src_path, "rb");
  if (src_file == NULL) {
    no_such_file(argv[0], src_path);
    return 1;
  }

  Src src(src_file);

  Lexer lexer(src);
  LexResult result = lexer.lex();

  for (Token token
     token.printf_fmt(src);
     printf("\n");
  }

  for (Error err : result.errors) {
      size_t line = src.which_line(static_cast<size_t>(err.index));
      printf("\033[31mError in position %lu (line %lu )\n\033[0m", err.index, line);
  }

  return 0;
}
