#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include <map>

#define RULE_SHIFT 8

// clang-format off
enum NonTerm {
    NonTerm_Skald       = 0,
    NonTerm_VarBlock    = 1,
    NonTerm_MainBlock   = 2,
    NonTerm_Decls       = 3,
    NonTerm_Decl        = 4,
    NonTerm_Stmts       = 5,
    NonTerm_Stmt        = 6,
    NonTerm_Expr        = 7,
    NonTerm_Val         = 8,
    NonTerm_None        = -1,
};
enum Rule {
    Rule_Skald              = 0,
    Rule_VarBlock           = 1,
    Rule_MainBlock          = 2,
    Rule_Decls_Decl         = 3,
    Rule_Decls_DeclDecls    = 4,
    Rule_Decl               = 5,
    Rule_Stmts_Stmt         = 6,
    Rule_Stmts_StmtStmts    = 7,
    Rule_Stmt_Stop          = 8,
    Rule_Stmt_Die           = 9,
    Rule_Stmt_If            = 10,
    Rule_Stmt_IfOr          = 11,
    Rule_Stmt_Loop          = 12,
    Rule_Stmt_Out           = 13,
    Rule_Stmt_Assign        = 14,
    Rule_Stmt_InId          = 15,
    Rule_Expr_Val           = 16,
    Rule_Expr_ValOpExpr     = 17,
    Rule_Val_ParExpr        = 18,
    Rule_Val_NotVal         = 19,
    Rule_Val_Lit            = 20,
    Rule_Val_Id             = 21,

    Rule_None = -1,
};
// clang-format on

const int RULE_LEN[22] = {
    3, // Rule_Skald
    3, // Rule_VarBlock
    3, // Rule_MainBlock
    1, // Rule_Decls_Decl
    2, // Rule_Decls_DeclDecls
    3, // Rule_Decl
    1, // Rule_Stmts_Stmt
    2, // Rule_Stmts_StmtStmts
    2, // Rule_Stmt_Stop
    2, // Rule_Stmt_Die
    5, // Rule_Stmt_If
    7, // Rule_Stmt_IfOr
    3, // Rule_Stmt_Loop
    3, // Rule_Stmt_Out
    4, // Rule_Stmt_Assign
    3, // Rule_Stmt_InId
    1, // Rule_Expr_Val
    3, // Rule_Expr_ValOpExpr
    3, // Rule_Val_ParExpr
    2, // Rule_Val_NotVal
    1, // Rule_Val_Lit
    1, // Rule_Val_Id
};

class ParseTable {
    std::map<std::pair<int, TokenKind>, int> table_push;
    std::map<std::pair<int, TokenKind>, Rule> table_reduce;
    std::map<std::pair<int, NonTerm>, int> table_goto;
    std::map<int, std::vector<TokenKind>> table_err_expect;
    std::map<int, Rule> table_err_reduce;

    void tpush(int state, TokenKind tok, int dest);
    void treduce(int state, TokenKind tok, Rule rule);
    void tgoto(int state, NonTerm nt, int dest);
    void terr(int state, Rule rule);

  public:
    ParseTable();

    bool accept(int state, TokenKind token);

    int push(int state, TokenKind token);

    Rule reduce(int state, TokenKind token);

    int goto_(int state, Rule rule);

    std::vector<TokenKind> &err_expect(int state);

    Rule err_reduce(int state);
};

enum SemType {
    SemType_Int,
    SemType_Real,
    SemType_Str,
    SemType_Void,
};

enum SemOper {
    SemOp_Add,
    SemOp_Sub,
    SemOp_Mul,
    SemOp_Div,
    SemOp_Mod,
    SemOp_Pow,
    SemOp_Less,
    SemOp_LessEq,
    SemOp_Greater,
    SemOp_GreaterEq,
    SemOp_Eq,
    SemOp_Neq,
    SemOp_And,
    SemOp_Or,
};

typedef int SemAddr;
typedef int SemLabel;
#define ADDR_INVALID (-1)
#define LABEL_PENDING (-1)

enum SemStmtKind {
    SemStmt_Line,
    SemStmt_If,
    SemStmt_IfOr,
    SemStmt_Loop,
};

struct SemVar {
    SemAddr addr;
    SemType type;
};

struct StackElem;

class SemData {
  public:
    Span span;
    SemAddr addr;
    SemType type;
    SemStmtKind kind;
    SemLabel br1, br2, br3;
    std::string code;
    std::vector<SemData> stmts1;
    std::vector<SemData> stmts2;
    std::vector<StackElem> stack;

