#include <cstring>
#include <string>
#include "lexer.h"


enum LexState {
    State_Start,
    State_IdentKw,
    State_Int,
    State_Real,
    State_Symbol,
    State_Comment,
    State_Str,
};

void Lexer::add_ident_or_kw(std::string &tmp, Span span) {
    const char *keywords[] =  { "var", "rav", "main", "niam", "loop", "pool",
        "if", "or", "fi", "do", "in", "out", "stop", "die", "int", "real", "str" };
    const TokenKind kinds[] { Tok_Var, Tok_Rav, Tok_Main, Tok_Niam, Tok_Loop, Tok_Pool,
        Tok_If, Tok_Or, Tok_Fi, Tok_Do, Tok_In, Tok_Out, Tok_Stop, Tok_Die, Tok_Int, Tok_Real, Tok_Str };

    for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
        if (tmp == keywords[i]) {
            tokens.push_back(Token(kinds[i]));
	    return;
        }
    }
    tokens.push_back(Token(Tok_Ident, TokenData(span)));
}

void Lexer::add_op_or_symbol(std::string &tmp, Span span) {
    const char *symbols[] {
        "+", "-", "*", "/", "%", "^",
        "<", "<=", ">", ">=", "=", "!=",
        "&", "|", "!",
        ")",
        "(",
        "<-",
        ".",
    };
    const TokenKind kinds[] = {
        Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, 
        Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, 
        Tok_OpLogic, Tok_OpLogic, Tok_OpLogic, 
        Tok_ParOpen,
        Tok_ParClose,
        Tok_Assign,
        Tok_Period,
    };
    const TokenData data[] = {
        TokenData(Op_Add), TokenData(Op_Sub), TokenData(Op_Mul), TokenData(Op_Div), TokenData(Op_Mod), TokenData(Op_Pow),
        TokenData(Op_Less), TokenData(Op_LessEq), TokenData(Op_Greater), TokenData(Op_GreaterEq), TokenData(Op_Eq), TokenData(Op_Neq),
        TokenData(Op_And), TokenData(Op_Or), TokenData(Op_Not),
        TokenData(),
        TokenData(),
        TokenData(),
        TokenData(),
    };

    for (size_t i = 0; i < sizeof(symbols) / sizeof(*symbols); i++) {
        if (tmp == symbols[i]) {
            Token token(kinds[i], data[i]);
            tokens.push_back(token);
	    return;
        }
    }
    errors.push_back(LexErr(LexErr_UnknownOp, span));
}

#define SINGLE_CHAR "+-*/%^()&|.="
#define PREFIX "!<>"
#define SUFFIX "="

Lexer::Lexer(Src &src) {
    LexState state = State_Start;
    std::string tmp;
    Span span;
    bool escape = false;

    src.bytes.push_back(' ');
    for (size_t i = 0; i < src.bytes.size(); i++) {
        char ch = src.bytes[i];

        switch (state) {
        case State_Start:
            if (isalpha(ch) || ch == '_') {
                span.first = i;
                state = State_IdentKw;
                tmp += ch;
            } else if (isdigit(ch)) {
                tmp += ch;
                state = State_Int;
            } else if (strchr(SINGLE_CHAR, ch) != NULL) {
                tmp += ch;
                add_op_or_symbol(tmp, span);
                tmp.clear();
            } else if (strchr(PREFIX, ch) != NULL) {
                span.first = i;
                tmp += ch;
                state = State_Symbol;
            } else if (ch == '$') {
                state = State_Comment;
            } else if (ch == '"') {
                state = State_Str;
            } else if (!isspace(ch)) {
                span.first = i;
                span.second = i;
                LexErr err(LexErr_BadChar, span);
                errors.push_back(err);
            }
            break;
        case State_IdentKw:
            if (isalpha(ch) || isdigit(ch) || ch == '_') {
                tmp += ch;
            } else {
                i--;
                span.second = i;
                add_ident_or_kw(tmp, span);
                tmp.clear();
                state = State_Start;
            }
            break;
        case State_Comment:
            if (ch == '\n') {
                state = State_Start;
            }
            break;
        case State_Int:
            if (isdigit(ch)) {
                tmp += ch;
            } else if (ch == ',') {
                tmp += '.';
                state = State_Real;
            } else {
                i--;
                int64_t data = std::stoll(tmp);
                tmp.clear();
                Token token = Token(Tok_LitInt, TokenData(data));
                tokens.push_back(token);
		state = State_Start;
            }
            break;
        case State_Real:
            if (isdigit(ch)) {
                tmp += ch;
            } else {
                i--;
                double data = std::stod(tmp);
                tmp.clear();
		state = State_Start;
            }
            break;
        case State_Symbol:
            if (strchr(SUFFIX, ch) != NULL) {
                tmp += ch;
            } else {
                i--;
            }
            span.second = i;
            add_op_or_symbol(tmp, span);
            tmp.clear();
            state = State_Start;
            break;
        case State_Str:
            if (ch == '"' && !escape) {
                span.second = i;
                Token token(Tok_LitStr, TokenData(span));
                tmp.clear();
                state = State_Start;
            }
            escape = (ch == '\\');
            break;
        }
    }
}
