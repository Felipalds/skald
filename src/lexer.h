#ifndef lexer_h
#define lexer_h

#include <cstdint>
#include <list>
#include <string>
#include "input.h"

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
    Tok_Int,
    Tok_Real,
    Tok_Str,
    Tok_Ident,
    Tok_Assign,
    Tok_OpArit,
    Tok_OpLogic,
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
    Op_Not
};

enum OpRel {
    Op_Less,
    Op_LessEq,
    Op_Greater,
    Op_GreaterEq,
    Op_Eq,
    Op_Neq,
};


union TokenData {
    double lit_real;
    int64_t lit_int;
    Span span;
    OpArit op_arit;
    OpLogic op_logic;
    OpRel op_rel;

    TokenData() {}

    TokenData(double x)
        : lit_real(x) {}

    TokenData(int64_t x)
        : lit_int(x) {}

    TokenData(Span x)
        : span(x) {}

    TokenData(OpArit x)
        : op_arit(x) {}

    TokenData(OpLogic x)
        : op_logic(x) {}

    TokenData(OpRel x)
        : op_rel(x) {}
};

struct Token {
    TokenKind kind;
    TokenData data;

    Token(TokenKind kind)
        : kind(kind) {}

    Token(TokenKind kind, TokenData data)
        : kind(kind)
        , data(data) {}

    void printf_fmt(Src src);
};

enum LexErrKind {
    LexErr_BadChar,
    LexErr_UnknownOp,
};

struct LexErr {
    LexErrKind kind;
    Span span;

    LexErr(LexErrKind k)
	: kind(k) {}

    LexErr(LexErrKind k, Span s)
	: kind(k)
	, span(s) {}
};

struct Lexer {
    std::list<Token> tokens;
    std::list<LexErr> errors;

    Lexer(Src &src);
    void add_ident_or_kw(std::string &tmp, Span span);
    void add_op_or_symbol(std::string &tmp, Span span);
};

#endif
