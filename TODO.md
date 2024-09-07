# TODO

### geral:
- [ ] usar interfaces privadas ao inves de `struct`
- [ ] fazer testes

### `main`:
- [x] reportar erro de `argv`

### `input`:
- [x] ler arquivo fonte
- [x] registrar indices das linhas

### `output`:
- [x] exibir token
- [ ] (bugado) reportar erro com highlight na a linha de origem

### `lexer`:
- [x] definir formato `Token`
- [x] definir formato `LexResult`
- [x] implementar lexer
- [x] separar Tok_OpArit em Tok_OpAdd (+, -), Tok_OpMul (\*, /, %)
- [x] separar Tok_OpLogic em Tok_OpOr (|), Tok_OpAnd (&), Tok_OpNot(!)

### `ast`:
- [ ] implementar analisador sintatico
