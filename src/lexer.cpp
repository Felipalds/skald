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

bool isDefault (char ch) {
   return (ch == '=' || ch == '.' || ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == '!' || ch =='&' ||ch == '|' || ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^');
}

Token Lexer::verify_token(const std::string& str) {

    if (std::regex_match(str, var_regex)) {
        return Token(Tok_Var);
    } else if (std::regex_match(str, rav_regex)) {
        return Token(Tok_Rav);
    } else if (std::regex_match(str, main_regex)) {
        return Token(Tok_Main);
    } else if (std::regex_match(str, niam_regex)) {
        return Token(Tok_Niam);
    } else if (std::regex_match(str, loop_regex)) {
        return Token(Tok_Loop);
    } else if (std::regex_match(str, pool_regex)) {
        return Token(Tok_Pool);
    } else if (std::regex_match(str, if_regex)) {
        return Token(Tok_If);
    } else if (std::regex_match(str, or_regex)) {
        return Token(Tok_Or);
    } else if (std::regex_match(str, fi_regex)) {
        return Token(Tok_Fi);
    } else if (std::regex_match(str, do_regex)) {
        return Token(Tok_Do);
    } else if (std::regex_match(str, in_regex)) {
        return Token(Tok_In);
    } else if (std::regex_match(str, out_regex)) {
        return Token(Tok_Out);
    } else if (std::regex_match(str, stop_regex)) {
        return Token(Tok_Stop);
    } else if (std::regex_match(str, die_regex)) {
        return Token(Tok_Die);
    } else if (std::regex_match(str, assign_regex)) {
        return Token(Tok_Assign);
    } else if (std::regex_match(str, literal_string_regex)) {
        return Token (Tok_LitStr, TokenData());
    } else if (std::regex_match(str, lit_int_regex)) {
        return Token(Tok_LitInt, std::stoll(str));
    } else if (std::regex_match(str, lit_real_regex)) {
        std::string modified_str = str;
        size_t pos = modified_str.find(',');
        if (pos != std::string::npos) {
            modified_str.replace(pos, 1, ".");
        }
        return Token(Tok_LitReal, std::stof(modified_str));
    }
    else if (std::regex_match(str, arithmetic_operator_regex)) {
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
    } else if (std::regex_match(str, ident_regex)) {
        return Token(Tok_Ident, TokenData());
    } else if (std::regex_match(str, logical_operator_regex)) {
        OpLogic op;
        switch (str[0]) {
            case '&': op = Op_And; break;
            case '|': op = Op_Or; break;
            case '!': op = Op_Not; break;
        }
        return Token(Tok_OpLogic, op);
    } else if (std::regex_match(str, relational_operator_regex)) {
        OpRel op;
        if (str == "<") op = Op_Less;
        else if (str == "<=") op = Op_LessEq;
        else if (str == ">") op = Op_Greater;
        else if (str == ">=") op = Op_GreaterEq;
        else if (str == "=") op = Op_Eq;
        else if (str == "!=") op = Op_Neq;
        return Token(Tok_OpRel, op);
    } else if (std::regex_match(str, open_par_regex)) {
        return Token(Tok_ParOpen);
    } else if (std::regex_match(str, close_par_regex)) {
        return Token(Tok_ParClose);
    } else if(std::regex_match(str, dot_regex)) {
        return Token(Tok_Period);
    }

    return Token(Err, TokenData());
}

enum State {
    START,
    ABC,
    DIG,
    DEC,
    LIT,
    O

};


LexResult Lexer::lex() {

    State state = START;
    State changed_state = START;

    LexResult result;

    std::string temporary = "";

    for (int i = 0; i < src.bytes.size(); i++){
        char ch = src.bytes[i];

        if (state == START) {
            if (std::isalpha(ch) || ch == '_') {
                state = ABC;
                temporary += ch;
                continue;
            }

            if (std::isdigit(ch)) {
                temporary += ch;
                state = DIG;
                continue;
            }

            if (ch == '"') {
                temporary += ch;
                state = LIT;
                continue;
            }

            if (isDefault(ch)) {
                temporary += ch;
                state = O;
                continue;
            }

            if (!isspace(ch)) {
                Token token = verify_token(temporary);
                if(token.kind == Err) {
                    Error error(i, token);
                    result.errors.push_back(error);
                }
                result.tokens.push_back(token);
                temporary = "";
                state = START;
                continue;
            }

        }

        if (state == O) {
            if (ch == '=' || ch == '-') {
                temporary += ch;
            }
            Token token = verify_token(temporary);
            if(token.kind == Err) {
                Error error(i, token);
                result.errors.push_back(error);
            }
            result.tokens.push_back(token);
            temporary = "";
            state = START;
            continue;
        }

        if (state == LIT) {
            if (ch == '"') {
                temporary += ch;
                Token token = verify_token(temporary);
                if(token.kind == Err) {
                    Error error(i, token);
                    result.errors.push_back(error);
                }
                result.tokens.push_back(token);
                temporary = "";
                state = START;
                continue;
            } else {
                temporary += ch;
                continue;
            }
        }

        if(state == DIG) {
           if (std::isdigit(ch)) {
               temporary += ch;
               continue;
           } else if (ch == ',') {
               state = DEC;
               temporary += ch;
               continue;
           } else {
               Token token = verify_token(temporary);
               if(token.kind == Err) {
                   Error error(i, token);
                   result.errors.push_back(error);
               }
               i--;
               result.tokens.push_back(token);
               temporary = "";
               state = START;
               continue;
           }
        }

        if(state == DEC) {
            if (std::isdigit(ch)) {
                temporary += ch;
                continue;
            } else {
                Token token = verify_token(temporary);
                if(token.kind == Err) {
                    Error error(i, token);
                    result.errors.push_back(error);
                }
                i--;
                result.tokens.push_back(token);
                temporary = "";
                state = START;
                continue;
            }
        }

        if (state == ABC) {
           if(std::isalnum(ch)) {
              temporary += ch;
              continue;
           } else {
               Token token = verify_token(temporary);
               if(token.kind == Err) {
                   Error error(i, token);
                   result.errors.push_back(error);
               }
               i--;
               result.tokens.push_back(token);
               temporary = "";
               state = START;
               continue;
           }
        }
    }
    return result;

}
