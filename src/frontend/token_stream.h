#ifndef QUANTA_TOKEN_STREAM_H
#define QUANTA_TOKEN_STREAM_H

#include <vector>

#include "token.h"
#include "lexer.h"


class TokenStream {
private:
    std::vector<Token> tokens;
    size_t idx = 0;
public:
    explicit TokenStream(Lexer lexer);

    [[nodiscard]] Token peek_back() const;

    [[nodiscard]] Token peek() const;

    [[nodiscard]] Token peek_next() const;

    [[nodiscard]] Token peek_ahead(size_t n) const;

    Token consume();

    bool expect(TokenType type);

    [[nodiscard]] bool eof() const;
};


#endif //QUANTA_TOKEN_STREAM_H
