#include "lexer.h"

enum Rule {
    Rule_Skald,
    Rule_Decls,
    Rule_Decls_IdType,
    Rule_Stmts,
    Rule_Stmts_Stmt,
    Rule_Stmt_Stop,
    Rule_Stmt_Die,
    Rule_Stmt_If,
    Rule_Stmt_IfOr,
    Rule_Stmt_Loop,
    Rule_Stmt_Out,
    Rule_Stmt_Assign,
    Rule_Expr_Val,
    Rule_Expr_ValOpExpr,
    Rule_Val_ParExpr,
    Rule_Val_NotVal,
    Rule_Val_InId,
    Rule_Val_Lit,
    Rule_Val_Id,
};

const int RULE_LEN[] = {
    6, // Rule_Skald
    0, // Rule_Decls
    4, // Rule_Decls_IdType
    0, // Rule_Stmts
    2, // Rule_Stmts_Stmt
    2, // Rule_Stmt_Stop
    2, // Rule_Stmt_Die
    5, // Rule_Stmt_If
    7, // Rule_Stmt_IfOr
    3, // Rule_Stmt_Loop
    3, // Rule_Stmt_Out
    4, // Rule_Stmt_Assign
    1, // Rule_Expr_Val
    3, // Rule_Expr_ValOpExpr
    3, // Rule_Val_ParExpr
    2, // Rule_Val_NotVal
    2, // Rule_Val_InId
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

    StackElemData(Token _token) : token(_token) {}
    StackElemData(Rule _rule) : rule(_rule) {}
    StackElemData(int _state) : state(_state) {}
};

struct StackElem {
    StackElemKind kind;
    StackElemData data;

    StackElem(Token token) : kind(StackElem_Token), data(token) {}
    StackElem(Rule rule) : kind(StackElem_Rule), data(rule) {}
    StackElem(int state) : kind(StackElem_State), data(state) {}
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
    void parse(std::vector<Token> &tokens);
    void pop_reduce(Rule rule);
};
