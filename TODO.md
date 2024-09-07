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
- [x] (bugado) reportar erro com highlight na a linha de origem
- [ ] (seria bonito) deduplicar erros na mesma linha
- [ ] unificar aviso de erros em Src::report_errors

### `lexer`:
- [x] definir formato `Token`
- [x] definir formato `LexResult`
- [x] implementar lexer
- [x] separar Tok_OpArit em Tok_OpAdd (+, -), Tok_OpMul (\*, /, %)
- [x] separar Tok_OpLogic em Tok_OpOr (|), Tok_OpAnd (&), Tok_OpNot(!)
- [x] implementar TokenData como bitset? ex. real = Tok_Type | Type_Real

### `ast`:
- [ ] implementar analisador sintatico
