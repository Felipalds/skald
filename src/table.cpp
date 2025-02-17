#include "table.h"
#include "lexer.h"

#define tpush(state, tok, dest) table_push[{state, tok}] = dest;
#define tgoto(state, nt, dest) table_goto[{state, nt}] = dest;
#define treduce(state, tok, dest) table_reduce[{state, tok}] = dest;

Table::Table() {
    tpush(0, Tok_Var, 3);
    tgoto(0, NonTerm_Skald, 1);
    tgoto(0, NonTerm_VarBlock, 2);

    // 1, Tok_Eof = Accept
    
    tpush(2, Tok_Main, 4);

    tpush(3, Tok_Ident, 7);
    tgoto(3, NonTerm_Decls, 5);
    tgoto(3, NonTerm_Decl, 6);

    tpush(4, Tok_Die, 11);
    tpush(4, Tok_Ident, 14);
    tpush(4, Tok_If, 15);
    tpush(4, Tok_In, 13);
    tpush(4, Tok_Loop, 16);
    tpush(4, Tok_Out, 12);
    tpush(4, Tok_Stop, 10);
    tgoto(4, NonTerm_Stmts, 8);
    tgoto(4, NonTerm_Stmt, 9);

    tpush(5, Tok_Rav, 17);

    tpush(6, Tok_Ident, 7);
    treduce(6, Tok_Rav, Rule_Decls_Decl);
    tgoto(6, NonTerm_Decls, 18);
    tgoto(6, NonTerm_Decl, 6);

    tpush(7, Tok_Str, 19);
    tpush(7, Tok_Int, 19);
    tpush(7, Tok_Real, 19);

    tpush(8, Tok_Niam, 20);

    tpush(9, Tok_Die, 11);
    treduce(9, Tok_Fi, Rule_Stmts_Stmt);
    tpush(9, Tok_Ident, 14);
    tpush(9, Tok_If, 15);
    tpush(9, Tok_In, 13);
    tpush(9, Tok_Loop, 16);
    treduce(9, Tok_Niam, Rule_Stmts_Stmt);
    treduce(9, Tok_Or, Rule_Stmts_Stmt);
    tpush(9, Tok_Out, 12);
    treduce(9, Tok_Pool, Rule_Stmts_Stmt);
    tpush(9, Tok_Stop, 10);
    tgoto(9, NonTerm_Stmts, 21);
    tgoto(9, NonTerm_Stmt, 9);

    tpush(10, Tok_Period, 22);

    tpush(11, Tok_Period, 23);

    tpush(12, Tok_Ident, 24);

    tpush(13, Tok_Ident, 25);

    tpush(14, Tok_Assign, 26);

    tpush(15, Tok_Ident, 27);

    tpush(16, Tok_Die, 11);
    tpush(16, Tok_Ident, 14);
    tpush(16, Tok_If, 15);
    tpush(16, Tok_In, 13);
    tpush(16, Tok_Loop, 16);
    tpush(16, Tok_Out, 12);
    tpush(16, Tok_Stop, 10);
    tgoto(16, NonTerm_Stmts, 28);
    tgoto(16, NonTerm_Stmt, 9);

    treduce(17, Tok_Main, Rule_VarBlock);

    treduce(18, Tok_Rav, Rule_Decls_DeclDecls);

    tpush(19, Tok_Period, 29);

    treduce(20, Tok_Eof, Rule_Skald);

    treduce(21, Tok_Fi, Rule_Stmts_StmtStmts);
    treduce(21, Tok_Niam, Rule_Stmts_StmtStmts);
    treduce(21, Tok_Or, Rule_Stmts_StmtStmts);
    treduce(21, Tok_Pool, Rule_Stmts_StmtStmts);

    treduce(22, Tok_Die, Rule_Stmt_Stop);
    treduce(22, Tok_Fi, Rule_Stmt_Stop);
    treduce(22, Tok_Ident, Rule_Stmt_Stop);
    treduce(22, Tok_If, Rule_Stmt_Stop);
    treduce(22, Tok_In, Rule_Stmt_Stop);
    treduce(22, Tok_Loop, Rule_Stmt_Stop);
    treduce(22, Tok_Niam, Rule_Stmt_Stop);
    treduce(22, Tok_Or, Rule_Stmt_Stop);
    treduce(22, Tok_Out, Rule_Stmt_Stop);
    treduce(22, Tok_Pool, Rule_Stmt_Stop);
    treduce(22, Tok_Stop, Rule_Stmt_Stop);

    treduce(23, Tok_Die, Rule_Stmt_Die);
    treduce(23, Tok_Fi, Rule_Stmt_Die);
    treduce(23, Tok_Ident, Rule_Stmt_Die);
    treduce(23, Tok_If, Rule_Stmt_Die);
    treduce(23, Tok_In, Rule_Stmt_Die);
    treduce(23, Tok_Loop, Rule_Stmt_Die);
    treduce(23, Tok_Niam, Rule_Stmt_Die);
    treduce(23, Tok_Or, Rule_Stmt_Die);
    treduce(23, Tok_Out, Rule_Stmt_Die);
    treduce(23, Tok_Pool, Rule_Stmt_Die);
    treduce(23, Tok_Stop, Rule_Stmt_Die);

    tpush(24, Tok_Period, 30);

    tpush(25, Tok_Period, 31);

    tpush(26, Tok_Not, 35);
    tpush(26, Tok_ParOpen, 34);
    tpush(26, Tok_Ident, 37);
    tpush(26, Tok_LitInt, 36);
    tpush(26, Tok_LitStr, 36);
    tpush(26, Tok_LitReal, 36);
    tgoto(26, NonTerm_Expr, 32);
    tgoto(26, NonTerm_Val, 33);

    tpush(27, Tok_Do, 38);

    tpush(28, Tok_Pool, 39);

    treduce(29, Tok_Ident, Rule_Decl);
    treduce(29, Tok_Rav, Rule_Decl);

    treduce(30, )
}

bool Table::accept(int state, TokenKind token) {
    return state == 1 && token == Tok_Eof;
}

int Table::push(int state, TokenKind token) {
    auto next_state = table_push.find({state, token});
    if (next_state == table_push.end()) {
        return -1;
    } else {
        return next_state->second;
    }
}

Rule Table::reduce(int state, TokenKind token) {
    auto rule = table_reduce.find({state, token});
    if (rule == table_reduce.end()) {
        return Rule_None;
    } else {
        return rule->second;
    }
}

int Table::goto_(int state, Rule rule) {
    NonTerm non_term = NonTerm(rule >> RULE_SHIFT);
    auto goto_state = table_goto.find({state, non_term});
    if (goto_state == table_goto.end()) {
        return -1;
    } else {
        return goto_state->second;
    }
}
