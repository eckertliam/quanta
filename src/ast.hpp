#ifndef QUANTA_AST_HPP
#define QUANTA_AST_HPP

#include <string>
#include <vector>
#include <numeric> // Add this for std::gcd

// Types of expressions
// - Number
// - Boolean
// - Symbol
// - List
// - String
// - Character

class Expression
{
public:
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
    virtual bool isAtom() const { return false; }
    virtual bool isList() const { return false; }
    virtual bool isString() const { return false; }
    virtual bool isCharacter() const { return false; }
    virtual bool isBoolean() const { return false; }
    virtual bool isSymbol() const { return false; }
    virtual bool isNumber() const { return false; }
};

class List : public Expression
{
private:
    static constexpr size_t SMALL_LIST_SIZE = 8; // threshold for inline storage

    union
    {
        struct
        {
            Expression **ptr;
            size_t capacity;
        } heap; // heap allocated storage for large lists

        struct
        {
            Expression *data[SMALL_LIST_SIZE];
        } inline_; // inline storage for small lists
    };

    size_t size;
    bool is_inline;

public:
    List(const std::vector<Expression *> &items) : size(items.size())
    {
        if (items.size() <= SMALL_LIST_SIZE)
        {
            is_inline = true;
            std::copy(items.begin(), items.end(), inline_.data);
        }
        else
        {
            is_inline = false;
            heap.capacity = size;
            heap.ptr = new Expression *[size];
            std::copy(items.begin(), items.end(), heap.ptr);
        }
    }

    ~List()
    {
        if (!is_inline)
        {
            delete[] heap.ptr;
        }
    }

    bool isList() const override
    {
        return true;
    }

    std::string toString() const override
    {
        std::string result = "(";
        for (size_t i = 0; i < size; ++i)
        {
            result += heap.ptr[i]->toString();
            if (i < size - 1)
            {
                result += " ";
            }
        }
        result += ")";
        return result;
    }
};

// TODO: work out promotion rules for numbers
class Number : public Expression
{
public:
    bool isNumber() const override { return true; }
    bool isAtom() const override { return true; }
};

class Integer : public Number
{
public:
    Integer(int value) : value(value) {}
    int value;

    std::string toString() const override
    {
        return std::to_string(value);
    }
};

class Float : public Number
{
public:
    Float(double value) : value(value) {}
    double value;

    std::string toString() const override
    {
        return std::to_string(value);
    }
};

class Rational : public Number
{
public:
    Rational(int numerator, int denominator)
    {
        if (denominator == 0)
        {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        int gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
    }
    int numerator;
    int denominator;

    std::string toString() const override
    {
        return std::to_string(numerator) + "/" + std::to_string(denominator);
    };
};

class Complex : public Number
{
public:
    Complex(double real, double imaginary) : real(real), imaginary(imaginary) {}
    double real;
    double imaginary;

    std::string toString() const override
    {
        return std::to_string(real) + "+" + std::to_string(imaginary) + "i";
    }
};

class String : public Expression
{
public:
    String(const std::string &value) : value(value) {}
    std::string value;

    std::string toString() const override
    {
        return "\"" + value + "\"";
    }

    bool isString() const override
    {
        return true;
    }

    bool isAtom() const override
    {
        return true;
    }
};

class Character : public Expression
{
public:
    Character(char value) : value(value) {}
    char value;

    std::string toString() const override
    {
        return "#\\" + std::string(1, value);
    }

    bool isCharacter() const override
    {
        return true;
    }

    bool isAtom() const override
    {
        return true;
    }
};

class Boolean : public Expression
{
public:
    Boolean(bool value) : value(value) {}
    bool value;

    std::string toString() const override
    {
        return value ? "#t" : "#f";
    }

    bool isBoolean() const override { return true; }

    bool isAtom() const override { return true; }
};

class Symbol : public Expression
{
public:
    Symbol(const std::string &name) : name(name) {}
    std::string name;

    std::string toString() const override
    {
        return name;
    }

    bool isSymbol() const override { return true; }

    bool isAtom() const override { return true; }
};

#endif // QUANTA_AST_HPP
