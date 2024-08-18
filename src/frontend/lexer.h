#ifndef QUANTA_LEXER_H
#define QUANTA_LEXER_H

#include "span.h"
#include "token.h"

class Lexer {
private:
    std::string src;
    size_t start;
    size_t current;
    Loc start_loc;
    size_t line;
    size_t column;

    /// Make a loc of the current line and column
    [[nodiscard]] Loc get_loc() const;
    /// Make a span from a previous loc to the current loc
    [[nodiscard]] Span get_span(Loc start) const;
    /// Get a lexeme from the start of the current token to the current character
    [[nodiscard]] std::string get_lexeme() const;
    /// Get a lexeme from an offset of start to an offset of current
    [[nodiscard]] std::string get_lexeme(size_t start_offset, size_t current_offset) const;
    /// Make a simple token
    [[nodiscard]] Token simple_token(TokenType type) const;
    /// Make a token with a lexeme from the start of the current token to the current character
    [[nodiscard]] Token token(TokenType type) const;
    /// Peek at the next character without consuming it
    [[nodiscard]] char peek() const;
    /// Peek ahead one character
    [[nodiscard]] char peek_next() const;
    /// Consume the next character
    char consume();
    /// Consume the next character if it matches the expected character
    bool expect(char expected);
    /// Skip whitespace
    void skip_whitespace();
    /// Skip a # comment
    void skip_comment();
    /// Lex an identifier
    Token lex_identifier();
    /// Lex a number
    Token lex_number();

public:
    explicit Lexer(std::string src)
            : src(std::move(src)), start(0), current(0), line(1), column(1), start_loc({1, 1}){}
    /// Lex the next token
    Token next_token();
};


#endif //QUANTA_LEXER_H
