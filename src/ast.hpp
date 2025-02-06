#ifndef QUANTA_AST_HPP
#define QUANTA_AST_HPP

#include <string>
#include <vector>

// Types of expressions
// - Number
// - Boolean
// - Symbol
// - List
// - String
// - Character

class Expression {
public:
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
    virtual bool isAtom() const = 0;
    virtual bool isList() const = 0;
    virtual bool isString() const = 0;
    virtual bool isCharacter() const = 0;
    virtual bool isBoolean() const = 0;
    virtual bool isSymbol() const = 0;
    virtual bool isNumber() const = 0;
};

class Number : public Expression {
public:
    Number(double value) : value(value) {}
    double value;
    std::string toString() const override {
        return std::to_string(value);
    }
    
    bool isNumber() const override {
        return true;
    }

    bool isAtom() const override {
        return true;
    }
    
};



#endif // QUANTA_AST_HPP
