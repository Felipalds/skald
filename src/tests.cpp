#include "lexer.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

struct TestCase {
    std::string input;
    std::vector<Token> expectedTokens;
};

void runTest(const TestCase& testCase, Lexer& lexer) {
    lexer.src.bytes = testCase.input;
    LexResult result = lexer.lex();

    assert(result.tokens.size() == testCase.expectedTokens.size() && "Token count mismatch!");

    for (size_t i = 0; i < testCase.expectedTokens.size(); ++i) {
        assert(result.tokens[i].kind == testCase.expectedTokens[i].kind && "Token kind mismatch!");

        if (result.tokens[i].kind == Tok_LitInt) {
            assert(result.tokens[i].data.intValue == testCase.expectedTokens[i].data.intValue && "Token int value mismatch!");
        } else if (result.tokens[i].kind == Tok_LitReal) {
            assert(result.tokens[i].data.realValue == testCase.expectedTokens[i].data.realValue && "Token real value mismatch!");
        } else if (result.tokens[i].kind == Tok_LitStr) {
            assert(result.tokens[i].data.span.first == testCase.expectedTokens[i].data.span.first && "Token string span start mismatch!");
            assert(result.tokens[i].data.span.second == testCase.expectedTokens[i].data.span.second && "Token string span end mismatch!");
        } else if (result.tokens[i].kind == Tok_Ident) {
            assert(result.tokens[i].data.span.first == testCase.expectedTokens[i].data.span.first && "Token ident span start mismatch!");
            assert(result.tokens[i].data.span.second == testCase.expectedTokens[i].data.span.second && "Token ident span end mismatch!");
        }
    }

    std::cout << "Test passed: " << testCase.input << std::endl;
}

int main() {

    Lexer lexer= Lexer(src);


    std::vector<TestCase> testCases = {
        {"var", {Token(Tok_Var)}},
        {"rav", {Token(Tok_Rav)}},
        {"main", {Token(Tok_Main)}},
        {"niam", {Token(Tok_Niam)}},
        {"loop", {Token(Tok_Loop)}},
        {"pool", {Token(Tok_Pool)}},
        {"int", {Token(Tok_Type, TokenData(Type_Int))}},
        {"real", {Token(Tok_Type, TokenData(Type_Real))}},
        {"str", {Token(Tok_Type, TokenData(Type_Str))}},
        {"if", {Token(Tok_If)}},
        {"or", {Token(Tok_Or)}},
        {"fi", {Token(Tok_Fi)}},
        {"do", {Token(Tok_Do)}},
        {"in", {Token(Tok_In)}},
        {"out", {Token(Tok_Out)}},
        {"stop", {Token(Tok_Stop)}},
        {"die", {Token(Tok_Die)}},
        {"<-", {Token(Tok_Assign)}},
        {"12345", {Token(Tok_LitInt, 12345)}},
        {"123,45", {Token(Tok_LitReal, 123.45f)}},
        {"\"hello\"", {Token(Tok_LitStr, TokenData({0, 6}))}},
        {"a_var", {Token(Tok_Ident, TokenData({0, 4}))}},
        {"+", {Token(Tok_OpArit, TokenData(Op_Add))}},
        {"<", {Token(Tok_OpRel, TokenData(Op_Less))}},
        {"(", {Token(Tok_ParOpen)}},
        {")", {Token(Tok_ParClose)}},
        {".", {Token(Tok_Period)}},
        {"$ this is a comment\n", {}},
        {"var x = 123\n", {Token(Tok_Var), Token(Tok_Ident, TokenData({4, 4})), Token(Tok_Assign), Token(Tok_LitInt, 123)}},
        {"int a = 10,5\n", {Token(Tok_Type, TokenData(Type_Int)), Token(Tok_Ident, TokenData({4, 4})), Token(Tok_Assign), Token(Tok_LitReal, 10.5f)}}
    };

    for (const auto& testCase : testCases) {
        runTest(testCase, lexer);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
