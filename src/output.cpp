#include "input.h"
#include "lexer.h"
#include <cstdio>

void Src::print() {
    size_t line = 1;
    /*printf("  1 | ");*/

    bool newline = true;
    for (size_t i = 0; i < bytes.size(); i++) {
        if (newline) {
            printf("%3zu | ", line);
        }
        newline = bytes[i] == '\n';
        line += newline;
        printf("%c", bytes[i]);
    }
}

void Src::print_span(Span span) {
    if (span.line + 1 > lines.size()) {
        return;
    }
    for (size_t i = span.first; i <= span.second; i++) {
        printf("%c", bytes[i]);
    }
}

#define GOTO_COLUMN_TERM_ESCAPE "\e[%uG"
// avanca cursor para `column`
void term_goto_column(unsigned int column) {
    printf(GOTO_COLUMN_TERM_ESCAPE, column);
}

void Token::print(Src src) {
    switch (kind) {
    case Tok_Var:
        printf("[var]");
        break;
    case Tok_Rav:
        printf("[rav]");
        break;
    case Tok_Main:
        printf("[main]");
        break;
    case Tok_Niam:
        printf("[niam]");
        break;
    case Tok_Loop:
        printf("[loop]");
        break;
    case Tok_Pool:
        printf("[pool]");
        break;
    case Tok_Int:
        printf("[int]");
        break;
    case Tok_Real:
        printf("[real]");
        break;
    case Tok_Str:
        printf("[str]");
        break;
    case Tok_If:
        printf("[if]");
        break;
    case Tok_Or:
        printf("[or]");
        break;
    case Tok_Fi:
        printf("[fi]");
        break;
    case Tok_Do:
        printf("[do]");
        break;
    case Tok_In:
        printf("[in]");
        break;
    case Tok_Out:
        printf("[out]");
        break;
    case Tok_Stop:
        printf("[stop]");
        break;
    case Tok_Die:
        printf("[die]");
        break;
    case Tok_Ident:
        printf("[ident]");
        break;
    case Tok_Assign:
        printf("[<-]");
        break;
    case Tok_Oper:
        printf("[op]");
        break;
    case Tok_Not:
        printf("[!]");
        break;
    case Tok_ParOpen:
        printf("[(]");
        break;
    case Tok_ParClose:
        printf("[)]");
        break;
    case Tok_Period:
        printf("[.]");
        break;
    case Tok_LitStr:
        printf("[str]");
        break;
    case Tok_LitReal:
        printf("[real]");
        break;
    case Tok_LitInt:
        printf("[int]");
        break;
    case Tok_Eof:
        printf("[eof]");
        break;
    default:
        printf("[??]");
    }
    term_goto_column(11);
    printf(" | ");
    src.print_span(span);
}

void LexErr::print(Src src) {
    printf("line %zu [%zu %zu]: ", span.line + 1, span.first, span.second);
    switch (kind) {
    case LexErr_BadChar:
        printf("(bad char)");
        break;
    case LexErr_UnknownOp:
        printf("(unknown operator)");
        break;
    case LexErr_UnexpectedEOF:
        printf("(unexpected EOF)");
        break;
    }
    printf("\n");

    Span line_span = src.line_span(span.line);
    src.print_span(line_span);

    term_goto_column(span.first - line_span.first + 1);
    for (size_t i = span.first; i <= span.second; i++) {
        printf("^");
    }
    printf("\n");
}
