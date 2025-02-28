#ifndef lexer_h
#define lexer_h

#include "input.h"
#include <cstdint>
#include <string>
#include <vector>

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
    Tok_Type,
    Tok_Ident,
    Tok_Assign,
    Tok_Oper,
    Tok_Not,
    Tok_ParOpen,
    Tok_ParClose,
    Tok_Period,
    Tok_Lit,
    Tok_Eof,
    Tok_None = -1,
};

enum TokenData {
    TokData_Op_Add,
    TokData_Op_Sub,
    TokData_Op_Mul,
    TokData_Op_Div,
    TokData_Op_Mod,
    TokData_Op_Pow,
    TokData_Op_Less,
    TokData_Op_LessEq,
    TokData_Op_Greater,
    TokData_Op_GreaterEq,
    TokData_Op_Eq,
    TokData_Op_Neq,
    TokData_Op_And,
    TokData_Op_Or,
    TokData_Type_Str,
    TokData_Type_Real,
    TokData_Type_Int,
    TokData_None = -1,
};

void TokenKind_print(TokenKind kind);

struct Token {
    TokenKind kind;
    TokenData data;
    Span span;

    Token(TokenKind kind, Span span)
        : kind(kind), data(TokData_None), span(span) {
    }

    Token(TokenKind kind, TokenData data, Span span)
        : kind(kind), data(data), span(span) {
    }

    // implementado em output.cpp
    void print(Src src);
};

enum LexErrKind {
    LexErr_BadChar,
    LexErr_UnknownOp,
    LexErr_UnexpectedEOF,
};

struct LexErr {
    LexErrKind kind;
    Span span;

    LexErr(LexErrKind k) : kind(k) {
    }
    LexErr(LexErrKind k, Span s) : kind(k), span(s) {
    }

    // implementado em output.cpp
    void print(Src &src);
};

struct Lexer {
    std::vector<Token> tokens;
    std::vector<LexErr> errors;

    Lexer(Src &src);
    void add_ident_or_kw(std::string &tmp, Span span);
    void add_op_or_symbol(std::string &tmp, Span span);
    void add_int(Span span);
    void add_float(Span span);
    void add_string(Span span);
    void add_err(LexErrKind kind, Span span);
};
#endif
