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
    Tok_Int,
    Tok_Real,
    Tok_Str,
    Tok_Ident,
    Tok_Assign,
    Tok_Oper,
    Tok_Not,
    Tok_ParOpen,
    Tok_ParClose,
    Tok_Period,
    Tok_LitStr,
    Tok_LitReal,
    Tok_LitInt,
    Tok_Eof,
    Tok_None = -1,
};

enum OperKind {
    Op_Add,
    Op_Sub,
    Op_Mul,
    Op_Div,
    Op_Mod,
    Op_Pow,
    Op_Less,
    Op_LessEq,
    Op_Greater,
    Op_GreaterEq,
    Op_Eq,
    Op_Neq,
    Op_None = -1,
};

void TokenKind_print(TokenKind kind);

struct Token {
    TokenKind kind;
    OperKind oper;
    Span span;

    Token(TokenKind kind, Span span) : kind(kind), span(span) {
    }

    Token(TokenKind kind, OperKind oper, Span span)
        : kind(kind), oper(oper), span(span) {
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
    void print(Src src);
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
