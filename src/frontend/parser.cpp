#include "parser.h"

void Parser::error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
}

Token Parser::consume() {
    Token prev = current;
    current = token_stream.consume();
    return prev;
}

bool Parser::expect(TokenType type) {
    if (current.type == type) {
        consume();
        return true;
    }
    return false;
}

bool Parser::expect_or_error(TokenType type, const std::string& message) {
    if (current.type == type) {
        consume();
        return true;
    }
    error(message);
    return false;
}

std::unique_ptr<Expr> Parser::parse_expr() {
    switch (current.type) {
        case TokenType::NUMBER:
            return parse_number();
        default:
            error("Expected an expression instead got " + current.to_string());
            return nullptr;
    }
}


std::unique_ptr<RecordDecl> Parser::parse_record_decl() {
    // current token is RECORD
    Token record_token = consume();
    Loc start = record_token.span.start;
    Token record_ident = consume();
    // if record identifier is not an identifier error
    if (record_ident.type != TokenType::IDENTIFIER) {
        error("Expected an identifier for record name instead got " + record_ident.to_string());
    }
    std::string name = record_ident.lexeme;
    // check for generic parameters
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    if (expect(TokenType::LT)) {
        generic_params = parse_generic_params();
    }
    // expect current token to be L_BRACE
    expect_or_error(TokenType::L_BRACE, "Expected '{' after record name instead got " + current.to_string());
    std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> fields;
    while (current.type != TokenType::R_BRACE && current.type != TokenType::EOF_) {
        // expect current token to be an ident
        Token field_ident = consume();
        if (field_ident.type != TokenType::IDENTIFIER) {
            error("Expected an identifier for field instead got " + field_ident.to_string());
        }
        std::string field_name = field_ident.lexeme;
        // expect current token to be a colon
        expect_or_error(TokenType::COLON, "Expected ':' after field name instead got " + current.to_string());
        // parse the type expression
        std::unique_ptr<TypeExpr> type = parse_type_expr();
        fields.emplace_back(field_name, std::move(type));
        // if there is a comma consume it otherwise break
        if (!expect(TokenType::COMMA)) {
            break;
        }
    }
    // expect current token to be R_BRACE
    expect_or_error(TokenType::R_BRACE, "Expected '}' instead got " + current.to_string());
    // if there is a semicolon consume it these are optional
    expect(TokenType::SEMICOLON);
    return std::make_unique<RecordDecl>(name, std::move(generic_params), std::move(fields), Span{start, token_stream.peek_back().span.end});
}

std::unique_ptr<EnumDecl> Parser::parse_enum_decl() {
    // current token is ENUM
    Loc start = consume().span.start;
    // expect current token to be an identifier
    Token ident = consume();
    if (ident.type != TokenType::IDENTIFIER) {
        error("Expected an identifier for enum name instead got " + ident.to_string());
    }
    std::string name = ident.lexeme;
    // check for generic parameters
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    if (expect(TokenType::LT)) {
        generic_params = parse_generic_params();
    }
    // expect current token to be L_BRACE
    expect_or_error(TokenType::L_BRACE, "Expected '{' after enum name instead got " + current.to_string());
    std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> variants;
    // variants follow the pattern IDENT COLON type_expr? (COMMA IDENT COLON type_expr?)*
    while (current.type != TokenType::R_BRACE && current.type != TokenType::EOF_) {
        // expect current token to be an identifier
        Token variant_ident = consume();
        if (variant_ident.type != TokenType::IDENTIFIER) {
            error("Expected an identifier for enum variant instead got " + variant_ident.to_string());
        }
        std::string variant_name = variant_ident.lexeme;
        // expect current token to be a colon if variant has a type expression
        // otherwise expect a comma or a right brace
        if (expect(TokenType::COLON)) {
            // parse the type expression
            std::unique_ptr<TypeExpr> type = parse_type_expr();
            variants.emplace_back(variant_name, std::move(type));
        } else {
            variants.emplace_back(variant_name, nullptr);
        }
        // if there is a comma consume it otherwise break
        if (!expect(TokenType::COMMA)) {
            break;
        }
    }
    // expect current token to be R_BRACE
    expect_or_error(TokenType::R_BRACE, "Expected '}' after enum variants instead got " + current.to_string());
    // if there is a semicolon consume it
    expect(TokenType::SEMICOLON);
    return std::make_unique<EnumDecl>(name, std::move(generic_params), std::move(variants), Span{start, token_stream.peek_back().span.end});
}

