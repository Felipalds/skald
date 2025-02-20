#include "parser.h"
#include <cassert>
#include <vector>

int Parser::state() {
    assert(stack.size() > 0);
    assert(stack.back().kind == StackElem_State);
    return stack.back().data.state;
}

NonTerm Parser::get_nonterm(Rule rule) {
    switch (rule) {
    case Rule_Skald:
        return NonTerm_Skald;

    case Rule_VarBlock:
        return NonTerm_VarBlock;

    case Rule_MainBlock:
        return NonTerm_MainBlock;

    case Rule_Decls_Decl:
    case Rule_Decls_DeclDecls:
        return NonTerm_Decls;

    case Rule_Decl:
        return NonTerm_Decl;

    case Rule_Stmts_Stmt:
    case Rule_Stmts_StmtStmts:
        return NonTerm_Stmts;

    case Rule_Stmt_Stop:
    case Rule_Stmt_Die:
    case Rule_Stmt_If:
    case Rule_Stmt_IfOr:
    case Rule_Stmt_Loop:
    case Rule_Stmt_Out:
    case Rule_Stmt_Assign:
    case Rule_Stmt_InId:
        return NonTerm_Stmt;

    case Rule_Expr_Val:
    case Rule_Expr_ValOpExpr:
        return NonTerm_Expr;

    case Rule_Val_ParExpr:
    case Rule_Val_NotVal:
    case Rule_Val_Lit:
    case Rule_Val_Id:
        return NonTerm_Val;

    case Rule_None:
    default:
        return NonTerm_None;
    }
}

void Parser::pop_reduce(Rule rule) {
    int rule_len = RULE_LEN[rule];
    /*printf("len is %d\n", rule_len);*/
    /* análise semantica e geração de código */

    for (int i = 0; i < 2 * rule_len; i++) {
        stack.pop_back();
    }
}

void stack_print(int curr_state, TokenKind tok, std::vector<StackElem> &stack) {
    printf("(%d ", curr_state);
    TokenKind_print(tok);
    printf(")\t: ");
    for (StackElem elem : stack) {
        elem.print();
    }
    printf("\n\n");
}
void Parser::parse(std::vector<Token> &tokens, Table &table) {
    size_t ip = 0;
    stack.push_back({0});

    while (true) {
        int curr_state = state();
        Token token = tokens[ip];

        stack_print(curr_state, token.kind, stack);

        int push_state = table.push(curr_state, token.kind);
        if (push_state != -1) {
            stack.push_back({token});
            stack.push_back({push_state});
            ip++;
            continue;
        }

        Rule rule = table.reduce(curr_state, token.kind);
        if (rule != Rule_None) {
            pop_reduce(rule);
            int goto_ = table.goto_(state(), rule);
            assert(goto_ > 0);
            stack.push_back({rule});
            stack.push_back({goto_});
            continue;
        }

        if (table.accept(state(), token.kind)) {
            return;
        }

        // armazena erro, tabela estava vazia
        size_t prev_token = ip == 0 ? ip : ip - 1;
        errors.push_back({state(), tokens[prev_token], tokens[ip]});

        // tenta reduzir para detectar mais erros
        Rule err_reduce = table.err_reduce(state());
        if (err_reduce != Rule_None) {
            pop_reduce(err_reduce);
            continue;
        }

        // senão, somente desiste
        return;
    }
}
