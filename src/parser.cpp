#include "parser.h"
#include "table.h"
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
    Table table;
    size_t ip = 0;

    stack.push_back({0});

    while (true) {
        int curr_state = state();
        Token token = tokens[ip];

        printf("(%d ", curr_state);
        TokenKind_print(token.kind);
        printf(")\t: ");
        for (StackElem elem : stack) {
            elem.print();
        }
        printf("\n");

        int push_state = table.push(curr_state, token.kind);
        if (push_state != -1) {
            stack.push_back({token});
            stack.push_back({push_state});
            ip++;
            printf("push\n");
            continue;
        }

        Rule rule = table.reduce(curr_state, token.kind);
        if (rule != Rule_None) {
            pop_reduce(rule);
            int goto_ = table.goto_(state(), rule);
            stack.push_back({rule});
            printf("reduce\n");
            stack.push_back({goto_});
            printf("goto\n");
            continue;
        }

        if (table.accept(state(), token.kind)) {
            printf("accept\n");
            return;
        }
        /* lidar com erros */
        printf("PARSE ERROR\n");
        return;
    }
}
