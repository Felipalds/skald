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

    StackElem(StackElemKind _kind, Token token) : kind(_kind), data(token) {}
    StackElem(StackElemKind _kind, Rule rule) : kind(_kind), data(rule) {}
    StackElem(StackElemKind _kind, int state) : kind(_kind), data(state) {}
};

enum ParseErrKind {};

struct ParseErr {
    ParseErrKind kind;
    Span span;
};

class Parser {
    std::vector<StackElem> stack;
    std::vector<ParseErr> errors;

    void parse(std::vector<Token> &tokens);
    void push_token(Token token);
    void push_state(int state);
    void push_rule(Rule rule);
    void pop_reduce(Rule rule);
};
