#include "lexer.h"
#include "parser.h"
#include <cassert>

void ParseTable::tpush(int state, TokenKind tok, int dest) {
    table_push[{state, tok}] = dest;

    auto find = table_err_expect.find(state);
    if (find == table_err_expect.end()) {
        table_err_expect[state] = {tok};
    } else {
        std::vector<TokenKind> &tokens = find->second;
        tokens.push_back(tok);
    }
}

void ParseTable::treduce(int state, TokenKind tok, Rule rule) {
    table_reduce[{state, tok}] = rule;

    auto find = table_err_expect.find(state);
    if (find == table_err_expect.end()) {
        table_err_expect[state] = {tok};
    } else {
        std::vector<TokenKind> &tokens = find->second;
        tokens.push_back(tok);
    }
}

void ParseTable::terr(int state, Rule rule) {
    table_err_reduce[state] = rule;
}

void ParseTable::tgoto(int state, NonTerm nt, int dest) {
    table_goto[{state, nt}] = dest;
}

std::vector<TokenKind> &ParseTable::err_expect(int state) {
    auto find = table_err_expect.find(state);
    assert(find != table_err_expect.end());
    return find->second;
}

Rule ParseTable::err_reduce(int state) {
    auto find = table_err_reduce.find(state);
    if (find == table_err_reduce.end()) {
        return Rule_None;
    } else {
        return find->second;
    }
}

