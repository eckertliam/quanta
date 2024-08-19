#ifndef QUANTA_AST_H
#define QUANTA_AST_H

#include <string>
#include <utility>
#include <vector>

class AstNode;
class Program;
class SymbolTypeExpr;
class RecordDecl;
class EnumDecl;
class TypeAliasDecl;
class TupleTypeExpr;
class SumTypeExpr;
class ArrayTypeExpr;
class IntExpr;
class FloatExpr;

class AstVisitor {
public:
    virtual void visit(const AstNode& node) = 0;
    virtual void visit(const Program& node) = 0;
    virtual void visit(const SymbolTypeExpr& node) = 0;
    virtual void visit(const RecordDecl& node) = 0;
    virtual void visit(const EnumDecl& node) = 0;
    virtual void visit(const TypeAliasDecl& node) = 0;
    virtual void visit(const TupleTypeExpr& node) = 0;
    virtual void visit(const SumTypeExpr& node) = 0;
    virtual void visit(const ArrayTypeExpr& node) = 0;
    virtual void visit(const IntExpr& node) = 0;
    virtual void visit(const FloatExpr& node) = 0;
};

/// A node in the abstract syntax tree
class AstNode {
public:
    virtual ~AstNode() = default;
    [[nodiscard]] virtual AstNode* clone() const = 0;
    virtual void accept(AstVisitor& visitor) const {
        visitor.visit(*this);
    }
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const {
        visitor.visit(*this);
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

/// a literal type expression (e.g. i32, f64, bool, or user defined type)
class SymbolTypeExpr : public TypeExpr {
public:
    std::string symbol;
    std::vector<std::unique_ptr<TypeExpr>> generic_args;
    Span span;

    /// Construct a symbol type expression
    SymbolTypeExpr(std::string symbol, std::vector<std::unique_ptr<TypeExpr>> generic_args, Span span)
            : symbol(std::move(symbol)), generic_args(std::move(generic_args)), span(span) {};

    /// Copy constructor
    SymbolTypeExpr(const SymbolTypeExpr& other) : symbol(other.symbol), generic_args(), span(other.span) {
        for (const auto& arg : other.generic_args) {
            auto arg_clone = std::unique_ptr<TypeExpr>(arg->clone());
            generic_args.push_back(std::move(arg_clone));
        }
    };

    /// Clone the symbol type expression
    [[nodiscard]] SymbolTypeExpr* clone() const override {
        return new SymbolTypeExpr(*this);
    }

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

/// a record type declaration, a product type with named fields e.g 'record Point {x: i32, y: i32}'
class RecordDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> fields;
    Span span;

    /// Construct a record declaration
    RecordDecl(std::string name, std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> fields, Span span)
            : name(std::move(name)), generic_params(std::move(generic_params)), fields(std::move(fields)), span(span) {};

    /// Copy constructor
    RecordDecl(const RecordDecl& other) : name(other.name), generic_params(), fields(), span(other.span) {
        for (const auto& param : other.generic_params) {
            auto param_clone = std::unique_ptr<SymbolTypeExpr>(param->clone());
            generic_params.push_back(std::move(param_clone));
        }
        for (const auto& field : other.fields) {
            auto first = field.first;
            auto second = field.second->clone();
            fields.emplace_back(first, second);
        }
    }

    /// Clone the record declaration
    [[nodiscard]] RecordDecl* clone() const override {
        return new RecordDecl(*this);
    }

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

/// a discriminated sum type declaration e.g 'enum Expr { Int: i32, Float: f64,}' or 'enum Option<a> { Some: a, None }'
class EnumDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    std::vector<std::pair<std::string , std::unique_ptr<TypeExpr>>> variants;
    Span span;

    /// Construct an enum declaration
    EnumDecl(std::string name, std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params, std::vector<std::pair<std::string, std::unique_ptr<TypeExpr>>> variants, Span span)
            : name(std::move(name)), generic_params(std::move(generic_params)), variants(std::move(variants)), span(span) {};


    /// Copy constructor
    EnumDecl(const EnumDecl& other) : name(other.name), generic_params(), variants(), span(other.span) {
        for (const auto& param : other.generic_params) {
            auto param_clone = std::unique_ptr<SymbolTypeExpr>(param->clone());
            generic_params.push_back(std::move(param_clone));
        }
        for (const auto& variant : other.variants) {
            auto first = variant.first;
            if (variant.second) {
                auto second = variant.second->clone();
                variants.emplace_back(first, second);
            } else {
                variants.emplace_back(first, nullptr);
            }
        }
    }

    /// Clone the enum declaration
    [[nodiscard]] EnumDecl* clone() const override {
        return new EnumDecl(*this);
    }

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

/// a type alias declaration e.g 'type Int = i32' or 'type Point = [i32, i32]'
class TypeAliasDecl : public TypeDecl {
public:
    std::string name;
    std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params;
    std::unique_ptr<TypeExpr> type;
    Span span;

    /// Construct a type alias declaration
    TypeAliasDecl(std::string name, std::vector<std::unique_ptr<SymbolTypeExpr>> generic_params, std::unique_ptr<TypeExpr> type, Span span)
            : name(std::move(name)), generic_params(std::move(generic_params)), type(std::move(type)), span(span) {};

    /// Copy constructor
    TypeAliasDecl(const TypeAliasDecl& other)
            : name(other.name), generic_params(), type(other.type->clone()), span(other.span) {
        for (const auto& param : other.generic_params) {
            auto param_clone = std::unique_ptr<SymbolTypeExpr>(param->clone());
            generic_params.push_back(std::move(param_clone));
        }
    };

    /// Clone the type alias declaration
    [[nodiscard]] TypeAliasDecl* clone() const override {
        return new TypeAliasDecl(*this);
    }

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

/// a positional product type expression e.g. [i32, f64] [T_0 ... T_N]
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

/// an array type expression e.g. [T; N]
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
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

    /// accept a visitor
    void accept(AstVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

#endif //QUANTA_AST_H
