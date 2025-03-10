#include "parser.h"
#include <cassert>
#include <cstdio>
#include <deque>
#include <memory>
#include <stdexcept>
#include <string>

template <typename... Args>
std::string string_format(const char *format, Args... args) {
    int size_s =
        std::snprintf(nullptr, 0, format, args...) + 1; // Extra space for '\0'
    if (size_s <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format, args...);
    return std::string(buf.get(),
                       buf.get() + size - 1); // We don't want the '\0' inside
}
// https://stackoverflow.com/a/26221725
// The code snippet above is licensed under CC0 1.0.
// Modificado.

std::string SemTable::gen_assign_lit(SemAddr addr, SemType type,
                                     std::string &lexeme) {
    switch (type) {
    case SemType_Int: {
        return string_format("t%d: i32 = %s\n", addr, lexeme.c_str());
    }
    case SemType_Real: {
        return string_format("t%d: double = %s\n", addr, lexeme.c_str());
    }
    case SemType_Str: {
        return string_format("t%d: str = %s\n", addr, lexeme.c_str());
        break;
    }
    default:
        assert(false);
        break;
    }
}

std::string SemTable::gen_assign_notval(SemAddr dest, SemAddr src) {
    return string_format("t%d: bool = not t%d\n", dest, src);
}

const char *get_op_string(TokenData op) {
    switch (op) {
    case TokData_Op_Add:
        return "+";
    case TokData_Op_Sub:
        return "-";
    case TokData_Op_Mul:
        return "*";
    case TokData_Op_Div:
        return "/";
    case TokData_Op_Mod:
        return "%";
    case TokData_Op_Pow:
        return "^";
    case TokData_Op_Less:
        return "<";
    case TokData_Op_LessEq:
        return "<=";
    case TokData_Op_Greater:
        return ">";
    case TokData_Op_GreaterEq:
        return ">=";
    case TokData_Op_Eq:
        return "==";
    case TokData_Op_Neq:
        return "!=";
    case TokData_Op_And:
        return "and";
    case TokData_Op_Or:
        return "or";
    default:
        return "???";
    }
}

std::string SemTable::gen_oper(SemAddr dest, SemAddr left, SemAddr right,
                               TokenData op) {

    const char *op_string = get_op_string(op);
    return string_format("t%d := t%d %s t%d\n", dest, left, op_string, right);
}

std::string SemTable::gen_input(SemAddr dest, SemType type) {
    switch (type) {
    case SemType_Int:
        return string_format("t%d: i32 = call read_int()\n", dest);
    case SemType_Real:
        return string_format("t%d: double = call read_double()\n", dest);
    case SemType_Str:
        return string_format("t%d: str = call read_string()\n", dest);
    default:
        assert(false);
        return {};
    }
}

std::string SemTable::gen_output(SemAddr src, SemType type) {
    switch (type) {
    case SemType_Int:
        return string_format("call write_int(t%d)\n", src);
    case SemType_Real:
        return string_format("call write_double(t%d)\n", src);
    case SemType_Str:
        return string_format("call write_string(t%d)\n", src);
    default:
        assert(false);
        return {};
    }
}

std::string SemTable::gen_assign_expr(SemAddr dest, SemAddr src) {
    return string_format("t%d := t%d\n", dest, src);
}

std::string SemTable::gen_if_or(SemLabel true_branch, SemLabel false_branch,
                                SemLabel exit, std::vector<SemData> &body,
                                std::vector<SemData> &body_false,
                                SemAddr cond) {
    std::string code;
    SemAddr cond_bool = new_tmp_var();
    code += string_format("t%d: bool = t%d != 0\n", cond_bool, cond);
    code += string_format("if t%d goto label%d \t\t\t# is true\n", cond_bool,
                          true_branch);
    code += string_format("goto label%d \t\t\t# is false\n", false_branch);
    code += string_format("label%d: \t\t\t# true branch\n", true_branch);
    code += gen_stmts(body);
    code += string_format("goto label%d \t\t\t# skip false branch", exit);
    code += string_format("label%d \t\t\t# false branch\n", false_branch);
    code += gen_stmts(body_false);
    code += string_format("label%d: \t\t\t# if-or exit\n", exit);
    return code;
}

std::string SemTable::gen_if(SemLabel true_branch, SemLabel exit,
                             std::vector<SemData> &body, SemAddr cond) {
    std::string code;
    SemAddr cond_bool = new_tmp_var();
    code += string_format("t%d: bool = t%d != 0\n", cond_bool, cond);
    code += string_format("if t%d goto label%d \t\t\t# is true\n", cond_bool,
                          true_branch);
    code += string_format("goto label%d \t\t\t# is false\n", exit);
    code += string_format("label%d: \t\t\t# true branch\n", true_branch);
    code += gen_stmts(body);
    code += string_format("label%d: \t\t\t# if exit\n", exit);
    return code;
}

