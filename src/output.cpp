#include "input.h"
#include "lexer.h"
#include "parser.h"
#include <cstdio>

void Src::print() {
    size_t line = 1;
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
    if (span.first >= bytes.size() || span.second >= bytes.size()) {
        printf("[span OOB]");
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

void TokenKind_print(TokenKind kind) {
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
    case Tok_Type:
        printf("[type]");
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
    case Tok_Lit:
        printf("[lit]");
        break;
    case Tok_Eof:
        printf("[eof]");
        break;
    default:
        printf("[??]");
    }
}

void Token::print(Src src) {
    TokenKind_print(kind);
    term_goto_column(11);
    printf(" | ");
    src.print_span(span);
}

void rule_print(Rule rule) {
    switch (rule) {
    case Rule_Skald:
        printf("[Skald -> VarBlock MainBlock]");
        break;
    case Rule_VarBlock:
        printf("[VarBlock -> var Decls rav]");
        break;
    case Rule_MainBlock:
        printf("[MainBlock -> main Stmts niam]");
        break;
    case Rule_Decls_Decl:
        printf("[Decls -> Decl]");
        break;
    case Rule_Decls_DeclDecls:
        printf("[Decls -> Decl Decls]");
        break;
    case Rule_Decl:
        printf("[Decl -> id type.]");
        break;
    case Rule_Stmts_Stmt:
        printf("[Stmts -> Stmt]");
        break;
    case Rule_Stmts_StmtStmts:
        printf("[Stmts -> Stmt Stmts]");
        break;
    case Rule_Stmt_Stop:
        printf("[Stmt -> stop.]");
        break;
    case Rule_Stmt_Die:
        printf("[Stmt -> die.]");
        break;
    case Rule_Stmt_If:
        printf("[Stmt -> if id do Stmts fi]");
        break;
    case Rule_Stmt_IfOr:
        printf("[Stmt -> if id do Stmts or Stmts fi]");
        break;
    case Rule_Stmt_Loop:
        printf("[Stmt -> loop Stmts pool]");
        break;
    case Rule_Stmt_Out:
        printf("[Stmt -> out id.]");
        break;
    case Rule_Stmt_Assign:
        printf("[Stmt -> id <- Expr.]");
        break;
    case Rule_Stmt_InId:
        printf("[Stmt -> in id.]");
        break;
    case Rule_Expr_Val:
        printf("[Expr -> Val]");
        break;
    case Rule_Expr_ValOpExpr:
        printf("[Expr -> Val op Expr]");
        break;
    case Rule_Val_ParExpr:
        printf("[Val -> (Expr)]");
        break;
    case Rule_Val_NotVal:
        printf("[Val -> !Val]");
        break;
    case Rule_Val_Lit:
        printf("[Val -> lit]");
        break;
    case Rule_Val_Id:
        printf("[Val -> id]");
        break;
    case Rule_None:
        printf("[?? -> ??]");
        break;
    }
}

void LexErr::print(Src &src) {
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
    printf("\n");

    term_goto_column(span.first - line_span.first + 1);
    for (size_t i = span.first; i <= span.second; i++) {
        printf("^");
    }
    printf("\n");
}

void StackElem::print() {
    switch (kind) {
    case StackElem_Token:
        TokenKind_print(data.token.kind);
        break;
    case StackElem_Rule:
        rule_print(data.rule);
        break;
    }
    printf(" ");
}

void ParseErr::print(Src &src, Table &table) {
    printf("line %zu [%zu %zu] - ", token.span.line + 1, token.span.first,
           token.span.second);

    printf("Got: ");
    TokenKind_print(token.kind);
    printf(", expected: ");
    std::vector<TokenKind> &expected = table.err_expect(state);
    for (TokenKind tok : expected) {
        TokenKind_print(tok);
        printf(", ");
    }
    printf("\n");

    if (got_token.span.line != token.span.line) {
        printf("%2zu | ", got_token.span.line + 1);
        Span line_span = src.line_span(got_token.span.line);
        src.print_span(line_span);
        printf("\n");
    }
    printf("%2zu | ", token.span.line + 1);
    Span line_span = src.line_span(token.span.line);
    src.print_span(line_span);
    printf("\n");

    term_goto_column(token.span.first - line_span.first + 1 + 5);
    for (size_t i = token.span.first; i <= token.span.second; i++) {
        printf("^");
    }
    printf("\n");
}

void Parser::stack_print(int curr_state, TokenKind tok) {
    printf("(%d ", curr_state);
    TokenKind_print(tok);
    printf(")");
    term_goto_column(13);
    printf(": ");

    for (size_t i = 0; i < state_stack.size(); i++) {
        printf("%d ", state_stack[i]);
        if (i < stack.size()) {
            stack[i].print();
        }
    }
    printf("\n");
}
