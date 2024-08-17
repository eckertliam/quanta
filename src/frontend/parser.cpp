#include "parser.h"

Token Parser::consume() {
    Token tok = current;
    current = lexer.next_token();
    return tok;
}

std::optional<Token> Parser::expect(TokenType type) {
    if (current.type == type) {
        return consume();
    }
    return std::nullopt;
}

std::unique_ptr<TypeDecl> Parser::parse_type_decl() {
    // current token is TYPE
    // take the start location of the token
    Loc start = consume().span.start;
    auto name = expect(TokenType::IDENTIFIER);
    if (!name) {
        errors.push_back("Expected an identifier after 'type' at " + current.span.to_string());
        return nullptr;
    }
    if (!expect(TokenType::ASSIGN)) {
        errors.push_back("Expected '=' after type name at " + current.span.to_string());
        return nullptr;
    }
    if (expect(TokenType::L_BRACE)) {
        // record type
        auto fields = parse_record_fields();
        auto end = expect(TokenType::R_BRACE)->span.end;
        return std::make_unique<RecordDecl>(name->lexeme, std::move(fields), Span{start, end});
    }
}

std::vector<std::pair<std::string, TypeExpr>> Parser::parse_record_fields() {
    // called when current token is L_BRACE
    // TODO implement this
}