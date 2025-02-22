#include "lexer.h"
#include <memory>
#include <vector>

// placeholder, como armazenar identifier?
// tem que incorporar com tabela de simbolos
typedef uint32_t Ident;

struct Sem_Stmts;
struct Sem_Stmt;
struct Sem_Expr;
struct Sem_Val;
struct Sem_Decl;
struct Sem_Decls;

enum Sem_Oper {
    Sem_Op_Add,
    Sem_Op_Sub,
    Sem_Op_Mul,
    Sem_Op_Div,
    Sem_Op_Mod,
    Sem_Op_Pow,
    Sem_Op_Less,
    Sem_Op_LessEq,
    Sem_Op_Greater,
    Sem_Op_GreaterEq,
    Sem_Op_Eq,
    Sem_Op_Neq,
    Sem_Op_And,
    Sem_Op_Or,
};

enum Sem_Type {
    Sem_Type_Str,
    Sem_Type_Real,
    Sem_Type_Int,
};

// ==============================================================
// Val
// ==============================================================
struct Sem_ValParExpr {
    std::unique_ptr<Sem_Expr> expr;
};
struct Sem_ValNotVal {
    std::unique_ptr<Sem_Val> val;
};
struct Sem_ValLit {
    Token lit;
};
struct Sem_ValId {
    Token ident;
};

struct Sem_Val {
    enum {
        Sem_ValKindParExpr,
        Sem_ValKindNotVal,
        Sem_ValKindLit,
        Sem_ValKindId,
    } kind;
    union {
        Sem_ValParExpr par_expr;
        Sem_ValNotVal not_val;
        Sem_ValLit lit;
        Sem_ValId id;
    } val;
};

// ==============================================================
// Expr, esse eu n sei exato como armazenar
// ==============================================================
struct Sem_Expr {
    std::vector<Sem_Val> vals;
    std::vector<Sem_Oper> oper;
};

// ==============================================================
// Stmt
// ==============================================================
struct Sem_StmtStop {};

struct Sem_StmtDie {};

struct Sem_StmtIf {
    Ident cond;
    Sem_Stmts *true_branch;
};

struct Sem_StmtIfOr {
    Ident cond;
    Sem_Stmts *true_branch;
    Sem_Stmts *false_branch;
};

struct Sem_StmtLoop {
    Sem_Stmts *body;
};

struct Sem_StmtOut {
    Ident id;
};

struct Sem_StmtAssign {
    Ident id;
    Sem_Expr expr;
};
struct Sem_StmtIn {
    Ident id;
};

struct Sem_Stmt {
    enum {
        Sem_StmtKindDecls,
        Sem_StmtKindDecl,
        Sem_StmtKindStmts,
        Sem_StmtKindStop,
        Sem_StmtKindDie,
        Sem_StmtKindIf,
        Sem_StmtKindIfOr,
        Sem_StmtKindLoop,
        Sem_StmtKindOut,
        Sem_StmtKindAssign,
        Sem_StmtKindIn
    } kind;
    union {
        Sem_StmtStop stop;
        Sem_StmtDie die;
        Sem_StmtIf if_;
        Sem_StmtIfOr ifor;
        Sem_StmtLoop loop;
        Sem_StmtOut out;
        Sem_StmtAssign assign;
        Sem_StmtIn in;
    } data;
};

struct Sem_Stmts {
    std::vector<Sem_Stmt> stmts;
};

// ==============================================================
// Decl
// ==============================================================
struct Sem_Decl {
    Ident id;
    Sem_Type type;
};

struct Sem_Decls {
    std::vector<Sem_Decl> decls;
};

// ==============================================================
// Raíz
// ==============================================================
struct Program {
    Sem_Decls decls;
    Sem_Stmts stmts;
};
