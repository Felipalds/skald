#include "lexer.h"
#include <cstring>
#include <string>

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
    struct {
        const char *word;
        TokenKind kind;
        TokenData data;
    } table[] = {
        {"var", Tok_Var, TokData_None},
        {"rav", Tok_Rav, TokData_None},
        {"main", Tok_Main, TokData_None},

        {"niam", Tok_Niam, TokData_None},
        {"loop", Tok_Loop, TokData_None},
        {"pool", Tok_Pool, TokData_None},

        {"if", Tok_If, TokData_None},
        {"or", Tok_Or, TokData_None},
        {"fi", Tok_Fi, TokData_None},

        {"do", Tok_Do, TokData_None},
        {"in", Tok_In, TokData_None},
        {"out", Tok_Out, TokData_None},
        {"stop", Tok_Stop, TokData_None},
        {"die", Tok_Die, TokData_None},
        {"int", Tok_Type, TokData_Type_Int},
        {"real", Tok_Type, TokData_Type_Real},
        {"str", Tok_Type, TokData_Type_Str},

    };

    for (size_t i = 0; i < sizeof(table) / sizeof(*table); i++) {
        if (tmp == table[i].word) {
            tokens.push_back(Token(table[i].kind, table[i].data, span));
            return;
        }
    }
    tokens.push_back(Token(Tok_Ident, span));
}

void Lexer::add_op_or_symbol(std::string &tmp, Span span) {
    struct {
        const char *sym;
        TokenKind kind;
        TokenData data;
    } table[] = {
        {"*", Tok_Oper, TokData_Op_Mul},
        {"/", Tok_Oper, TokData_Op_Div},
        {"%", Tok_Oper, TokData_Op_Mod},
        {"^", Tok_Oper, TokData_Op_Pow},

        {"+", Tok_Oper, TokData_Op_Add},
        {"-", Tok_Oper, TokData_Op_Sub},

        {"<", Tok_Oper, TokData_Op_Less},
        {"<=", Tok_Oper, TokData_Op_LessEq},
        {">", Tok_Oper, TokData_Op_Greater},
        {">=", Tok_Oper, TokData_Op_GreaterEq},
        {"=", Tok_Oper, TokData_Op_Eq},
        {"!=", Tok_Oper, TokData_Op_Neq},

        {"&", Tok_Oper, TokData_Op_And},
        {"|", Tok_Oper, TokData_Op_Or},

        {"!", Tok_Not, TokData_None},

        {"(", Tok_ParOpen, TokData_None},
        {")", Tok_ParClose, TokData_None},
        {"<-", Tok_Assign, TokData_None},
        {".", Tok_Period, TokData_None},
    };

    for (size_t i = 0; i < sizeof(table) / sizeof(*table); i++) {
        if (tmp == table[i].sym) {
            Token token(table[i].kind, table[i].data, span);
            tokens.push_back(token);
            return;
        }
    }
    errors.push_back(LexErr(LexErr_UnknownOp, span));
}

void Lexer::add_int(Span span) {
    Token token(Tok_Lit, TokData_Type_Int, span);
    tokens.push_back(token);
}
void Lexer::add_float(Span span) {
    Token token(Tok_Lit, TokData_Type_Real, span);
    tokens.push_back(token);
}

void Lexer::add_string(Span span) {
    Token token(Tok_Lit, TokData_Type_Str, span);
    tokens.push_back(token);
}

void Lexer::add_err(LexErrKind kind, Span span) {
    LexErr err(kind, span);
    errors.push_back(err);
}

#define SINGLE_CHAR "+-*/%^()&|.="
#define PREFIX "!<>"
#define SUFFIX "=-"
// ilegais: !- >-

Lexer::Lexer(Src &src) {
    src.bytes.push_back(' ');
    LexState state = State_Start;
    std::string tmp;
    Span span = {0, 0, 0};
    bool escape = false;

    size_t i;
    for (i = 0; i < src.bytes.size(); i++) {
        char ch = src.bytes[i];

        switch (state) {
        // chars que iniciam tokens
        case State_Start:
            if (isalpha(ch) || ch == '_') {
                // a-zA-z ou _, inicia keyword ou ident
                span.first = i;
                tmp += ch;
                state = State_IdentKw;
            } else if (isdigit(ch)) {
                // 0-9, inicia inteiro ou float
                span.first = i;
                state = State_Int;
            } else if (strchr(SINGLE_CHAR, ch) != NULL) {
                // chars que ja terminam um token
                span.first = i;
                span.second = i;
                tmp += ch;
                add_op_or_symbol(tmp, span);
                tmp.clear();
            } else if (strchr(PREFIX, ch) != NULL) {
                // char que inicia um operador
                span.first = i;
                tmp += ch;
                state = State_Symbol;
            } else if (ch == '$') {
                // $ inicia comentario
                state = State_Comment;
            } else if (ch == '"') {
                // " inicia string
                span.first = i;
                state = State_Str;
                escape = false;
            } else if (ch == '\n') {
                // incrementa contador de linha
                span.line++;
            } else if (!isspace(ch)) {
                // reseta span, qualquer outro char eh erro
                span.first = i;
                span.second = i;
                add_err(LexErr_BadChar, span);
            }
            break;
        case State_IdentKw:
            // continua adicionando chars no ident
            if (isalpha(ch) || isdigit(ch) || ch == '_') {
                tmp += ch;
            } else {
                // terminou ident
                i--;
                span.second = i;
                add_ident_or_kw(tmp, span);
                tmp.clear();
                state = State_Start;
            }
            break;
        case State_Comment:
            // newline termina comentario
            if (ch == '\n') {
                span.line++;
                state = State_Start;
            }
            break;
        case State_Int:
            if (ch == ',') {
                // não eh mais inteiro
                state = State_Real;
            } else if (!isdigit(ch)) {
                // terminou inteiro
                i--;
                span.second = i;
                add_int(span);
                state = State_Start;
            }
            break;
        case State_Real:
            if (!isdigit(ch)) {
                // terminou float
                i--;
                span.second = i;
                add_float(span);
                state = State_Start;
            }
            break;
        case State_Symbol:
            if (strchr(SUFFIX, ch) != NULL) {
                // se é <=, >=, ou !=
                tmp += ch;
            } else {
                // senão, é <, >, ou !
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
                add_string(span);
                state = State_Start;
            } else {
                // _TODO? escapar outros chars
                escape = (ch == '\\');
            }
            break;
        }
    }
    span.second = std::max(i, size_t(1)) - 1;
    span.first = span.second;
    if (state != State_Start && state != State_Comment) {
        add_err(LexErr_UnexpectedEOF, span);
    } else {
        tokens.push_back({Tok_Eof, span});
    }
}
