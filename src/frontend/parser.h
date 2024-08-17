#ifndef QUANTA_PARSER_H
#define QUANTA_PARSER_H

#include <optional>

#include "lexer.h"
#include "ast.h"

class Parser {
private:
    Lexer lexer;
    Token current;
    Program program;
    std::vector<std::string> errors;

    /// consume the current token
    Token consume();
    /// expect the current token to be of a certain type
    std::optional<Token> expect(TokenType type);
    /// Parse a type declaration
    std::unique_ptr<TypeDecl> parse_type_decl();
    /// Parse a record type declaration
    std::vector<std::pair<std::string, TypeExpr>> parse_record_fields();
public:
    explicit Parser(Lexer lexer) : lexer(std::move(lexer)), current(lexer.next_token()) {}

    /// Parse a program
    Program parse_program();
};


#endif //QUANTA_PARSER_H
