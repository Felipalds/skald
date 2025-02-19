#ifndef LEXER_H
#define LEXER_H
#include "lexer.h"
#include <unordered_map>

#define RULE_SHIFT 8

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
    NonTerm_None        = -1,
};
enum Rule {
    Rule_Skald              = 0,
    Rule_VarBlock           = 1,
    Rule_MainBlock          = 2,
    Rule_Decls_Decl         = 3,
    Rule_Decls_DeclDecls    = 4,
    Rule_Decl               = 5,
    Rule_Stmts_Stmt         = 6,
    Rule_Stmts_StmtStmts    = 7,
    Rule_Stmt_Stop          = 8,
    Rule_Stmt_Die           = 9,
    Rule_Stmt_If            = 10,
    Rule_Stmt_IfOr          = 11,
    Rule_Stmt_Loop          = 12,
    Rule_Stmt_Out           = 13,
    Rule_Stmt_Assign        = 14,
    Rule_Stmt_InId          = 15,
    Rule_Expr_Val           = 16,
    Rule_Expr_ValOpExpr     = 17,
    Rule_Val_ParExpr        = 18,
    Rule_Val_NotVal         = 19,
    Rule_Val_Lit            = 20,
    Rule_Val_Id             = 21,

    Rule_None = -1,
};
// clang-format on

const int RULE_LEN[22] = {
    3, // Rule_Skald
    3, // Rule_VarBlock
    3, // Rule_MainBlock
    1, // Rule_Decls_Decl
    2, // Rule_Decls_DeclDecls
    3, // Rule_Decl
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
    static NonTerm get_nonterm(Rule rule);
    void parse(std::vector<Token> &tokens);
};
#endif
