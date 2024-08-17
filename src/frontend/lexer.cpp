#include "lexer.h"

Loc Lexer::get_loc() const {
    return {line, column};
}

Span Lexer::get_span(Loc prev) const {
    return {prev, get_loc()};
}

std::string Lexer::get_lexeme() const {
    return {src.data() + start, current - start};
}

std::string Lexer::get_lexeme(size_t start_offset, size_t current_offset) const {
    return {src.data() + start_offset, current_offset - start_offset};
}

Token Lexer::simple_token(TokenType type) const {
    return {type, "", get_span(start_loc)};
}

Token Lexer::token(TokenType type) const {
    return {type, get_lexeme(), get_span(start_loc)};
}

char Lexer::peek() const {
    if (current >= src.size()) {
        return '\0';
    }
    return src[current];
}

char Lexer::peek_next() const {
    if (current + 1 >= src.size()) {
        return '\0';
    }
    return src[current + 1];
}


char Lexer::consume() {
    if (current >= src.size()) {
        return '\0';
    }
    current++;
    return src[current - 1];
}

bool Lexer::expect(char expected) {
    if (peek() == expected) {
        consume();
        return true;
    }
    return false;
}

void Lexer::skip_whitespace() {
    while (true) {
        switch (peek()) {
            case ' ':
                column++;
                break;
            case '\t':
                column += 4;
                break;
            case '\n':
                line++;
                column = 1;
                break;
            default:
                return;
        }
        consume();
    }
}

void Lexer::skip_comment() {
    while (peek() != '\n' && peek() != '\0') {
        consume();
    }
    // Consume the newline
    consume();
}

static std::map<std::string , TokenType> keywords = {
        {"type", TokenType::TYPE},
        {"fn", TokenType::FN},
};

Token Lexer::lex_identifier() {
    while (std::isalnum(peek()) || peek() == '_') {
        consume();
    }
    TokenType type = TokenType::IDENTIFIER;
    std::string lexeme = get_lexeme();
    if (keywords.contains(lexeme)) {
        type = keywords[lexeme];
    }
    return {type, lexeme, get_span(start_loc)};
}

Token Lexer::lex_number() {
    while (std::isdigit(peek())) {
        consume();
    }
    if (peek() == '.' && std::isdigit(peek_next())) {
        consume();
        while (std::isdigit(peek())) {
            consume();
        }
    }
    return {TokenType::NUMBER, get_lexeme(), get_span(start_loc)};
}

Token Lexer::next_token() {
    skip_whitespace();
    start = current;
    start_loc = get_loc();
    if (current >= src.size()) {
        return simple_token(TokenType::EOF_);
    }
    char c = consume();
    switch (c) {
        case '(':
            return simple_token(TokenType::L_PAREN);
        case ')':
            return simple_token(TokenType::R_PAREN);
        case '{':
            return simple_token(TokenType::L_BRACE);
        case '}':
            return simple_token(TokenType::R_BRACE);
        case '[':
            return simple_token(TokenType::L_BRACKET);
        case ']':
            return simple_token(TokenType::R_BRACKET);
        case ',':
            return simple_token(TokenType::COMMA);
        case '.':
            return simple_token(TokenType::DOT);
        case '<':
            return expect('=') ? simple_token(TokenType::LE) : simple_token(TokenType::LT);
        case '>':
            return expect('=') ? simple_token(TokenType::GE) : simple_token(TokenType::GT);
        case '=':
            return expect('=') ? simple_token(TokenType::EQ) : simple_token(TokenType::ASSIGN);
        case '!':
            return expect('=') ? simple_token(TokenType::NE) : simple_token(TokenType::BANG);
        case '+':
            return simple_token(TokenType::PLUS);
        case '-':
            return expect('>') ? simple_token(TokenType::ARROW) : simple_token(TokenType::MINUS);
        case '*':
            return simple_token(TokenType::STAR);
        case '/':
            return simple_token(TokenType::SLASH);
        case '%':
            return simple_token(TokenType::PERCENT);
        case '&':
            return simple_token(TokenType::AMP);
        case '|':
            return simple_token(TokenType::PIPE);
        case '^':
            return simple_token(TokenType::CARET);
        case '#':
            skip_comment();
            return next_token();
        default:
            if (std::isalpha(c) || c == '_') {
                return lex_identifier();
            }else if (std::isdigit(c)) {
                return lex_number();
            }
            return {TokenType::ERROR, get_lexeme(), get_span(start_loc)};
    }
}
