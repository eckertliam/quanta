#ifndef NEBULA_TOKEN_H
#define NEBULA_TOKEN_H

#include <string>
#include <utility>
#include "span.h"

enum class TokenType {
    L_PAREN,
    R_PAREN,
    L_BRACE,
    R_BRACE,
    L_BRACKET,
    R_BRACKET,
    COMMA,
    DOT,
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,
    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    BANG,
    AMP,
    PIPE,
    CARET,
    ARROW,
    // Keywords
    TYPE,
    FN,
    // Literals
    IDENTIFIER,
    NUMBER,
    // Special
    EOF_,
    ERROR,
};



class Token {
public:
    TokenType type;
    std::string lexeme;
    Span span;

    Token(TokenType type, std::string lexeme, Span span) : type(type), lexeme(std::move(lexeme)), span(span) {}

    [[nodiscard]] std::string to_string() const;
};


#endif //NEBULA_TOKEN_H
