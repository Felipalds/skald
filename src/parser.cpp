#include "parser.h"
#include <cassert>
#include <vector>

int Parser::state() {
    assert(stack.size() > 0);
    assert(stack.back().kind == StackElem_State);
    return stack.back().data.state;
}

void Parser::pop_reduce(Rule rule) {
    int rule_len = RULE_LEN[rule];
    /* análise semantica e geração de código */

    for (int i = 0; i < 2 * rule_len; i++) {
        stack.pop_back();
    }
}

void Parser::parse(std::vector<Token> &tokens) {
    size_t ip = 0;

    /*temp*/
    int p_state = 0;
    Rule rule = Rule_Skald;
    /*temp*/

    while (true) {
        int c_state = state();
        Token token = tokens[ip];

        if (false /* tabela(state, token) = empilhar p_state */) {
            stack.push_back({token});
            stack.push_back({p_state});
            ip++;
        } else if (false /* tabela(state, token) = reduzir rule->prod */) {
            pop_reduce(rule);
            int n_state = state();
            stack.push_back({rule});
        } else if (false /* tabela(state, token) = aceitar */) {
            return;
        } else {
            /* lidar com erro usando first/follow */
        }
    }
}
