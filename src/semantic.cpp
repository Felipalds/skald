#include "parser.h"
#include <cassert>

SemAddr SemTable::register_new(std::string id, SemType type) {
    if (get_var(id).addr != ADDR_INVALID) {
        return ADDR_INVALID;
    }
    int addr = counter++;
    table[id] = {addr, type};
    return addr;
}

SemVar SemTable::get_var(std::string &id) {
    auto find = table.find(id);
    if (find == table.end()) {
        return {ADDR_INVALID, SemType_Int};
    }
    return {find->second};
}

SemAddr SemTable::new_tmp_var() {
    int addr = counter++;
    return addr;
}

SemLabel SemTable::new_label() {
    SemLabel label = label_counter++;
    return label;
}

SemType SemTable::type_from_tokdata(TokenData data) {
    switch (data) {
    case TokData_Type_Str:
        return SemType_Str;
    case TokData_Type_Int:
        return SemType_Int;
    case TokData_Type_Real:
        return SemType_Real;
    default:
        break;
    }
    assert(false);
    return SemType_Int;
}

StackElem SemTable::apply_rule(Rule rule, std::vector<StackElem> &stack,
                               Src &src) {
    SemData sem;

    switch (rule) {
    case Rule_Skald:
    case Rule_VarBlock:
    case Rule_Decls_Decl:
    case Rule_Decls_DeclDecls:
        break;
    case Rule_Decl: {
        // assert(not exists(id))
        Token tok_id = stack[stack.size() - 3].data.token;
        Token tok_type = stack[stack.size() - 2].data.token;
        std::string id = src.get_lexeme(tok_id.span);
        SemType type = type_from_tokdata(tok_type.data);
        register_new(id, type);
        break;
    }
    case Rule_MainBlock: {
        SemData stmts = stack[stack.size() - 2].sem_data;
        code_final = gen_stmts(stmts.stmts1);
        break;
    }
    case Rule_Stmts_Stmt: {
        SemData stmt = stack.back().sem_data;
        sem.stmts1 = {stmt};
        break;
    }
    case Rule_Stmts_StmtStmts: {
        SemData stmts1 = stack[stack.size() - 1].sem_data;
        SemData stmt = stack[stack.size() - 2].sem_data;

        sem.stmts1 = {stmt};
        sem.stmts1.insert(sem.stmts1.end(), stmts1.stmts1.begin(),
                          stmts1.stmts1.end());
        break;
    }
    case Rule_Stmt_Stop: {
        sem.br1 = LABEL_PENDING;
        sem.code = "goto LABEL_PENDING \t# needs backpatch\n";
        break;
    }
    case Rule_Stmt_Die: {
        sem.code = "halt\n";
        break;
    }
    case Rule_Stmt_If: {
        std::vector<SemData> &stmts =
            stack[stack.size() - 1 - 1].sem_data.stmts1;
        Token token = stack[stack.size() - 1 - 3].data.token;
        std::string lexeme = src.get_lexeme(token.span);
        SemVar var = get_var(lexeme);
        sem.addr = var.addr;
        sem.br1 = new_label(); // true branch
        sem.br2 = new_label(); // exit branch
        sem.kind = SemStmt_If;
        sem.stmts1 = stmts;
        /*sem.code = gen_if(sem.br1, sem.br2, stmts, var.addr);*/
        break;
    }
    case Rule_Stmt_IfOr: {
        std::vector<SemData> &stmts1 =
            stack[stack.size() - 1 - 3].sem_data.stmts1;
        std::vector<SemData> &stmts2 =
            stack[stack.size() - 1 - 1].sem_data.stmts1;
        Token token = stack[stack.size() - 1 - 5].data.token;
        std::string lexeme = src.get_lexeme(token.span);
        SemVar var = get_var(lexeme);
        sem.addr = var.addr;
        sem.br1 = new_label(); // true branch
        sem.br2 = new_label(); // false branch
        sem.br3 = new_label(); // exit branch
        sem.kind = SemStmt_IfOr;
        sem.stmts1 = stmts1;
        sem.stmts2 = stmts2;
        /*sem.code =*/
        /*    gen_if_or(sem.br1, sem.br2, sem.br3, stmts1, stmts2, var.addr);*/
        break;
    }
    case Rule_Stmt_Loop: {
        std::vector<SemData> &stmts =
            stack[stack.size() - 1 - 1].sem_data.stmts1;

        sem.br1 = new_label(); // loop entry
        sem.br2 = new_label(); // loop exit
        sem.kind = SemStmt_Loop;
        sem.stmts1 = stmts;
        /*sem.code = gen_loop(sem.br1, sem.br2, stmts);*/
        break;
    }
    case Rule_Stmt_Out: {
        Token token = stack[stack.size() - 2].data.token;
        std::string lexeme = src.get_lexeme(token.span);
        SemVar var = get_var(lexeme);
        /* checar existencia de var */
        sem.code = gen_output(var.addr, var.type);
        break;
    }
    case Rule_Stmt_Assign: {
        SemData expr = stack[stack.size() - 1 - 1].sem_data;
        Token dest = stack[stack.size() - 1 - 3].data.token;
        std::string lexeme = src.get_lexeme(dest.span);
        SemVar var = get_var(lexeme);
        /* checar existencia de var */
        /* checar tipo de var */
        sem.code = gen_precedence(expr.stack);
        sem.code += gen_assign_expr(var.addr, expr.addr);
        break;
    }
    case Rule_Stmt_InId: {
        Token token = stack[stack.size() - 2].data.token;
        std::string lexeme = src.get_lexeme(token.span);
        SemVar var = get_var(lexeme);
        /* checar existencia de var */
        sem.code = gen_input(var.addr, var.type);
        break;
    }
    case Rule_Expr_Val: {
        StackElem val = stack.back();
        sem.addr = val.sem_data.addr;
        sem.type = val.sem_data.type;
        sem.code = val.sem_data.code;
        sem.stack = {val};
        break;
    }
    case Rule_Expr_ValOpExpr: {
        StackElem val = stack[stack.size() - 1 - 2];
        StackElem op = stack[stack.size() - 1 - 1];
        StackElem expr = stack[stack.size() - 1 - 0];

        sem.type = val.sem_data.type;
        sem.addr = new_tmp_var();
        sem.stack = {val, op};
        sem.stack.insert(sem.stack.end(), expr.sem_data.stack.begin(),
                         expr.sem_data.stack.end());

        /*sem.code =*/
        /*    val.code + expr.code + gen_oper(sem.addr, val.addr, expr.addr,
         * op);*/
        break;
    }
    case Rule_Val_ParExpr: {
        SemData expr = stack[stack.size() - 1 - 1].sem_data;
        sem.addr = expr.addr;
        sem.type = expr.type;
        sem.code = gen_precedence(expr.stack);
        // PARSE PRECEDENCE HERE
        break;
    }
    case Rule_Val_NotVal: {
        SemData val = stack.back().sem_data;
        sem.addr = new_tmp_var();
        /* checar se type = int */
        sem.type = val.type;
        sem.code = val.code;
        sem.code += gen_assign_notval(sem.addr, val.addr);
        break;
    }
    case Rule_Val_Lit: {
        Token token = stack.back().data.token;
        std::string lexeme = src.get_lexeme(token.span);
        sem.addr = new_tmp_var();
        sem.type = type_from_tokdata(token.data);
        sem.code = gen_assign_lit(sem.addr, sem.type, lexeme);
        break;
    }
    case Rule_Val_Id: {
        Token token = stack.back().data.token;
        std::string lexeme = src.get_lexeme(token.span);
        SemVar var = get_var(lexeme);
        /* checar se existe if (var.addr == ADDR_INVALID) */
        sem.addr = var.addr;
        sem.type = var.type;
        break;
    }
    case Rule_None:
        break;
    }

    return StackElem(rule, sem);
}
