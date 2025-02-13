#include "table.h"

Table::Table() {
    table_push[{0, Tok_Var}] = 2;
    table_goto[{0, Rule_Skald}] = 1;

    // {1, Tok_Eof} = Accept

    table_push[{2, Tok_Ident}] = 4;
    table_goto[{2, Rule_Decls}] = 3;
    table_goto[{2, Rule_Decls_IdType}] = 3;
    // GAMBIARRA
    table_reduce[{2, Tok_Rav}] = Rule_Decls;

    table_push[{3, Tok_Rav}] = 5;

    table_push[{4, Tok_Int}] = 6;
    table_push[{4, Tok_Real}] = 6;
    table_push[{4, Tok_Str}] = 6;

    table_push[{5, Tok_Main}] = 7;

    table_push[{6, Tok_Period}] = 8;

    table_push[{7, Tok_Die}] = 12;
    table_push[{7, Tok_Ident}] = 16;
    table_push[{7, Tok_If}] = 13;
    table_push[{7, Tok_Loop}] = 14;
    table_push[{7, Tok_Out}] = 15;
    table_push[{7, Tok_Stop}] = 11;
    table_goto[{7, Rule_Stmts}] = 9;
    table_goto[{7, Rule_Stmts_Stmt}] = 9;
    table_goto[{7, Rule_Stmt_If}] = 10;
    table_goto[{7, Rule_Stmt_Die}] = 10;
    table_goto[{7, Rule_Stmt_Out}] = 10;
    table_goto[{7, Rule_Stmt_IfOr}] = 10;
    table_goto[{7, Rule_Stmt_Loop}] = 10;
    table_goto[{7, Rule_Stmt_Stop}] = 10;
    table_goto[{7, Rule_Stmt_Assign}] = 10;
    // GAMBIARRA
    table_reduce[{7, Tok_Niam}] = Rule_Stmts;

    table_push[{8, Tok_Ident}] = 4;
    table_goto[{8, Rule_Decls}] = 17;
    table_goto[{8, Rule_Decls_IdType}] = 17;
    // GAMBIARRA
    table_reduce[{8, Tok_Rav}] = Rule_Decls;

    table_push[{9, Tok_Niam}] = 18;

    table_push[{10, Tok_Die}] = 12;
    table_push[{10, Tok_Ident}] = 16;
    table_push[{10, Tok_If}] = 13;
    table_push[{10, Tok_Loop}] = 14;
    table_push[{10, Tok_Out}] = 15;
    table_push[{10, Tok_Stop}] = 11;
    table_goto[{10, Rule_Stmts}] = 19;
    table_goto[{10, Rule_Stmts_Stmt}] = 19;
    table_goto[{10, Rule_Stmt_If}] = 10;
    table_goto[{10, Rule_Stmt_Die}] = 10;
    table_goto[{10, Rule_Stmt_Out}] = 10;
    table_goto[{10, Rule_Stmt_IfOr}] = 10;
    table_goto[{10, Rule_Stmt_Loop}] = 10;
    table_goto[{10, Rule_Stmt_Stop}] = 10;
    table_goto[{10, Rule_Stmt_Assign}] = 10;
    // GAMBIARRA
    table_reduce[{10, Tok_Rav}] = Rule_Decls;

    table_push[{11, Tok_Period}] = 20;

    table_push[{12, Tok_Period}] = 21;

    table_push[{13, Tok_Ident}] = 22;

    table_push[{14, Tok_Die}] = 12;
    table_push[{14, Tok_Ident}] = 16;
    table_push[{14, Tok_If}] = 13;
    table_push[{14, Tok_Loop}] = 14;
    table_push[{14, Tok_Out}] = 15;
    table_push[{14, Tok_Stop}] = 11;
    table_goto[{14, Rule_Stmts}] = 23;
    table_goto[{14, Rule_Stmts_Stmt}] = 23;
    table_goto[{14, Rule_Stmt_If}] = 10;
    table_goto[{14, Rule_Stmt_Die}] = 10;
    table_goto[{14, Rule_Stmt_Out}] = 10;
    table_goto[{14, Rule_Stmt_IfOr}] = 10;
    table_goto[{14, Rule_Stmt_Loop}] = 10;
    table_goto[{14, Rule_Stmt_Stop}] = 10;
    table_goto[{14, Rule_Stmt_Assign}] = 10;
    // GAMBIARRA
    table_reduce[{14, Tok_Pool}] = Rule_Stmts;

    table_push[{15, Tok_Ident}] = 24;

    table_push[{16, Tok_Assign}] = 25;

    table_reduce[{17, Tok_Rav}] = Rule_Decls_IdType;

    table_reduce[{18, Tok_Eof}] = Rule_Skald;

    table_reduce[{19, Tok_Fi}] = Rule_Stmts_Stmt;
    table_reduce[{19, Tok_Niam}] = Rule_Stmts_Stmt;
    table_reduce[{19, Tok_Or}] = Rule_Stmts_Stmt;
    table_reduce[{19, Tok_Pool}] = Rule_Stmts_Stmt;

    table_reduce[{20, Tok_Die}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Fi}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Ident}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_If}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Loop}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Niam}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Or}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Out}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Pool}] = Rule_Stmt_Stop;
    table_reduce[{20, Tok_Stop}] = Rule_Stmt_Stop;

    table_reduce[{21, Tok_Die}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Fi}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Ident}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_If}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Loop}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Niam}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Or}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Out}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Pool}] = Rule_Stmt_Die;
    table_reduce[{21, Tok_Stop}] = Rule_Stmt_Die;

    table_push[{22, Tok_Do}] = 26;

    table_push[{23, Tok_Pool}] = 27;

    table_push[{24, Tok_Period}] = 28;

    table_push[{25, Tok_Not}] = 32;
    table_push[{25, Tok_ParOpen}] = 31;
    table_push[{25, Tok_Ident}] = 35;
    table_push[{25, Tok_In}] = 33;
    table_push[{25, Tok_LitInt}] = 34;
    table_push[{25, Tok_LitStr}] = 34;
    table_push[{25, Tok_LitReal}] = 34;
    table_goto[{25, Rule_Expr_Val}] = 29;
    table_goto[{25, Rule_Expr_ValOpExpr}] = 29;
    table_goto[{25, Rule_Val_Id}] = 30;
    table_goto[{25, Rule_Val_Lit}] = 30;
    table_goto[{25, Rule_Val_InId}] = 30;
    table_goto[{25, Rule_Val_NotVal}] = 30;
    table_goto[{25, Rule_Val_ParExpr}] = 30;

    table_push[{26, Tok_Die}] = 12;
    table_push[{26, Tok_Ident}] = 16;
    table_push[{26, Tok_If}] = 13;
    table_push[{26, Tok_Loop}] = 14;
    table_push[{26, Tok_Out}] = 15;
    table_push[{26, Tok_Stop}] = 11;
    table_goto[{26, Rule_Stmts}] = 36;
    table_goto[{26, Rule_Stmts_Stmt}] = 36;
    table_goto[{26, Rule_Stmt_If}] = 10;
    table_goto[{26, Rule_Stmt_Die}] = 10;
    table_goto[{26, Rule_Stmt_Out}] = 10;
    table_goto[{26, Rule_Stmt_IfOr}] = 10;
    table_goto[{26, Rule_Stmt_Loop}] = 10;
    table_goto[{26, Rule_Stmt_Stop}] = 10;
    table_goto[{26, Rule_Stmt_Assign}] = 10;
    // GAMBIARRA
    table_reduce[{26, Tok_Pool}] = Rule_Stmts;

    table_reduce[{27, Tok_Die}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Ident}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_If}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Loop}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Niam}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Or}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Out}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Pool}] = Rule_Stmt_Loop;
    table_reduce[{27, Tok_Stop}] = Rule_Stmt_Loop;

    table_reduce[{28, Tok_Die}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Fi}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Ident}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_If}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Loop}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Niam}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Or}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Out}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Pool}] = Rule_Stmt_Out;
    table_reduce[{28, Tok_Stop}] = Rule_Stmt_Out;

    table_push[{29, Tok_Period}] = 37;

    table_reduce[{30, Tok_ParClose}] = Rule_Expr_Val;
    table_reduce[{30, Tok_Period}] = Rule_Expr_Val;
    table_push[{30, Tok_Oper}] = 38;

    table_push[{31, Tok_Not}] = 32;
    table_push[{31, Tok_ParOpen}] = 31;
    table_push[{31, Tok_Ident}] = 35;
    table_push[{31, Tok_In}] = 33;
    table_push[{31, Tok_LitReal}] = 34;
    table_push[{31, Tok_LitInt}] = 34;
    table_push[{31, Tok_LitStr}] = 34;
    table_goto[{31, Rule_Expr_Val}] = 39;
    table_goto[{31, Rule_Expr_ValOpExpr}] = 39;
    table_goto[{31, Rule_Val_Id}] = 30;
    table_goto[{31, Rule_Val_Lit}] = 30;
    table_goto[{31, Rule_Val_InId}] = 30;
    table_goto[{31, Rule_Val_NotVal}] = 30;
    table_goto[{31, Rule_Val_ParExpr}] = 30;

    table_push[{32, Tok_Not}] = 32;
    table_push[{32, Tok_ParOpen}] = 31;
    table_push[{32, Tok_Ident}] = 35;
    table_push[{32, Tok_In}] = 33;
    table_push[{32, Tok_LitReal}] = 34;
    table_push[{32, Tok_LitInt}] = 34;
    table_push[{32, Tok_LitStr}] = 34;
    table_goto[{32, Rule_Val_Id}] = 40;
    table_goto[{32, Rule_Val_Lit}] = 40;
    table_goto[{32, Rule_Val_InId}] = 40;
    table_goto[{32, Rule_Val_NotVal}] = 40;
    table_goto[{32, Rule_Val_ParExpr}] = 40;

    table_push[{33, Tok_Ident}] = 41;

    table_reduce[{34, Tok_ParClose}] = Rule_Val_Lit;
    table_reduce[{34, Tok_Period}] = Rule_Val_Lit;
    table_reduce[{34, Tok_Oper}] = Rule_Val_Lit;

    table_reduce[{35, Tok_ParClose}] = Rule_Val_Id;
    table_reduce[{35, Tok_Period}] = Rule_Val_Id;
    table_reduce[{35, Tok_Oper}] = Rule_Val_Id;

    table_push[{36, Tok_Fi}] = 42;
    table_push[{36, Tok_Or}] = 43;

    table_reduce[{37, Tok_Die}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Fi}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Ident}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_If}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Loop}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Niam}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Or}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Out}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Pool}] = Rule_Stmt_Assign;
    table_reduce[{37, Tok_Stop}] = Rule_Stmt_Assign;
    // falta GOTO para tabela 37 ?

    table_push[{38, Tok_Not}] = 32;
    table_push[{38, Tok_ParOpen}] = 31;
    table_push[{38, Tok_Ident}] = 35;
    table_push[{38, Tok_In}] = 33;
    table_push[{38, Tok_LitStr}] = 34;
    table_push[{38, Tok_LitInt}] = 34;
    table_push[{38, Tok_LitReal}] = 34;
    table_goto[{38, Rule_Expr_Val}] = 44;
    table_goto[{38, Rule_Expr_ValOpExpr}] = 44;
    table_goto[{38, Rule_Val_Id}] = 30;
    table_goto[{38, Rule_Val_Lit}] = 30;
    table_goto[{38, Rule_Val_InId}] = 30;
    table_goto[{38, Rule_Val_NotVal}] = 30;
    table_goto[{38, Rule_Val_ParExpr}] = 30;

    table_push[{39, Tok_ParClose}] = 45;

    table_reduce[{40, Tok_ParClose}] = Rule_Val_NotVal;
    table_reduce[{40, Tok_Period}] = Rule_Val_NotVal;
    table_reduce[{40, Tok_Oper}] = Rule_Val_NotVal;

    table_reduce[{41, Tok_ParClose}] = Rule_Val_InId;
    table_reduce[{41, Tok_Period}] = Rule_Val_InId;
    table_reduce[{41, Tok_Oper}] = Rule_Val_InId;

    table_reduce[{42, Tok_Die}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Fi}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Ident}] = Rule_Stmt_If;
    table_reduce[{42, Tok_If}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Loop}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Niam}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Or}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Out}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Pool}] = Rule_Stmt_If;
    table_reduce[{42, Tok_Stop}] = Rule_Stmt_If;

    table_push[{43, Tok_Die}] = 12;
    table_push[{43, Tok_Ident}] = 16;
    table_push[{43, Tok_If}] = 13;
    table_push[{43, Tok_Loop}] = 14;
    table_push[{43, Tok_Out}] = 15;
    table_push[{43, Tok_Stop}] = 11;
    table_goto[{43, Rule_Stmts}] = 46;
    table_goto[{43, Rule_Stmts_Stmt}] = 46;
    table_goto[{43, Rule_Stmt_If}] = 10;
    table_goto[{43, Rule_Stmt_Die}] = 10;
    table_goto[{43, Rule_Stmt_Out}] = 10;
    table_goto[{43, Rule_Stmt_IfOr}] = 10;
    table_goto[{43, Rule_Stmt_Loop}] = 10;
    table_goto[{43, Rule_Stmt_Stop}] = 10;
    table_goto[{43, Rule_Stmt_Assign}] = 10;
    // GAMBIARRA
    table_reduce[{43, Tok_Pool}] = Rule_Stmts;

    table_reduce[{44, Tok_ParClose}] = Rule_Expr_ValOpExpr;
    table_reduce[{44, Tok_Period}] = Rule_Expr_ValOpExpr;

    table_reduce[{45, Tok_ParClose}] = Rule_Val_ParExpr;
    table_reduce[{45, Tok_Period}] = Rule_Val_ParExpr;
    table_reduce[{45, Tok_Oper}] = Rule_Val_ParExpr;

    table_push[{46, Tok_Fi}] = 47;

    table_reduce[{47, Tok_Die}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Fi}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Ident}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_If}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Loop}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Niam}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Or}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Out}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Pool}] = Rule_Stmt_IfOr;
    table_reduce[{47, Tok_Stop}] = Rule_Stmt_IfOr;
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
    auto goto_state = table_goto.find({state, rule});
    if (goto_state == table_goto.end()) {
        return -1;
    } else {
        return goto_state->second;
    }
}
