#ifndef QUANTA_AST_H
#define QUANTA_AST_H

#include <string>
#include <utility>
#include <vector>

/// A node in the abstract syntax tree
class AstNode {
public:
    virtual ~AstNode() = default;
    [[nodiscard]] virtual AstNode* clone() const = 0;
};

class Program {
public:
    std::vector<std::unique_ptr<AstNode>> body;

    /// Construct a program with an empty body
    explicit Program() : body() {};

    /// Copy constructor
    Program(const Program& other) : body() {
        for (const auto& node : other.body) {
            auto clone = std::unique_ptr<AstNode>(node->clone());
            body.push_back(std::move(clone));
        }
    }

    /// Clone the program
    [[nodiscard]] Program clone() const {
        return Program(*this);
    }

    /// push a node to the back of the body
    void push_back(std::unique_ptr<AstNode> node) {
        body.push_back(std::move(node));
    }
};

/// Unit class for representing a type declaration
class TypeDecl : public AstNode {
public:
    [[nodiscard]] TypeDecl* clone() const override = 0;
};

/// unit class for representing a type expression
class TypeExpr : public AstNode {
public:
    [[nodiscard]] TypeExpr* clone() const override = 0;
};

/// unit class for representing an expression
class Expr : public AstNode {
public:
    [[nodiscard]] Expr* clone() const override = 0;
};

/// unit class for a literal expression
class LiteralExpr : public Expr {
public:
    [[nodiscard]] LiteralExpr* clone() const override = 0;
};

/// a record type declaration, a product type with named fields e.g 'record Point {x: i32, y: i32}'
class RecordDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> fields;
    Span span;

    /// Construct a record declaration
    RecordDecl(std::string name, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> fields, Span span)
            : name(std::move(name)), fields(std::move(fields)), span(span) {};

    /// Copy constructor
    RecordDecl(const RecordDecl& other) : name(other.name), fields(), span(other.span) {
        for (const auto& field : other.fields) {
            auto first = field.first;
            auto second = std::unique_ptr<TypeExpr>(field.second->clone());
            fields.emplace_back(first, std::move(second));
        }
    }

    /// Clone the record declaration
    [[nodiscard]] RecordDecl* clone() const override {
        return new RecordDecl(*this);
    }
};

/// a discriminated sum type declaration e.g 'enum Expr { Int: i32, Float: f64,}' or 'enum Option { Some: a, None }'
class EnumDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> variants;
    Span span;

    /// Construct an enum declaration
    EnumDecl(std::string name, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> variants, Span span)
            : name(std::move(name)), variants(std::move(variants)), span(span) {};


    /// Copy constructor
    EnumDecl(const EnumDecl& other) : name(other.name), variants(), span(other.span) {
        for (const auto& variant : other.variants) {
            auto first = variant.first;
            if (variant.second) {
                auto second = std::unique_ptr<TypeExpr>(variant.second->clone());
                variants.emplace_back(first, std::move(second));
            } else {
                variants.emplace_back(first, nullptr);
            }
        }
    }

    /// Clone the enum declaration
    [[nodiscard]] EnumDecl* clone() const override {
        return new EnumDecl(*this);
    }
};

/// a type alias declaration e.g 'type Int = i32' or 'type Point = (i32, i32)'
class TypeAliasDecl : public TypeDecl {
public:
    std::string name;
    std::unique_ptr<TypeExpr> type;
    Span span;

    /// Construct a type alias declaration
    TypeAliasDecl(std::string name, std::unique_ptr<TypeExpr> type, Span span)
            : name(std::move(name)), type(std::move(type)), span(span) {};

    /// Copy constructor
    TypeAliasDecl(const TypeAliasDecl& other) : name(other.name), type(other.type->clone()), span(other.span) {};

    /// Clone the type alias declaration
    [[nodiscard]] TypeAliasDecl* clone() const override {
        return new TypeAliasDecl(*this);
    }
};

/// a literal type expression (e.g. i32, f64, bool, or user defined type)
class SymbolTypeExpr : public TypeExpr {
public:
    std::string symbol;
    Span span;

    /// Construct a symbol type expression
    SymbolTypeExpr(std::string symbol, Span span)
            : symbol(std::move(symbol)), span(span) {};

    /// Copy constructor
    SymbolTypeExpr(const SymbolTypeExpr& other) : symbol(other.symbol), span(other.span) {};

    /// Clone the symbol type expression
    [[nodiscard]] SymbolTypeExpr* clone() const override {
        return new SymbolTypeExpr(*this);
    }
};

