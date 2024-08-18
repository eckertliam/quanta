#include "../src/frontend/lexer.h"
#include "../src/frontend/token.h"
#include <gtest/gtest.h>

TEST(Lexer, RecordDecl) {
    std::string src = "record Point { x: Int, y: Int }";
    Lexer lexer(src);
    Token tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::RECORD);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "Point");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::L_BRACE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COLON);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COMMA);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COLON);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::R_BRACE);
}
