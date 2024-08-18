#ifndef QUANTA_PARSER_H
#define QUANTA_PARSER_H

#include <iostream>
#include <utility>

#include "lexer.h"
#include "ast.h"

class Parser {
private:
    Lexer lexer;
    Token prev;
    Token current;
    Program program;

    /// consume the current token
    Token consume();
    /// expect the current token to be of a certain type
    bool expect(TokenType type);
    /// expect or error
    bool expect_or_error(TokenType type, const std::string& message);
    /// Parse an expression
    std::unique_ptr<Expr> parse_expr();
    /// Parse a record type declaration
    std::unique_ptr<RecordDecl> parse_record_decl();
    /// Parse an enum type declaration
    std::unique_ptr<EnumDecl> parse_enum_decl();
    /// Parse a type alias declaration
    std::unique_ptr<TypeAliasDecl> parse_type_alias_decl();
    /// Parse a type expression
    std::unique_ptr<TypeExpr> parse_type_expr();
    /// Parse a symbol type expression
    std::unique_ptr<SymbolTypeExpr> parse_symbol_type();
    /// Parse a Tuple type expression
    std::unique_ptr<TupleTypeExpr> parse_tuple_type();
    /// Parse a sum type expression
    std::unique_ptr<SumTypeExpr> parse_sum_type();
    /// Parse an array type expression
    std::unique_ptr<ArrayTypeExpr> parse_array_type();
    /// Parse a function type expression
    std::unique_ptr<FuncTypeExpr> parse_function_type();
    /// Parse a number expression
    std::unique_ptr<NumberExpr> parse_number();
public:
    explicit Parser(std::string src)
            : lexer(Lexer(std::move(src))), prev(Token()), current(lexer.next_token()) {}

    /// Parse a program
    Program parse_program();
};


#endif //QUANTA_PARSER_H
