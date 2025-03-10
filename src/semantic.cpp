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

SemOper SemTable::oper_from_tokdata(TokenData data) {
    switch (data) {
    case TokData_Op_Add:
        return SemOp_Add;
    case TokData_Op_Sub:
        return SemOp_Sub;
    case TokData_Op_Mul:
        return SemOp_Mul;
    case TokData_Op_Div:
        return SemOp_Div;
    case TokData_Op_Mod:
        return SemOp_Mod;
    case TokData_Op_Pow:
        return SemOp_Pow;
    case TokData_Op_Less:
        return SemOp_Less;
    case TokData_Op_LessEq:
        return SemOp_LessEq;
    case TokData_Op_Greater:
        return SemOp_Greater;
    case TokData_Op_GreaterEq:
        return SemOp_GreaterEq;
    case TokData_Op_Eq:
        return SemOp_Eq;
    case TokData_Op_Neq:
        return SemOp_Neq;
    case TokData_Op_And:
        return SemOp_And;
    case TokData_Op_Or:
        return SemOp_Or;
    default:
        assert(false);
        return SemOp_Add;
    }
}

bool SemTable::oper_compatible(SemOper op, SemType type) {
    if (type == SemType_Str) {
        return false;
    }
    switch (op) {
    case SemOp_Add:
    case SemOp_Sub:
    case SemOp_Mul:
    case SemOp_Div:
    case SemOp_Pow:
    case SemOp_Less:
    case SemOp_LessEq:
    case SemOp_Greater:
    case SemOp_GreaterEq:
    case SemOp_Eq:
    case SemOp_Neq:
        return true;
    case SemOp_Mod:
    case SemOp_And:
    case SemOp_Or:
        return type == SemType_Int;
    default:
        assert(false);
        return false;
    }
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
        Token tok_period = stack[stack.size() - 1].data.token;
        std::string id = src.get_lexeme(tok_id.span);
        SemType type = type_from_tokdata(tok_type.data);

        sem.span.line = tok_id.span.line;
        sem.span.first = tok_id.span.first;
        sem.span.second = tok_period.span.second;
        SemAddr new_addr = register_new(id, type);

        if (new_addr == ADDR_INVALID) {
            SemErr error = {SemErr_DeclRedefineVar, sem.span};
            errors.push_back(error);
        }

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
        sem.span = stmt.span;
        break;
    }
    case Rule_Stmts_StmtStmts: {
        SemData stmts1 = stack[stack.size() - 1].sem_data;
        SemData stmt = stack[stack.size() - 2].sem_data;

        sem.stmts1 = {stmt};
        sem.stmts1.insert(sem.stmts1.end(), stmts1.stmts1.begin(),
                          stmts1.stmts1.end());
        sem.span.line = stmt.span.line;
        sem.span.first = stmt.span.first;
        sem.span.second = stmts1.stmts1.back().span.second;
        break;
    }
    case Rule_Stmt_Stop: {
        Token tok_stop = stack[stack.size() - 2].data.token;
        Token tok_period = stack[stack.size() - 1].data.token;

        sem.br1 = LABEL_PENDING;
        sem.code = "goto LABEL_PENDING \t# needs backpatch\n";
        sem.span.line = tok_stop.span.line;
        sem.span.first = tok_stop.span.first;
        sem.span.second = tok_period.span.second;
        break;
    }
    case Rule_Stmt_Die: {
        Token tok_die = stack[stack.size() - 2].data.token;
        Token tok_period = stack[stack.size() - 1].data.token;

        sem.code = "halt\n";
        sem.span.line = tok_die.span.line;
        sem.span.first = tok_die.span.first;
        sem.span.second = tok_period.span.second;
        break;
    }
    case Rule_Stmt_If: {
        Token tok_if = stack[stack.size() - 5].data.token;
        Token tok_id = stack[stack.size() - 4].data.token;
        /*Token tok_do = stack[stack.size() - 3].data.token;*/
        std::vector<SemData> &stmts = stack[stack.size() - 2].sem_data.stmts1;
        Token tok_fi = stack[stack.size() - 1].data.token;
        std::string lexeme = src.get_lexeme(tok_id.span);

        sem.span.line = tok_if.span.line;
        sem.span.first = tok_if.span.first;
        sem.span.second = tok_fi.span.second;

        SemVar var = get_var(lexeme);
        if (var.type != SemType_Int) {
            SemErr error = {
                SemErr_IfTypeNotInt,
                sem.span,
            };
            errors.push_back(error);
        }

        sem.addr = var.addr;
        sem.br1 = new_label(); // true branch
        sem.br2 = new_label(); // exit branch
        sem.kind = SemStmt_If;
        sem.stmts1 = stmts;
        break;
    }
    case Rule_Stmt_IfOr: {
        Token tok_if = stack[stack.size() - 7].data.token;
        Token tok_id = stack[stack.size() - 6].data.token;
        // Token tok_do = stack[stack.size() - 5].data.token;
        std::vector<SemData> &stmts1 = stack[stack.size() - 4].sem_data.stmts1;
        // Token tok_or = stack[stack.size() - 3].data.token;
        std::vector<SemData> &stmts2 = stack[stack.size() - 2].sem_data.stmts1;
        Token tok_fi = stack[stack.size() - 1].data.token;
        std::string lexeme = src.get_lexeme(tok_id.span);

        sem.span.line = tok_if.span.line;
        sem.span.first = tok_if.span.first;
        sem.span.second = tok_fi.span.second;

        SemVar var = get_var(lexeme);
        if (var.type != SemType_Int) {
            SemErr error = {
                SemErr_IfTypeNotInt,
                sem.span,
            };
            errors.push_back(error);
        }

        sem.addr = var.addr;
        sem.br1 = new_label(); // true branch
        sem.br2 = new_label(); // false branch
        sem.br3 = new_label(); // exit branch
        sem.kind = SemStmt_IfOr;
        sem.stmts1 = stmts1;
        sem.stmts2 = stmts2;

        break;
    }
    case Rule_Stmt_Loop: {
        Token tok_loop = stack[stack.size() - 3].data.token;
        std::vector<SemData> &stmts = stack[stack.size() - 2].sem_data.stmts1;
        Token tok_pool = stack[stack.size() - 1].data.token;

        sem.br1 = new_label(); // loop entry
        sem.br2 = new_label(); // loop exit
        sem.kind = SemStmt_Loop;
        sem.stmts1 = stmts;
        sem.span.line = tok_loop.span.line;
        sem.span.first = tok_loop.span.first;
        sem.span.second = tok_pool.span.second;
        break;
    }
    case Rule_Stmt_Out: {
        Token tok_out = stack[stack.size() - 3].data.token;
        Token tok_id = stack[stack.size() - 2].data.token;
        Token tok_period = stack[stack.size() - 1].data.token;

        std::string lexeme = src.get_lexeme(tok_id.span);
        SemVar var = get_var(lexeme);
        sem.span.line = tok_out.span.line;
        sem.span.first = tok_out.span.first;
        sem.span.second = tok_period.span.second;
        sem.code = gen_output(var.addr, var.type);
        break;
    }
    case Rule_Stmt_Assign: {
        Token tok_dest = stack[stack.size() - 4].data.token;
        /*Token tok_assign = stack[stack.size() - 3].data.token;*/
        SemData expr = stack[stack.size() - 2].sem_data;
        Token tok_period = stack[stack.size() - 1].data.token;

        sem.span.line = tok_dest.span.line;
        sem.span.first = tok_dest.span.first;
        sem.span.second = tok_period.span.second;

        std::string lexeme = src.get_lexeme(tok_dest.span);
        SemVar var = get_var(lexeme);
        if (var.type != expr.type) {
            SemErr error = {
                SemErr_AssignTypeMismatch,
                sem.span,
            };
            errors.push_back(error);
        }

        sem.code = gen_expr(expr.addr, expr.stack);
        sem.code += gen_assign_expr(var.addr, expr.addr);
        break;
    }
    case Rule_Stmt_InId: {
        Token tok_in = stack[stack.size() - 3].data.token;
        Token tok_id = stack[stack.size() - 2].data.token;
        Token tok_period = stack[stack.size() - 1].data.token;
        std::string lexeme = src.get_lexeme(tok_id.span);
        SemVar var = get_var(lexeme);

        sem.span.line = tok_in.span.line;
        sem.span.first = tok_in.span.first;
        sem.span.second = tok_period.span.second;
        sem.code = gen_input(var.addr, var.type);
        break;
    }
    case Rule_Expr_Val: {
        StackElem val = stack.back();
        sem.addr = val.sem_data.addr;
        sem.type = val.sem_data.type;
        sem.code = val.sem_data.code;
        sem.span = val.sem_data.span;
        sem.stack = {val};
        break;
    }
    case Rule_Expr_ValOpExpr: {
        StackElem val = stack[stack.size() - 3];
        StackElem op = stack[stack.size() - 2];
        SemData expr = stack[stack.size() - 1].sem_data;

        sem.span.line = val.sem_data.span.line;
        sem.span.first = val.sem_data.span.first;
        sem.span.second = expr.span.second;

        if (val.sem_data.type != expr.type) {
            SemErr error = {SemErr_OperTypeMismatch, sem.span};
            errors.push_back(error);
        } else if (!oper_compatible(oper_from_tokdata(op.data.token.data),
                                    expr.type)) {
            SemErr error = {SemErr_OperTypeIncompatible, sem.span};
            errors.push_back(error);
        }

        sem.type = val.sem_data.type;
        sem.addr = new_tmp_var();
        sem.stack = {val, op};
        sem.stack.insert(sem.stack.end(), expr.stack.begin(), expr.stack.end());
        break;
    }
    case Rule_Val_ParExpr: {
        SemData expr = stack[stack.size() - 1 - 1].sem_data;
        Token par_open = stack[stack.size() - 1 - 2].data.token;
        Token par_close = stack[stack.size() - 1].data.token;

        sem.span.line = par_open.span.line;
        sem.span.first = par_open.span.first;
        sem.span.second = par_close.span.second;
        sem.addr = expr.addr;
        sem.type = expr.type;
        sem.code = gen_expr(expr.addr, expr.stack);
        break;
    }
    case Rule_Val_NotVal: {
        Token tok_not = stack[stack.size() - 2].data.token;
        SemData val = stack[stack.size() - 1].sem_data;

        sem.addr = new_tmp_var();
        sem.type = val.type;
        sem.span.line = tok_not.span.line;
        sem.span.first = tok_not.span.first;
        sem.span.second = val.span.second;

        if (val.type != SemType_Int) {
            SemErr error = {
                SemErr_NegateNotInt,
                sem.span,
            };
            errors.push_back(error);
        }

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
        sem.span = token.span;
        break;
    }
    case Rule_Val_Id: {
        Token token = stack.back().data.token;
        std::string lexeme = src.get_lexeme(token.span);
        sem.span = token.span;
        SemVar var = get_var(lexeme);
        if (var.addr == ADDR_INVALID) {
            SemErr error = {
                SemErr_UndefinedVar,
                sem.span,
            };
            errors.push_back(error);
        }
        sem.addr = var.addr;
        sem.type = var.type;
        break;
    }
    case Rule_None:
        break;
    }

    return StackElem(rule, sem);
}
