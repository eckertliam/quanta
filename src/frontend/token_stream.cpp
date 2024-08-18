#include "token_stream.h"

TokenStream::TokenStream(Lexer lexer) {
    Token token = lexer.next_token();
    do {
        tokens.push_back(token);
        token = lexer.next_token();
    } while (token.type != TokenType::EOF_);
    tokens.push_back(token);
}

Token TokenStream::peek_back() const {
    return tokens[idx - 1];
}

Token TokenStream::peek() const {
    return tokens[idx];
}

Token TokenStream::peek_next() const {
    return tokens[idx + 1];
}

Token TokenStream::peek_ahead(size_t n) const {
    return tokens[idx + n];
}

Token TokenStream::consume() {
    Token token = tokens[idx];
    idx += 1;
    return token;
}

bool TokenStream::expect(TokenType type) {
    if (tokens[idx].type == type) {
        idx += 1;
        return true;
    }
    return false;
}

bool TokenStream::eof() const {
    return idx >= tokens.size();
}