std::unique_ptr<TypeAliasDecl> Parser::parse_type_alias_decl() {
    // current token is TYPE
    Loc start = consume().span.start;
    // expect current token to be an identifier
    Token ident = consume();
    if (ident.type != TokenType::IDENTIFIER) {
        error("Expected an identifier for type alias name instead got " + ident.to_string());
    }
    std::string name = ident.lexeme;
    // check for generic parameters
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    if (expect(TokenType::LT)) {
        generic_params = parse_generic_params();
    }
    // expect current token to be ASSIGN
    expect_or_error(TokenType::ASSIGN, "Expected '=' after type alias name instead got " + current.to_string());
    std::cout << current.to_string() << std::endl;
    // parse the type expression
    std::unique_ptr<TypeExpr> type = parse_type_expr();
    // expect current token to be SEMICOLON
    expect_or_error(TokenType::SEMICOLON, "Expected ';' after type alias declaration instead got " + current.to_string());
    return std::make_unique<TypeAliasDecl>(name, std::move(generic_params), std::move(type), Span{start, token_stream.peek_back().span.end});
}

std::unique_ptr<TypeExpr> Parser::parse_type_expr() {
    // a type expression will begin with one of the following:
    // an identifier, a left paren, a left bracket, pipe, or a function type
    switch (current.type) {
        case TokenType::IDENTIFIER: {
            if (token_stream.peek().type == TokenType::PIPE) {
                return parse_sum_type();
            }else {
                return parse_symbol_type();
            }
        }
        case TokenType::L_BRACKET:
            return parse_bracketed_type();
        case TokenType::L_PAREN:
            return parse_paren_type();
        case TokenType::PIPE:
            return parse_sum_type();
        default:
            error("Expected a type expression instead got " + current.to_string());
            return nullptr;
    }
}

std::unique_ptr<TypeExpr> Parser::parse_simple_type() {
    switch (current.type) {
        case TokenType::IDENTIFIER:
            return parse_symbol_type();
        case TokenType::L_PAREN:
            return parse_paren_type();
        case TokenType::L_BRACKET:
            return parse_bracketed_type();
        default:
            error("Expected a simple type expression instead got " + current.to_string());
            return nullptr;
    }
}

std::unique_ptr<TypeExpr> Parser::parse_bracketed_type() {
    // current token is L_BRACKET
    Loc start = consume().span.start;
    // here there is ambiguity between tuple and array types
    // we parse the type expression, if the token after is a comma then it is a tuple if it is a semicolon then it is an array
    std::unique_ptr<TypeExpr> type = parse_type_expr();
    if (expect(TokenType::COMMA)) {
        // it is a tuple type
        // now we parse the type list
        std::vector<std::unique_ptr<TypeExpr>> fields;
        fields.push_back(std::move(type));
        while (current.type != TokenType::R_BRACKET && current.type != TokenType::EOF_) {
            // parse the type expression
            std::unique_ptr<TypeExpr> field = parse_type_expr();
            fields.push_back(std::move(field));
            // if there is a comma consume it otherwise break
            if (!expect(TokenType::COMMA)) {
                break;
            }
        }
        // expect current token to be R_BRACKET
        expect_or_error(TokenType::R_BRACKET, "Expected ']' after tuple type instead got " + current.to_string());
        return std::make_unique<TupleTypeExpr>(std::move(fields), Span{start, token_stream.peek_back().span.end});
    } else if (expect(TokenType::SEMICOLON)) {
        // it is an array type
        // parse the size expression
        std::unique_ptr<Expr> size = parse_expr();
        // expect current token to be R_BRACKET
        expect_or_error(TokenType::R_BRACKET, "Expected ']' after array type instead got " + current.to_string());
        return std::make_unique<ArrayTypeExpr>(std::move(type), std::move(size), Span{start, token_stream.peek_back().span.end});
    } else {
        error("Expected ',' or ';' after type expression instead got " + current.to_string());
        return nullptr;
    }
}