std::string SemTable::gen_stmts(std::vector<SemData> &stmts) {
    std::string code;
    for (SemData &stmt : stmts) {
        switch (stmt.kind) {
        case SemStmt_Line:
            code += stmt.code;
            break;
        case SemStmt_If:
            code += gen_if(stmt.br1, stmt.br2, stmt.stmts1, stmt.addr);
            break;
        case SemStmt_IfOr:
            code += gen_if_or(stmt.br1, stmt.br2, stmt.br3, stmt.stmts1,
                              stmt.stmts1, stmt.addr);
            break;
        case SemStmt_Loop:
            code += gen_loop(stmt.br1, stmt.br2, stmt.stmts1);
            break;
        }
    }
    return code;
}

void SemTable::gen_backpatch(std::vector<SemData> &stmts, SemLabel exit) {
    printf("backpatching\n");
    for (SemData &stmt : stmts) {
        bool has_sub_stmts = false;
        if (stmt.stmts1.size() > 0) {
            printf("backpatch recurse\n");
            gen_backpatch(stmt.stmts1, exit);
            has_sub_stmts = true;
        }
        if (stmt.stmts2.size() > 0) {
            printf("backpatch recurse on false\n");
            gen_backpatch(stmt.stmts2, exit);
            has_sub_stmts = true;
        }
        if (has_sub_stmts) {
            continue;
        }

        if (stmt.br1 == LABEL_PENDING) {
            printf("stmt needed backpatch!, dest label = label%d\n", exit);
            stmt.br1 = exit;
            stmt.code =
                string_format("goto label%d \t\t\t# backpatched\n", exit);
        } else {
            printf("stmt doesn't need backpatch: %s", stmt.code.c_str());
        }
    }
}

std::string SemTable::gen_loop(SemLabel entry, SemLabel exit,
                               std::vector<SemData> &body) {
    gen_backpatch(body, exit);
    std::string code;
    code += string_format("label%d: \t\t\t# loop entry\n", entry);
    code += gen_stmts(body);
    code += string_format("goto label%d \t\t\t# continue loop\n", entry);
    code += string_format("label%d: \t\t\t# loop exit\n", exit);
    return code;
}

int get_op_precedence(TokenData op) {
    switch (op) {
    case TokData_Op_Add:
    case TokData_Op_Sub:
        return 1;
    case TokData_Op_Mul:
    case TokData_Op_Div:
    case TokData_Op_Mod:
        return 2;
    case TokData_Op_Pow:
        return 2;
    case TokData_Op_Less:
    case TokData_Op_LessEq:
    case TokData_Op_Greater:
    case TokData_Op_GreaterEq:
    case TokData_Op_Eq:
    case TokData_Op_Neq:
        return 4;
    case TokData_Op_And:
        return 5;
    case TokData_Op_Or:
        return 6;
    default:
        return -1;
    }
}

// algoritmo shunting-yard
std::string SemTable::gen_precedence(std::vector<StackElem> &input) {
    printf("gen_precedence\n");

    std::vector<StackElem> op_stack;
    std::deque<StackElem> queue;

    for (StackElem &elem : input) {
        printf("for stackelem : input\n");
        if (elem.kind != StackElem_Token) {
            queue.push_back(elem);
            continue;
        }

        int cur_precedence = get_op_precedence(elem.data.token.data);
        while (!op_stack.empty()) {
            printf("while op_stack not empty 1\n");
            StackElem top = op_stack.back();
            int top_precedence = get_op_precedence(top.data.token.data);
            if (top_precedence < cur_precedence) {
                break;
            }
            queue.push_back(top);
            op_stack.pop_back();
        }
        op_stack.push_back(elem);
    }
    while (!op_stack.empty()) {
        printf("while op_stack not empty 2\n");
        queue.push_back(op_stack.back());
        op_stack.pop_back();
    }

    std::string code;
    std::vector<SemData> stack;

    for (size_t i = 0; i < queue.size(); i++) {
        printf("for stackelem: queue\n");
        if (queue[i].kind != StackElem_Token) {
            stack.push_back(queue[i].sem_data);
        } else {
            TokenData op = queue[i].data.token.data;

            SemData left = stack[stack.size() - 2];
            SemData right = stack[stack.size() - 1];
            SemData dest;
            dest.addr = new_tmp_var();

            printf("dest_%d = left_%d . right_%d\n", dest.addr, left.addr,
                   right.addr);

            code += left.code;
            code += right.code;
            code += gen_oper(dest.addr, left.addr, right.addr, op);

            stack.resize(stack.size() - 2);
            stack.push_back(dest);
        }
    }

    return code;
}
