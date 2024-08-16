#ifndef NEBULA_TOKEN_H
#define NEBULA_TOKEN_H

#include <string_view>

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
    // Special
    EOF_,
    ERROR,
};

class Token {
public:
    TokenType type;
    std::string_view lexeme;
    size_t line;
    size_t  column;

    Token(TokenType type, std::string_view lexeme, size_t line, size_t column)
            : type(type), lexeme(lexeme), line(line), column(column) {};

    Token(TokenType type, size_t line, size_t column)
            : type(type), lexeme(), line(line), column(column) {};


};


#endif //NEBULA_TOKEN_H