std::unique_ptr<TypeExpr> Parser::parse_paren_type() {
    // current token is L_PAREN
    Loc start = consume().span.start;
    // parse the type expression
    std::unique_ptr<TypeExpr> type = parse_type_expr();
    // expect current token to be R_PAREN
    expect_or_error(TokenType::R_PAREN, "Expected ')' after parenthesized type expression instead got " + current.to_string());
    return type;
}

std::unique_ptr<SymbolTypeExpr> Parser::parse_symbol_type() {
    // current token is an identifier
    Loc start = current.span.start;
    std::string symbol = current.lexeme;
    consume();
    // check for generic arguments
    std::vector<std::unique_ptr<TypeExpr>> generic_args;
    if (expect(TokenType::LT)) {
        generic_args = parse_generic_args();
    }
    return std::make_unique<SymbolTypeExpr>(symbol, std::move(generic_args), Span{start, token_stream.peek_back().span.end});
}

std::vector<std::unique_ptr<SymbolTypeExpr>> Parser::parse_generic_params() {
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    while (current.type != TokenType::GT && current.type != TokenType::EOF_) {
        // parse the symbol type
        auto symbol_type = parse_symbol_type();
        generic_params.push_back(std::move(symbol_type));
        // if there is a comma consume it otherwise break
        if (!expect(TokenType::COMMA)) {
            break;
        }
    }
    // expect current token to be GT
    expect_or_error(TokenType::GT, "Expected '>' after generic parameters instead got " + current.to_string());
    return generic_params;
}

std::vector<std::unique_ptr<TypeExpr>> Parser::parse_generic_args() {
    std::vector<std::unique_ptr<TypeExpr>> generic_args;
    while (current.type != TokenType::GT && current.type != TokenType::EOF_) {
        // parse the type expression
        auto type = parse_type_expr();
        generic_args.push_back(std::move(type));
        // if there is a comma consume it otherwise break
        if (!expect(TokenType::COMMA)) {
            break;
        }
    }
    // expect current token to be GT
    expect_or_error(TokenType::GT, "Expected '>' after generic arguments instead got " + current.to_string());
    return generic_args;
}

std::unique_ptr<SumTypeExpr> Parser::parse_sum_type() {
    // current token is a pipe or an identifier
    // if it is a pipe consume it and start parsing the variants
    Loc start = current.span.start;
    // if it is an identifier the loop will parse it as the first variant
    expect(TokenType::PIPE);
    std::vector<std::unique_ptr<TypeExpr>> variants;
    while (current.type != TokenType::EOF_) {
        auto type = parse_simple_type();
        variants.push_back(std::move(type));
        // if there is a pipe consume it otherwise break
        if (!expect(TokenType::PIPE)) {
            break;
        }
    }
    return std::make_unique<SumTypeExpr>(std::move(variants), Span{start, token_stream.peek_back().span.end});
}

std::unique_ptr<NumberExpr> Parser::parse_number() {
    // current token is a number
    auto token = consume();
    bool is_float = false;
    // check for a dot to determine if the number is a float
    for (auto ch : token.lexeme) {
        if (ch == '.') {
            is_float = true;
            break;
        }
    }
    if (is_float) {
        return std::make_unique<FloatExpr>(std::stod(token.lexeme), token.span);
    } else {
        return std::make_unique<IntExpr>(std::stol(token.lexeme), token.span);
    }
}

Program Parser::parse_program() {
    while (current.type != TokenType::EOF_) {
        switch (current.type) {
            case TokenType::RECORD: {
                auto record_decl = parse_record_decl();
                program.push_back(std::move(record_decl));
                break;
            }
            case TokenType::ENUM: {
                auto enum_decl = parse_enum_decl();
                program.push_back(std::move(enum_decl));
                break;
            }
            case TokenType::TYPE: {
                auto type_alias_decl = parse_type_alias_decl();
                program.push_back(std::move(type_alias_decl));
                break;
            }
            default:
                error("Expected a top level declaration instead got " + current.to_string());
                break;
        }
    }
    return program;
}
