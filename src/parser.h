#ifndef LEXER_H
#define LEXER_H
#include "lexer.h"

#define RULE_SHIFT 4

// clang-format off
enum NonTerm {
    NonTerm_Skald       = 1 << 0,
    NonTerm_VarBlock    = 1 << 1,
    NonTerm_MainBlock   = 1 << 2,
    NonTerm_Decls       = 1 << 3,
    NonTerm_Decl        = 1 << 4,
    NonTerm_Stmts       = 1 << 5,
    NonTerm_Stmt        = 1 << 6,
    NonTerm_Expr        = 1 << 7,
    NonTerm_Val         = 1 << 8,
};
// rule >> RULE_SHIFT retorna o nao terminal dono da regra
enum Rule {
    Rule_Skald              = NonTerm_Skald     << RULE_SHIFT | 1,

    Rule_VarBlock           = NonTerm_VarBlock  << RULE_SHIFT | 1,

    Rule_Decls_Decl         = NonTerm_Decls     << RULE_SHIFT | 1,
    Rule_Decls_DeclDecls    = NonTerm_Decls     << RULE_SHIFT | 2,

    Rule_Decl               = NonTerm_Decl      << RULE_SHIFT | 1,

    Rule_Stmts_Stmt         = NonTerm_Stmts     << RULE_SHIFT | 1,
    Rule_Stmts_StmtStmts    = NonTerm_Stmts     << RULE_SHIFT | 2,

    Rule_Stmt_Stop          = NonTerm_Stmt      << RULE_SHIFT | 1,
    Rule_Stmt_Die           = NonTerm_Stmt      << RULE_SHIFT | 2,
    Rule_Stmt_If            = NonTerm_Stmt      << RULE_SHIFT | 3,
    Rule_Stmt_IfOr          = NonTerm_Stmt      << RULE_SHIFT | 4,
    Rule_Stmt_Loop          = NonTerm_Stmt      << RULE_SHIFT | 5,
    Rule_Stmt_Out           = NonTerm_Stmt      << RULE_SHIFT | 6,
    Rule_Stmt_Assign        = NonTerm_Stmt      << RULE_SHIFT | 7,
    Rule_Stmt_InId          = NonTerm_Stmt      << RULE_SHIFT | 8,

    Rule_Expr_Val           = NonTerm_Expr      << RULE_SHIFT | 1,
    Rule_Expr_ValOpExpr     = NonTerm_Expr      << RULE_SHIFT | 2,

    Rule_Val_ParExpr        = NonTerm_Val       << RULE_SHIFT | 1,
    Rule_Val_NotVal         = NonTerm_Val       << RULE_SHIFT | 2,
    Rule_Val_Lit            = NonTerm_Val       << RULE_SHIFT | 3,
    Rule_Val_Id             = NonTerm_Val       << RULE_SHIFT | 4,

    Rule_None = -1,
};
// clang-format on

const int RULE_LEN[] = {
    4, // Rule_Skald
    2, // Rule_VarBlock_VarRav
    3, // Rule_VarBlock_VarDecRav
    3, // Rule_Decls_IdType
    4, // Rule_Decls_IdTypeDecls
    1, // Rule_Stmts_Stmt
    2, // Rule_Stmts_StmtStmts
    2, // Rule_Stmt_Stop
    2, // Rule_Stmt_Die
    5, // Rule_Stmt_If
    7, // Rule_Stmt_IfOr
    3, // Rule_Stmt_Loop
    3, // Rule_Stmt_Out
    4, // Rule_Stmt_Assign
    3, // Rule_Stmt_InId
    1, // Rule_Expr_Val
    3, // Rule_Expr_ValOpExpr
    3, // Rule_Val_ParExpr
    2, // Rule_Val_NotVal
    1, // Rule_Val_Lit
    1, // Rule_Val_Id
};

enum StackElemKind {
    StackElem_Token,
    StackElem_Rule,
    StackElem_State,
};

union StackElemData {
    Token token;
    Rule rule;
    int state;

    StackElemData(Token _token) : token(_token) {
    }
    StackElemData(Rule _rule) : rule(_rule) {
    }
    StackElemData(int _state) : state(_state) {
    }
};

struct StackElem {
    StackElemKind kind;
    StackElemData data;

    StackElem(Token token) : kind(StackElem_Token), data(token) {
    }
    StackElem(Rule rule) : kind(StackElem_Rule), data(rule) {
    }
    StackElem(int state) : kind(StackElem_State), data(state) {
    }

    void print();
};

enum ParseErrKind {};

struct ParseErr {
    ParseErrKind kind;
    Span span;
};

class Parser {
    std::vector<StackElem> stack;
    std::vector<ParseErr> errors;

    int state();
    void pop_reduce(Rule rule);

  public:
    void parse(std::vector<Token> &tokens);
};
#endif