/// a positional product type expression e.g. (i32, f64) or (i32, f64, bool) etc
class TupleTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> fields;
    Span span;

    /// Construct a tuple type expression
    TupleTypeExpr(std::vector<std::unique_ptr<TypeExpr>> fields, Span span)
            : fields(std::move(fields)), span(span) {};

    /// Copy constructor
    TupleTypeExpr(const TupleTypeExpr& other) : fields(), span(other.span) {
        if (!other.fields.empty()) {
            for (const auto& field : other.fields) {
                auto field_clone = std::unique_ptr<TypeExpr>(field->clone());
                fields.push_back(std::move(field_clone));
            }
        }
    };

    /// Clone the tuple type expression
    [[nodiscard]] TupleTypeExpr* clone() const override {
        return new TupleTypeExpr(*this);
    }
};

/// a sum type expression e.g. i32 | f64
class SumTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> variants;
    Span span;

    /// Construct a sum type expression
    explicit SumTypeExpr(std::vector<std::unique_ptr<TypeExpr>> variants, Span span)
            : variants(std::move(variants)), span(span) {};

    /// Copy constructor
    SumTypeExpr(const SumTypeExpr& other) : variants(), span(other.span) {
        for (const auto& variant : other.variants) {
            auto variant_clone = std::unique_ptr<TypeExpr>(variant->clone());
            variants.push_back(std::move(variant_clone));
        }
    }

    /// Clone the sum type expression
    [[nodiscard]] SumTypeExpr* clone() const override {
        return new SumTypeExpr(*this);
    }
};

/// an array type expression e.g. [i32] if a value is provided or [i32 4] if a size is provided
class ArrayTypeExpr : public TypeExpr {
public:
    std::unique_ptr<TypeExpr> elem;
    std::unique_ptr<Expr> size;
    Span span;

    /// Construct an array type expression
    ArrayTypeExpr(std::unique_ptr<TypeExpr> elem, std::unique_ptr<Expr> size, Span span)
            : elem(std::move(elem)), size(std::move(size)), span(span) {};

    /// Copy constructor
    ArrayTypeExpr(const ArrayTypeExpr& other) : elem(other.elem->clone()), size(other.size->clone()), span(other.span) {};

    /// Clone the array type expression
    [[nodiscard]] ArrayTypeExpr* clone() const override {
        return new ArrayTypeExpr(*this);
    }
};

/// a function type expression e.g. (i32, f64) -> bool
class FuncTypeExpr : public TypeExpr {
public:
    std::vector<std::unique_ptr<TypeExpr>> params;
    std::unique_ptr<TypeExpr> ret;
    Span span;

    /// Construct a function type expression
    FuncTypeExpr(std::vector<std::unique_ptr<TypeExpr>> params, std::unique_ptr<TypeExpr> ret, Span span)
            : params(std::move(params)), ret(std::move(ret)), span(span) {};

    /// Copy constructor
    FuncTypeExpr(const FuncTypeExpr& other) : params(), ret(other.ret->clone()), span(other.span) {
        for (const auto& param : other.params) {
            auto clone = std::unique_ptr<TypeExpr>(param->clone());
            params.push_back(std::move(clone));
        }
    }

    /// Clone the function type expression
    [[nodiscard]] FuncTypeExpr* clone() const override {
        return new FuncTypeExpr(*this);
    }
};

/// a number literal expression
class NumberExpr : public LiteralExpr {
public:
    [[nodiscard]] NumberExpr* clone() const override = 0;
};

/// an integer literal expression
class IntExpr : public NumberExpr {
public:
    long value;
    Span span;

    /// Construct an integer expression
    explicit IntExpr(long value, Span span) : value(value), span(span) {};

    /// Copy constructor
    IntExpr(const IntExpr& other) : value(other.value), span(other.span) {};

    /// Clone the integer expression
    [[nodiscard]] IntExpr* clone() const override {
        return new IntExpr(*this);
    }
};

/// a floating point literal expression
class FloatExpr : public NumberExpr {
public:
    double value;
    Span span;

    /// Construct a floating point expression
    explicit FloatExpr(double value, Span span) : value(value), span(span) {};

    /// Copy constructor
    FloatExpr(const FloatExpr& other) : value(other.value), span(other.span) {};

    /// Clone the floating point expression
    [[nodiscard]] FloatExpr* clone() const override {
        return new FloatExpr(*this);
    }
};

#endif //QUANTA_AST_H
