#ifndef TABLE_H
#define TABLE_H
#include "lexer.h"
#include "parser.h"
#include <map>

class Table {
    std::map<std::pair<int, TokenKind>, int> table_push;
    std::map<std::pair<int, TokenKind>, Rule> table_reduce;
    std::map<std::pair<int, NonTerm>, int> table_goto;

  public:
    Table();

    bool accept(int state, TokenKind token);

    int push(int state, TokenKind token);

    Rule reduce(int state, TokenKind token);

    int goto_(int state, Rule rule);
};
#endif
