void parse(std::vector<Token> &tokens) {
    size_t ip = 0;
    std::vector<int> stack = { 0 };

    while (true) {
        int state = stack.back();
        Token token = tokens[ip];

        if (/* tabela(state, token) = empilhar p_state */) {
            push_token(stack, token);
            push_state(stack, p_state);
            ip++;
        } else if (/* tabela(state, token) = reduzir rule->prod */) {
            pop_reduce(stack, prod); // aqui incorpora analisador semantico
            int n_state = stack.back();
            push_rule(stack, rule);
        } else if (/* tabela(state, token) = aceitar */) {
            return;
        } else {
            /* lidar com erro usando first/follow */
        }
    }
}
