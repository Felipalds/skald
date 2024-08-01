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

Token is_ident_or_kw(std::string &tmp) {
    const char *keywords[] =  { "var", "rav", "main", "niam", "loop", "pool",
        "if", "or", "fi", "do", "in", "out", "stop", "die", "int", "real", "str" };
    const TokenKind kinds[] { Tok_Var, Tok_Rav, Tok_Main, Tok_Niam, Tok_Loop, Tok_Pool,
        Tok_If, Tok_Or, Tok_Fi, Tok_Do, Tok_In, Tok_Out, Tok_Stop, Tok_Die, Tok_Int, Tok_Real, Tok_Str };

    for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
        if (tmp == keywords[i]) {
            return Token(kinds[i]);
        }
    }
    return Token(Tok_Ident);
}

Token get_op(std::string &tmp) {
    const char *operators[] {
        "+", "-", "*", "/", "%", "^",
        "&", "|", "!",
        "=", "!=", "<", "<=", ">", ">=",
        "<", "<=", ">", ">=", "=", "!=",
        "<-",
        ".",
    };
    const TokenKind kinds[] = {
        Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, Tok_OpArit, 
        Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, Tok_OpRel, 
        Tok_OpLogic, Tok_OpLogic, Tok_OpLogic, 
        Tok_Assign,
        Tok_Period,
    };
    const TokenData data[] = {
        TokenData(Op_Add), TokenData(Op_Sub), TokenData(Op_Mul), TokenData(Op_Div), TokenData(Op_Mod), TokenData(Op_Pow),
        TokenData(Op_And), TokenData(Op_Or), TokenData(Op_Not),
        TokenData(Op_Less), TokenData(Op_LessEq), TokenData(Op_Greater), TokenData(Op_GreaterEq), TokenData(Op_Eq), TokenData(Op_Neq),
        TokenData(),
        TokenData(),
    };

    for (size_t i = 0; i < sizeof(operators) / sizeof(*operators); i++) {
        if (tmp == operators[i]) {
            return Token(kinds[i], data[i]);
        }
    }
    return Token(Tok_Err);
}

#define SPECIAL_CHARS "+-*/%^()&!|.=<>"

LexResult Lexer::lex(Src &src) {
    LexState state = State_Start;
    LexResult result;
    std::string tmp;
    Span span;
    bool escape = false;

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
            } else if (strchr(SPECIAL_CHARS, ch) != NULL) {
                tmp += ch;
                state = State_Symbol;
            } else if (ch == '$') {
                state = State_Comment;
            } else if (ch == '"') {
                tmp += ch;
                state = State_Str;
            } else if (!isspace(ch)) {
                // error
            }
            break;
        case State_IdentKw:
            if (isalpha(ch) || isdigit(ch) || ch == '_') {
                tmp += ch;
                break;
            } else {
                i--;
                Token token = is_ident_or_kw(tmp);
                tmp.clear();
                if (token.kind == Tok_Ident) {
                    span.second = i;
                    token.data = span;
                    result.tokens.push_back(token);
                } else {
                    result.tokens.push_back(token);
                }
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
                result.tokens.push_back(token);
            }
            break;
        case State_Real:
            if (isdigit(ch)) {
                tmp += ch;
            } else {
                i--;
                double data = std::stod(tmp);
                tmp.clear();
            }
            break;
        case State_Symbol:
            if (strchr(SPECIAL_CHARS, ch) != NULL) {
                tmp += ch;
            } else {
                i--;
                Token token = get_op(tmp);
                if (token.kind == Tok_Err) {
                    span.second = i;
                    token.data = TokenData(span);
                    result.errors.push_back(token);
                } else {
                    result.tokens.push_back(token);
                }
                state = State_Start;
            }
            break;
        case State_Str:
            escape = ch == '\\';
            if (ch == '"' && !escape) {
                span.second = i;
                Token token(Tok_LitStr, TokenData(span));
            }
            break;
        }
    }
}
