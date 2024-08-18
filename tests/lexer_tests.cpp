#include "../src/frontend/lexer.h"
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
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::EOF_);
}

TEST(Lexer, BasicEnum) {
    std::string src = "enum Color { Red, Green, Blue }";
    Lexer lexer(src);
    Token tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::ENUM);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "Color");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::L_BRACE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COMMA);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COMMA);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::R_BRACE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::EOF_);
}

TEST(Lexer, Enum) {
    std::string src = "enum Test { A i32, B f32, C bool }";
    Lexer lexer(src);
    Token tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::ENUM);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "Test");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::L_BRACE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "i32");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COMMA);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "f32");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::COMMA);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "bool");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::R_BRACE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::EOF_);
}

TEST(Lexer, SumTypeAlias) {
    std::string src = "type Number = i32 | f32;";
    Lexer lexer(src);
    Token tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::TYPE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "Number");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::ASSIGN);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "i32");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::PIPE);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::IDENTIFIER);
    EXPECT_EQ(tok.lexeme, "f32");
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::SEMICOLON);
    tok = lexer.next_token();
    EXPECT_EQ(tok.type, TokenType::EOF_);
}
