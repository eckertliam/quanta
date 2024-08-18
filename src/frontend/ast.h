#ifndef QUANTA_AST_H
#define QUANTA_AST_H

#include <string>
#include <utility>
#include <vector>

/// A node in the abstract syntax tree
class AstNode {};

class Program {
public:
    std::vector<std::unique_ptr<AstNode>> body;

    explicit Program() : body() {}

    void push_back(std::unique_ptr<AstNode> node) {
        body.push_back(std::move(node));
    }
};

/// Unit class for representing a type declaration
class TypeDecl : public AstNode {};

/// unit class for representing a type expression
class TypeExpr : public AstNode {};

/// unit class for representing an expression
class Expr : public AstNode {};

/// unit class for a literal expression
class LiteralExpr : public Expr {};

/// a record type declaration, a product type with named fields e.g 'record Point {x: i32, y: i32}'
class RecordDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> fields;
    Span span;

    RecordDecl(std::string name, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> fields, Span span)
            : name(std::move(name)), fields(std::move(fields)), span(span) {}
};

/// a discriminated sum type declaration e.g 'enum Expr { Int: i32, Float: f64,}' or 'enum Option { Some: a, None }'
class EnumDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> variants;
    Span span;

    EnumDecl(std::string name, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> variants, Span span)
            : name(std::move(name)), variants(std::move(variants)), span(span) {}
};

/// a type alias declaration e.g 'type Int = i32' or 'type Point = (i32, i32)'
class TypeAliasDecl : public TypeDecl {
public:
    std::string name;
    std::unique_ptr<TypeExpr> type;
    Span span;

    TypeAliasDecl(std::string name, std::unique_ptr<TypeExpr> type, Span span)
            : name(std::move(name)), type(std::move(type)), span(span) {}
};

/// a literal type expression (e.g. i32, f64, bool, or user defined type)
class SymbolTypeExpr : public TypeExpr {
public:
    std::string symbol;
    Span span;

    SymbolTypeExpr(std::string symbol, Span span)
            : symbol(std::move(symbol)), span(span) {}
};

/// a positional product type expression e.g. (i32, f64) or (i32, f64, bool) etc
class TupleTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> fields;
    Span span;

    TupleTypeExpr(std::vector<std::unique_ptr<TypeExpr>> fields, Span span)
            : fields(std::move(fields)), span(span) {}
};

/// a sum type expression e.g. i32 | f64
class SumTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> variants;
    Span span;

    explicit SumTypeExpr(std::vector<std::unique_ptr<TypeExpr>> variants, Span span)
            : variants(std::move(variants)), span(span) {}
};

/// an array type expression e.g. [i32] if a value is provided or [i32 4] if a size is provided
class ArrayTypeExpr : public TypeExpr {
public:
    std::unique_ptr<TypeExpr> elem;
    std::unique_ptr<Expr> size;
    Span span;

    ArrayTypeExpr(std::unique_ptr<TypeExpr> elem, std::unique_ptr<Expr> size, Span span)
            : elem(std::move(elem)), size(std::move(size)), span(span) {}
};

/// a function type expression e.g. (i32, f64) -> bool
class FuncTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> params;
    std::unique_ptr<TypeExpr> ret;
    Span span;

    FuncTypeExpr(std::vector<std::unique_ptr<TypeExpr>> params, std::unique_ptr<TypeExpr> ret, Span span)
            : params(std::move(params)), ret(std::move(ret)), span(span) {}
};

/// a number literal expression
class NumberExpr : public LiteralExpr {};

/// an integer literal expression
class IntExpr : public NumberExpr {
public:
    long value;
    Span span;

    explicit IntExpr(long value, Span span) : value(value), span(span) {}
};

/// a floating point literal expression
class FloatExpr : public NumberExpr {
public:
    double value;
    Span span;

    explicit FloatExpr(double value, Span span) : value(value), span(span) {}
};

#endif //QUANTA_AST_H
