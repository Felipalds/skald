#include "ast.h"
#include <vector>

void Parser::push_token(Token token) {
    StackElem elem(StackElem_Token, token);
    stack.push_back(elem);
}

void Parser::push_state(int state) {
    StackElem elem(StackElem_State, state);
    stack.push_back(elem);
}

void Parser::push_rule(Rule rule) {
    /*StackElem elem(StackElem_Rule, rule);*/
    stack.push_back({StackElem_Rule, rule});
}

void pop_reduce(Rule rule) {
    /* busca qual é o comprimento da produção para remover 2*|prod| da pilha */
}

void Parser::parse(std::vector<Token> &tokens) {
    size_t ip = 0;

    /*temp*/
    int p_state = 0;
    Rule rule = Rule_Decls;
    /*temp*/

    while (true) {
        int state = stack.back().data.state;
        Token token = tokens[ip];

        if (false /* tabela(state, token) = empilhar p_state */) {
            stack.push_back({StackElem_Token, token});
            stack.push_back({StackElem_State, p_state});
            ip++;
        } else if (false /* tabela(state, token) = reduzir rule->prod */) {
            pop_reduce(rule); // aqui incorpora analisador semantico
            int n_state = stack.back().data.state;
            stack.push_back({StackElem_Rule, rule});
        } else if (false /* tabela(state, token) = aceitar */) {
            return;
        } else {
            /* lidar com erro usando first/follow */
        }
    }
}
