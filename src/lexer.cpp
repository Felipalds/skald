#include "lexer.h"
#include <cctype>
#include <string>
#include <regex>

Token Lexer::verify_token(const std::string& str) {
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
    static const std::regex assign_regex("^<-?$");
    static const std::regex lit_int_regex("^\\d+$");
    static const std::regex op_arit_regex("^[+\\-*/%]$");
    static const std::regex ident_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");

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
    // } else if (std::regex_match(str, lit_int_regex)) {
    //     return Token(Tok_LitInt, TokenData(std::stoi(str)));
    } else if (std::regex_match(str, op_arit_regex)) {
        OpArit op;
        switch (str[0]) {
            case '+': op = Op_Add; break;
            case '-': op = Op_Sub; break;
            case '*': op = Op_Mul; break;
            case '/': op = Op_Div; break;
            case '%': op = Op_Mod; break;
        }
        return Token(Tok_OpArit, TokenData(op));
    } else if (std::regex_match(str, ident_regex)) {
        return Token(Tok_Ident, TokenData());
    }

    // TODO: return an special error
    return Token(Tok_Ident);
}

enum State {
    START,
    START_TEXT,
    TEXT,
    NUM_IN_TEXT,
    FINISH_TEXT,
    START_LITERAL,
    END_LITERAL,
    NUMBER,
    SYMBOL
};


LexResult Lexer::lex() {

    State state = START;
    State changed_state = START;

    LexResult result;

    std::string temporary = "";

    for (int i = 0; i < src.bytes.size(); i++){
        char ch = src.bytes[i];

        // rules to change state
        if (state == START) { //rules if we are starting reading
           if(std::isalpha(ch) || ch == '_') {
               state = START_TEXT;
           }
           if(std::isdigit(ch)) {
               state = NUMBER;
           }
           if(ch == '"') {
               state = START_LITERAL;
           }
        }
        if(state == START_TEXT) {
            if(std::isdigit(ch)) {
                state = NUM_IN_TEXT;
            }
            if(std::isalpha(ch) || ch == '_') {
                state = TEXT;
            }
        }

        // rules to consume and return token
        if (state == START) {
           if (std::ispunct(ch)) {
                temporary += ch;
                result.tokens.push_back(verify_token(temporary));
                temporary = "";
           }
        }
    }

}
