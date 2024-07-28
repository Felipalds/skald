#include "lexer.h"
#include <cstdio>
#include <cinttypes>

const char *op_rel_sym(OpRel op) {
    switch (op) {
        case Op_Less:
            return "<";
        case Op_LessEq:
            return "<=";
        case Op_Greater:
            return ">";
        case Op_GreaterEq:
            return ">=";
        case Op_Eq:
             return "=";
        case Op_Neq:
             return "!=";
        default:
             return "??";
    }
}

const char *op_arit_sym(OpArit op) {
    switch (op) {
        case Op_Add:
            return "+";
        case Op_Sub:
            return "-";
        case Op_Mul:
            return "*";
        case Op_Div:
            return "/";
        case Op_Mod:
             return "%";
        case Op_Pow:
             return "^";
        default:
             return "??";
    }
}

const char *op_logic_sym(OpLogic op) {
    switch (op) {
        case Op_And:
            return "&";
        case Op_Or:
            return "|";
        default:
             return "??";
    }
}

void Token::printf_fmt() {
    switch(kind) {
        case Tok_Var:
            printf("[var; ]");
            break;
        case Tok_Rav:
            printf("[rav; ]");
            break;
        case Tok_Main:
            printf("[main; ]");
            break;
        case Tok_Niam:
            printf("[niam; ]");
            break;
        case Tok_Loop:
            printf("[loop; ]");
            break;
        case Tok_Pool:
            printf("[pool; ]");
            break;
        case Tok_If:
            printf("[if; ]");
            break;
        case Tok_Or:
            printf("[or; ]");
            break;
        case Tok_Fi:
            printf("[fi; ]");
            break;
        case Tok_Do:
            printf("[do; ]");
            break;
        case Tok_In:
            printf("[in; ]");
            break;
        case Tok_Out:
            printf("[out; ]");
            break;
        case Tok_Stop:
            printf("[stop; ]");
            break;
        case Tok_Die:
            printf("[die; ]");
            break;
        case Tok_Ident:
            printf("[ident; (%zu..%zu)]", data.span.first, data.span.second);
            break;
        case Tok_Assign:
            printf("[<-; ]");
            break;
        case Tok_OpArit:
            printf("[arit; %s]", op_arit_sym(data.op_arit));
            break;
        case Tok_OpLogic:
            printf("[logic; %s]", op_logic_sym(data.op_logic));
            break;
        case Tok_OpUnaryNot:
            printf("[!; ]");
            break;
        case Tok_OpRel:
            printf("[rel; %s]", op_rel_sym(data.op_rel));
            break;
        case Tok_ParOpen:
            printf("[(; ]");
            break;
        case Tok_ParClose:
            printf("[); ]");
            break;
        case Tok_Period:
            printf("[.; ]");
            break;
        case Tok_LitStr:
            printf("[str; (%zu..%zu)]", data.span.first, data.span.second);
            break;
        case Tok_LitReal:
            printf("[real; %lf]", data.lit_real);
            break;
        case Tok_LitInt:
            printf("[int; %" PRId64 "]", data.lit_int);
            break;
        case Err:
            printf("[invalid token; ]");
            break;
        default:
            printf("[??; ]");
    }
}
