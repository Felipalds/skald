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
    } table[] = {
        {"var", Tok_Var},   {"rav", Tok_Rav},   {"main", Tok_Main},
        {"niam", Tok_Niam}, {"loop", Tok_Loop}, {"pool", Tok_Pool},
        {"if", Tok_If},     {"or", Tok_Or},     {"fi", Tok_Fi},
        {"do", Tok_Do},     {"in", Tok_In},     {"out", Tok_Out},
        {"stop", Tok_Stop}, {"die", Tok_Die},   {"int", Tok_Int},
        {"real", Tok_Real}, {"str", Tok_Str},
    };

    for (size_t i = 0; i < sizeof(table) / sizeof(*table); i++) {
        if (tmp == table[i].word) {
            tokens.push_back(Token(table[i].kind, span));
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
        {"*", Tok_OpMul, TokenData(Op_Mul)},
        {"/", Tok_OpMul, TokenData(Op_Div)},
        {"%", Tok_OpMul, TokenData(Op_Mod)},
        {"^", Tok_OpMul, TokenData(Op_Pow)},

        {"+", Tok_OpSum, TokenData(Op_Add)},
        {"-", Tok_OpSum, TokenData(Op_Sub)},

        {"<", Tok_OpRel, TokenData(Op_Less)},
        {"<=", Tok_OpRel, TokenData(Op_LessEq)},
        {">", Tok_OpRel, TokenData(Op_Greater)},
        {">=", Tok_OpRel, TokenData(Op_GreaterEq)},
        {"=", Tok_OpRel, TokenData(Op_Eq)},
        {"!=", Tok_OpRel, TokenData(Op_Neq)},

        {"&", Tok_OpAnd, TokenData()},
        {"|", Tok_OpOr, TokenData()},
        {"!", Tok_OpNot, TokenData()},

        {"(", Tok_ParOpen, TokenData()},
        {")", Tok_ParClose, TokenData()},
        {"<-", Tok_Assign, TokenData()},
        {".", Tok_Period, TokenData()},
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

void Lexer::add_int(std::string &tmp, Span span) {
    int64_t data = std::stoll(tmp);
    Token token(Tok_LitInt, TokenData(data), span);
    tokens.push_back(token);
}
void Lexer::add_float(std::string &tmp, Span span) {
    double data = std::stod(tmp);
    Token token(Tok_LitInt, TokenData(data), span);
    tokens.push_back(token);
}

void Lexer::add_string(Span span) {
    Token token(Tok_LitStr, span);
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
                tmp += ch;
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
                tmp += "#"; // marcador
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
                state = State_Start;
            }
            break;
        case State_Int:
            if (isdigit(ch)) {
                // continue adicionando digitos
                tmp += ch;
            } else if (ch == ',') {
                // não eh mais inteiro
                tmp += '.';
                state = State_Real;
            } else {
                // terminou inteiro
                i--;
                span.second = i;
                add_int(tmp, span);
                tmp.clear();
                state = State_Start;
            }
            break;
        case State_Real:
            if (isdigit(ch)) {
                // continua adiconando digitos
                tmp += ch;
            } else {
                // terminou float
                i--;
                span.second = i;
                add_float(tmp, span);
                tmp.clear();
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
                tmp.clear();
                state = State_Start;
            } else {
                // _TODO? escapar outros chars
                escape = (ch == '\\');
            }
            break;
        }
    }
    if (!tmp.empty()) {
        span.second = std::max(i, size_t(1)) - 1;
        add_err(LexErr_UnexpectedEOF, span);
    }
}