    SemData() {
        addr = ADDR_INVALID;
        type = SemType_Void;
        kind = SemStmt_Line;
        br1 = 0;
        br2 = 0;
        br3 = 0;
        code = "";
        stmts1 = {};
        stmts2 = {};
        stack = {};
        span = {0, 0, 0};
    }

    SemData(const SemData &orig) {
        addr = orig.addr;
        type = orig.type;
        kind = orig.kind;
        br1 = orig.br1;
        br2 = orig.br2;
        br3 = orig.br3;
        code = orig.code;
        stmts1 = orig.stmts1;
        stmts2 = orig.stmts2;
        stack = orig.stack;
        span = orig.span;
    }

    SemData &operator=(const SemData &orig) {
        if (this != &orig) {
            SemData out(orig);
            *this = out;
        }
        return *this;
    }
};

enum StackElemKind {
    StackElem_Token,
    StackElem_Rule,
};

union StackElemData {
    Token token;
    Rule rule;

    StackElemData(Token _token) : token(_token) {
    }
    StackElemData(Rule _rule) : rule(_rule) {
    }
};

struct StackElem {
    StackElemKind kind;
    StackElemData data;
    SemData sem_data;

    StackElem(Token token) : kind(StackElem_Token), data(token) {
    }
    StackElem(Rule rule) : kind(StackElem_Rule), data(rule) {
    }
    StackElem(Rule rule, SemData sem_data)
        : kind(StackElem_Rule), data(rule), sem_data(sem_data) {
    }
    void print();
};

struct ParseErr {
    int state;
    Token got_token;
    Token token;

    void print(Src &src, ParseTable &table);
};

enum SemErrKind {
    SemErr_AssignTypeMismatch,
    SemErr_OperTypeMismatch,
    SemErr_OperTypeIncompatible,
    SemErr_IfTypeNotInt,
    SemErr_NegateNotInt,
    SemErr_DeclRedefineVar,
    SemErr_UndefinedVar,
    SemErr_StopOutsideLoop,
    SemErr_ChainedComparisons,
};

struct SemErr {
    SemErrKind kind;
    Span span;

    void print(Src &src);
};

class SemTable {
    int counter;
    int label_counter;
    std::map<std::string, SemVar> table;

  public:
    std::string code_final;
    std::vector<SemErr> errors;

    SemLabel new_label();
    SemTable() : counter(0){};
    SemAddr register_new(std::string id, SemType type);
    SemVar get_var(std::string &id);
    SemAddr new_tmp_var();
    StackElem apply_rule(Rule rule, std::vector<StackElem> &stack, Src &src);
    SemType type_from_tokdata(TokenData data);
    SemOper oper_from_tokdata(TokenData data);
    bool oper_compatible(SemOper op, SemType type);
    void init_var(SemAddr addr, std::string &id, SemType type);

    std::string gen_assign_lit(SemAddr addr, SemType type, std::string &lexeme);
    std::string gen_assign_notval(SemAddr dest, SemAddr src);
    std::string gen_oper(SemAddr dest, SemAddr left, SemAddr right,
                         TokenData op, SemType type);
    std::string gen_input(SemAddr dest, SemType type);
    std::string gen_output(SemAddr src, SemType type);
    std::string gen_assign_expr(SemAddr dest, SemAddr src, SemType type);
    std::string gen_loop(SemLabel entry, SemLabel exit,
                         std::vector<SemData> &body);
    std::string gen_if_or(SemLabel true_branch, SemLabel false_branch,
                          SemLabel exit, std::vector<SemData> &body,
                          std::vector<SemData> &body_false, SemAddr cond);
    std::string gen_if(SemLabel true_branch, SemLabel exit,
                       std::vector<SemData> &body, SemAddr cond);
    std::string gen_stmts(std::vector<SemData> &body);
    std::string gen_expr(SemData expr);
    void shunting_yard_pop(SemAddr addr, std::vector<TokenData> &op_stack,
                           std::vector<SemData> &stack, std::string &code,
                           SemType type);
    void gen_backpatch(std::vector<SemData> &stmts, SemLabel exit);
};

class Parser {
    Src src;
    std::vector<int> state_stack;
    std::vector<StackElem> stack;

    int state();
    void pop_reduce(Rule rule);
    void err_reduce(Rule rule);

    void stack_print(int curr_state, TokenKind tok);

  public:
    SemTable sem_table;
    Parser(Src src) : src(src){};
    ParseTable table;
    std::vector<ParseErr> errors;
    static NonTerm get_nonterm(Rule rule);
    void parse(std::vector<Token> &tokens);
};
#endif