ParseTable::ParseTable() {
    tpush(0, Tok_Var, 3);
    tgoto(0, NonTerm_Skald, 1);
    tgoto(0, NonTerm_VarBlock, 2);

    // 1, Tok_Eof = Accept

    tpush(2, Tok_Main, 4);
    tgoto(2, NonTerm_MainBlock, 1);

    tpush(3, Tok_Ident, 7);
    tgoto(3, NonTerm_Decls, 5);
    tgoto(3, NonTerm_Decl, 6);
    terr(3, Rule_Decls_Decl);

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
    terr(5, Rule_VarBlock);

    tpush(6, Tok_Ident, 7);
    treduce(6, Tok_Rav, Rule_Decls_Decl);
    tgoto(6, NonTerm_Decls, 18);
    tgoto(6, NonTerm_Decl, 6);

    tpush(7, Tok_Type, 19);

    tpush(8, Tok_Niam, 20);
    terr(8, Rule_MainBlock);

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
    terr(9, Rule_Stmts_StmtStmts);

    tpush(10, Tok_Period, 22);
    terr(11, Rule_Stmt_Stop);

    tpush(11, Tok_Period, 23);
    terr(11, Rule_Stmt_Die);

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
    terr(16, Rule_Stmts_Stmt);

    treduce(17, Tok_Main, Rule_VarBlock);

    treduce(18, Tok_Rav, Rule_Decls_DeclDecls);

    tpush(19, Tok_Period, 29);
    terr(19, Rule_Decl);

    treduce(20, Tok_Eof, Rule_MainBlock);

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
    terr(24, Rule_Stmt_Out);

    tpush(25, Tok_Period, 31);
    terr(25, Rule_Stmt_InId);

    tpush(26, Tok_Not, 35);
    tpush(26, Tok_ParOpen, 34);
    tpush(26, Tok_Ident, 37);
    tpush(26, Tok_Lit, 36);
    tgoto(26, NonTerm_Expr, 32);
    tgoto(26, NonTerm_Val, 33);

    tpush(27, Tok_Do, 38);

    tpush(28, Tok_Pool, 39);
    terr(28, Rule_Stmt_Loop);

    treduce(29, Tok_Ident, Rule_Decl);
    treduce(29, Tok_Rav, Rule_Decl);

    treduce(30, Tok_Die, Rule_Stmt_Out);
    treduce(30, Tok_Fi, Rule_Stmt_Out);
    treduce(30, Tok_Ident, Rule_Stmt_Out);
    treduce(30, Tok_If, Rule_Stmt_Out);
    treduce(30, Tok_In, Rule_Stmt_Out);
    treduce(30, Tok_Loop, Rule_Stmt_Out);
    treduce(30, Tok_Niam, Rule_Stmt_Out);
    treduce(30, Tok_Or, Rule_Stmt_Out);
    treduce(30, Tok_Out, Rule_Stmt_Out);
    treduce(30, Tok_Pool, Rule_Stmt_Out);
    treduce(30, Tok_Stop, Rule_Stmt_Out);

    treduce(31, Tok_Die, Rule_Stmt_InId);
    treduce(31, Tok_Fi, Rule_Stmt_InId);
    treduce(31, Tok_Ident, Rule_Stmt_InId);
    treduce(31, Tok_If, Rule_Stmt_InId);
    treduce(31, Tok_In, Rule_Stmt_InId);
    treduce(31, Tok_Loop, Rule_Stmt_InId);
    treduce(31, Tok_Niam, Rule_Stmt_InId);
    treduce(31, Tok_Or, Rule_Stmt_InId);
    treduce(31, Tok_Out, Rule_Stmt_InId);
    treduce(31, Tok_Pool, Rule_Stmt_InId);
    treduce(31, Tok_Stop, Rule_Stmt_InId);

    tpush(32, Tok_Period, 40);
    terr(32, Rule_Stmt_Assign);

    treduce(33, Tok_ParClose, Rule_Expr_Val);
    tpush(33, Tok_Oper, 41);
    treduce(33, Tok_Period, Rule_Expr_Val);

    tpush(34, Tok_Not, 35);
    tpush(34, Tok_ParOpen, 34);
    tpush(34, Tok_Ident, 37);
    tpush(34, Tok_Lit, 36);
    tgoto(34, NonTerm_Expr, 42);
    tgoto(34, NonTerm_Val, 33);
    terr(34, Rule_Expr_Val);

    tpush(35, Tok_Not, 35);
    tpush(35, Tok_ParOpen, 34);
    tpush(35, Tok_Ident, 37);
    tpush(35, Tok_Lit, 36);
    tgoto(35, NonTerm_Val, 43);
    terr(35, Rule_Val_NotVal);

    treduce(36, Tok_ParClose, Rule_Val_Lit);
    treduce(36, Tok_Oper, Rule_Val_Lit);
    treduce(36, Tok_Period, Rule_Val_Lit);

    treduce(37, Tok_ParClose, Rule_Val_Id);
    treduce(37, Tok_Oper, Rule_Val_Id);
    treduce(37, Tok_Period, Rule_Val_Id);

    tpush(38, Tok_Die, 11);
    tpush(38, Tok_Ident, 14);
    tpush(38, Tok_If, 15);
    tpush(38, Tok_In, 13);
    tpush(38, Tok_Loop, 16);
    tpush(38, Tok_Out, 12);
    tpush(38, Tok_Stop, 10);
    tgoto(38, NonTerm_Stmts, 44);
    tgoto(38, NonTerm_Stmt, 9);

    treduce(39, Tok_Die, Rule_Stmt_Loop);
    treduce(39, Tok_Fi, Rule_Stmt_Loop);
    treduce(39, Tok_Ident, Rule_Stmt_Loop);
    treduce(39, Tok_If, Rule_Stmt_Loop);
    treduce(39, Tok_In, Rule_Stmt_Loop);
    treduce(39, Tok_Loop, Rule_Stmt_Loop);
    treduce(39, Tok_Niam, Rule_Stmt_Loop);
    treduce(39, Tok_Or, Rule_Stmt_Loop);
    treduce(39, Tok_Out, Rule_Stmt_Loop);
    treduce(39, Tok_Pool, Rule_Stmt_Loop);
    treduce(39, Tok_Stop, Rule_Stmt_Loop);

    treduce(40, Tok_Die, Rule_Stmt_Assign);
    treduce(40, Tok_Fi, Rule_Stmt_Assign);
    treduce(40, Tok_Ident, Rule_Stmt_Assign);
    treduce(40, Tok_If, Rule_Stmt_Assign);
    treduce(40, Tok_In, Rule_Stmt_Assign);
    treduce(40, Tok_Loop, Rule_Stmt_Assign);
    treduce(40, Tok_Niam, Rule_Stmt_Assign);
    treduce(40, Tok_Or, Rule_Stmt_Assign);
    treduce(40, Tok_Out, Rule_Stmt_Assign);
    treduce(40, Tok_Pool, Rule_Stmt_Assign);
    treduce(40, Tok_Stop, Rule_Stmt_Assign);

    tpush(41, Tok_Not, 35);
    tpush(41, Tok_ParOpen, 34);
    tpush(41, Tok_Ident, 37);
    tpush(41, Tok_Lit, 36);
    tgoto(41, NonTerm_Expr, 45);
    tgoto(41, NonTerm_Val, 33);
    terr(41, Rule_Expr_Val);

    tpush(42, Tok_ParClose, 46);

    treduce(43, Tok_ParClose, Rule_Val_NotVal);
    treduce(43, Tok_Oper, Rule_Val_NotVal);
    treduce(43, Tok_Period, Rule_Val_NotVal);

    tpush(44, Tok_Fi, 47);
    tpush(44, Tok_Or, 48);
    terr(44, Rule_Stmt_If);

    treduce(45, Tok_ParClose, Rule_Expr_ValOpExpr);
    treduce(45, Tok_Period, Rule_Expr_ValOpExpr);

    treduce(46, Tok_ParClose, Rule_Val_ParExpr);
    treduce(46, Tok_Oper, Rule_Val_ParExpr);
    treduce(46, Tok_Period, Rule_Val_ParExpr);

    treduce(47, Tok_Die, Rule_Stmt_If);
    treduce(47, Tok_Fi, Rule_Stmt_If);
    treduce(47, Tok_Ident, Rule_Stmt_If);
    treduce(47, Tok_If, Rule_Stmt_If);
    treduce(47, Tok_In, Rule_Stmt_If);
    treduce(47, Tok_Loop, Rule_Stmt_If);
    treduce(47, Tok_Niam, Rule_Stmt_If);
    treduce(47, Tok_Or, Rule_Stmt_If);
    treduce(47, Tok_Out, Rule_Stmt_If);
    treduce(47, Tok_Pool, Rule_Stmt_If);
    treduce(47, Tok_Stop, Rule_Stmt_If);

    tpush(48, Tok_Die, 11);
    tpush(48, Tok_Ident, 14);
    tpush(48, Tok_If, 15);
    tpush(48, Tok_In, 13);
    tpush(48, Tok_Loop, 16);
    tpush(48, Tok_Out, 12);
    tpush(48, Tok_Stop, 10);
    tgoto(48, NonTerm_Stmts, 49);
    tgoto(48, NonTerm_Stmt, 9);

    tpush(49, Tok_Fi, 50);
    terr(49, Rule_Stmt_IfOr);

    treduce(50, Tok_Die, Rule_Stmt_IfOr);
    treduce(50, Tok_Fi, Rule_Stmt_IfOr);
    treduce(50, Tok_Ident, Rule_Stmt_IfOr);
    treduce(50, Tok_If, Rule_Stmt_IfOr);
    treduce(50, Tok_In, Rule_Stmt_IfOr);
    treduce(50, Tok_Loop, Rule_Stmt_IfOr);
    treduce(50, Tok_Niam, Rule_Stmt_IfOr);
    treduce(50, Tok_Or, Rule_Stmt_IfOr);
    treduce(50, Tok_Out, Rule_Stmt_IfOr);
    treduce(50, Tok_Pool, Rule_Stmt_IfOr);
    treduce(50, Tok_Stop, Rule_Stmt_IfOr);
}

bool ParseTable::accept(int state, TokenKind token) {
    return state == 1 && token == Tok_Eof;
}

int ParseTable::push(int state, TokenKind token) {
    auto next_state = table_push.find({state, token});
    if (next_state == table_push.end()) {
        return -1;
    } else {
        return next_state->second;
    }
}

Rule ParseTable::reduce(int state, TokenKind token) {
    auto rule = table_reduce.find({state, token});
    if (rule == table_reduce.end()) {
        return Rule_None;
    } else {
        return rule->second;
    }
}

int ParseTable::goto_(int state, Rule rule) {
    NonTerm non_term = Parser::get_nonterm(rule);
    auto goto_state = table_goto.find({state, non_term});
    if (goto_state == table_goto.end()) {
        return -1;
    } else {
        return goto_state->second;
    }
}
