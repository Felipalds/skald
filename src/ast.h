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
};

struct StackElem {
    StackElemKind kind;
    union {
        Token token;
        Rule rule;
    };
};
