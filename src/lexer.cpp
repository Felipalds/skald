#include "lexer.h"
#include <cctype>
#include <cstdio>
#include <string>
#include <regex>
#include <iostream>
#include <algorithm>

static const std::regex var_regex("^var$");
static const std::regex rav_regex("^rav$");
static const std::regex main_regex("^main$");
static const std::regex niam_regex("^niam$");
static const std::regex loop_regex("^loop$");
static const std::regex pool_regex("^pool$");
static const std::regex if_regex("^if$");
static const std::regex or_regex("^or$");
static const std::regex fi_regex("^fi$");
static const std::regex do_regex("^do$");
static const std::regex in_regex("^in$");
static const std::regex out_regex("^out$");
static const std::regex stop_regex("^stop$");
static const std::regex die_regex("^die$");
static const std::regex assign_regex("^<-$");
static const std::regex lit_int_regex("^\\d+$");
static const std::regex lit_real_regex("^\\d+(,\\d+)*$");
static const std::regex logical_operator_regex("^([&|!])$");
static const std::regex arithmetic_operator_regex("^[+\\-*/%^]$");
static const std::regex relational_operator_regex("^([<>]=?|>=|=|!=)$");
static const std::regex ident_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
static const std::regex literal_string_regex("^\".*\"$");
static const std::regex open_par_regex("^\\($");
static const std::regex close_par_regex("^\\)$");
static const std::regex dot_regex("^\\.$");

bool isDefault(char ch) {
    return std::strchr("=.<>()!&|+-*/%^", ch) != nullptr;
}

Token Lexer::verify_token(const std::string& str) {
    if (std::regex_match(str, var_regex)) return Token(Tok_Var);
    if (std::regex_match(str, rav_regex)) return Token(Tok_Rav);
    if (std::regex_match(str, main_regex)) return Token(Tok_Main);
    if (std::regex_match(str, niam_regex)) return Token(Tok_Niam);
    if (std::regex_match(str, loop_regex)) return Token(Tok_Loop);
    if (std::regex_match(str, pool_regex)) return Token(Tok_Pool);
    if (std::regex_match(str, if_regex)) return Token(Tok_If);
    if (std::regex_match(str, or_regex)) return Token(Tok_Or);
    if (std::regex_match(str, fi_regex)) return Token(Tok_Fi);
    if (std::regex_match(str, do_regex)) return Token(Tok_Do);
    if (std::regex_match(str, in_regex)) return Token(Tok_In);
    if (std::regex_match(str, out_regex)) return Token(Tok_Out);
    if (std::regex_match(str, stop_regex)) return Token(Tok_Stop);
    if (std::regex_match(str, die_regex)) return Token(Tok_Die);
    if (std::regex_match(str, assign_regex)) return Token(Tok_Assign);
    if (std::regex_match(str, literal_string_regex)) return Token(Tok_LitStr, TokenData());
    if (std::regex_match(str, lit_int_regex)) return Token(Tok_LitInt, std::stoll(str));

    if (std::regex_match(str, lit_real_regex)) {
        std::string modified_str = str;
        std::replace(modified_str.begin(), modified_str.end(), ',', '.');
        return Token(Tok_LitReal, std::stof(modified_str));
    }

    if (std::regex_match(str, arithmetic_operator_regex)) {
        OpArit op;
        switch (str[0]) {
            case '+': op = Op_Add; break;
            case '-': op = Op_Sub; break;
            case '*': op = Op_Mul; break;
            case '/': op = Op_Div; break;
            case '%': op = Op_Mod; break;
            case '^': op = Op_Pow; break;
        }
        return Token(Tok_OpArit, TokenData(op));
    }

    if (std::regex_match(str, ident_regex)) return Token(Tok_Ident, TokenData());

    if (std::regex_match(str, logical_operator_regex)) {
        OpLogic op;
        switch (str[0]) {
            case '&': op = Op_And; break;
            case '|': op = Op_Or; break;
            case '!': op = Op_Not; break;
        }
        return Token(Tok_OpLogic, op);
    }

    if (std::regex_match(str, relational_operator_regex)) {
        OpRel op;
        if (str == "<") op = Op_Less;
        else if (str == "<=") op = Op_LessEq;
        else if (str == ">") op = Op_Greater;
        else if (str == ">=") op = Op_GreaterEq;
        else if (str == "=") op = Op_Eq;
        else if (str == "!=") op = Op_Neq;
        return Token(Tok_OpRel, op);
    }

    if (std::regex_match(str, open_par_regex)) return Token(Tok_ParOpen);
    if (std::regex_match(str, close_par_regex)) return Token(Tok_ParClose);
    if (std::regex_match(str, dot_regex)) return Token(Tok_Period);

    return Token(Err, TokenData());
}

enum State {
    START,
    ABC,
    DIG,
    DEC,
    LIT,
    O,
    COMMENT
};

LexResult Lexer::lex() {
    State state = START;
    LexResult result;
    std::string temporary;
    Span span = {0, 0};

    for (size_t i = 0; i < src.bytes.size(); ++i) {
        char ch = src.bytes[i];

        switch (state) {
            case START:
                if (std::isalpha(ch) || ch == '_') {
                    span.first = i;
                    state = ABC;
                } else if (std::isdigit(ch)) {
                    state = DIG;
                } else if (ch == '"') {
                    span.first = i;
                    state = LIT;
                } else if (ch == '$') {
                    state = COMMENT;
                } else if (isDefault(ch)) {
                    state = O;
                } else if (!isspace(ch)) {
                    state = START;
                }

                if (state != START) temporary += ch;
                break;

            case COMMENT:
                if (ch == '\n') state = START;
                break;

            case O:
                if (ch == '=' || ch == '-') {
                    temporary += ch;
                }

                result.tokens.push_back(verify_token(temporary));
                temporary.clear();
                state = START;
                break;

            case LIT:
                temporary += ch;
                if (ch == '"') {
                    span.second = i;
                    result.tokens.push_back(verify_token(temporary));
                    result.tokens.back().data = TokenData(span);
                    temporary.clear();
                    state = START;
                }
                break;

            case DIG:
                if (std::isdigit(ch)) {
                    temporary += ch;
                } else if (ch == ',') {
                    state = DEC;
                    temporary += ch;
                } else {
                    result.tokens.push_back(verify_token(temporary));
                    temporary.clear();
                    state = START;
                    --i; // re-evaluate current character
                }
                break;

            case DEC:
                if (std::isdigit(ch)) {
                    temporary += ch;
                } else {
                    result.tokens.push_back(verify_token(temporary));
                    temporary.clear();
                    state = START;
                    --i; // re-evaluate current character
                }
                break;

            case ABC:
                if (std::isalnum(ch)) {
                    temporary += ch;
                } else {
                    span.second = i;
                    result.tokens.push_back(verify_token(temporary));
                    result.tokens.back().data = TokenData(span);
                    temporary.clear();
                    state = START;
                    --i; // re-evaluate current character
                }
                break;
        }
    }

    if (!temporary.empty()) {
        result.tokens.push_back(verify_token(temporary));
    }

    return result;
}
