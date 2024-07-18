#include <cstdint>
#include <string>
#include <utility>
#include <cstring>

enum TokenKind {
    Tok_Var,
    Tok_Rav,
    Tok_Main,
    Tok_Niam,
    Tok_Loop,
    Tok_Pool,
    Tok_If,
    Tok_Or,
    Tok_Fi,
    Tok_Do,
    Tok_In,
    Tok_Out,
    Tok_Stop,
    Tok_Die,
    Tok_Ident,
    Tok_Assign,
    Tok_OpArit,
    Tok_OpLogic,
    Tok_OpUnaryNot,
    Tok_OpRel,
    Tok_ParOpen,
    Tok_ParClose,
    Tok_Period,
    Tok_LitStr,
    Tok_LitReal,
    Tok_LitInt,
};

enum OpArit {
    Op_Add,
    Op_Sub,
    Op_Mul,
    Op_Div,
    Op_Mod,
    Op_Pow,
};

enum OpLogic {
    Op_And,
    Op_Or,
};

enum OpRel {
    Op_Less,
    Op_LessEq,
    Op_Greater,
    Op_GreaterEq,
    Op_Eq,
    Op_Neq,
};

enum Type {
    Type_Int,
    Type_Str,
    Type_Real,
};

union TokenData {
    double lit_real;
    int64_t lit_int;
    std::pair<size_t, size_t> span;
    OpArit op_arit;
    OpLogic op_logic;
    OpRel op_rel;
    Type type;

    TokenData operator=(const TokenData &data) {
        return *(TokenData*)memcpy(this, &data, sizeof(*this));
    }

    TokenData() {}
    TokenData(double x): lit_real(x) {}
    TokenData(int64_t x): lit_int(x) {}
    TokenData(std::pair<size_t, size_t> x): span(x) {}
    TokenData(OpArit x): op_arit(x) {}
    TokenData(OpLogic x): op_logic(x) {}
    TokenData(OpRel x): op_rel(x) {}
    TokenData(Type x): type(x) {}
};

struct Token {
    TokenKind kind;
    TokenData data;

    Token(TokenKind kind): kind(kind) {}
    Token(TokenKind kind, TokenData data): kind(kind), data(data) {}
};

