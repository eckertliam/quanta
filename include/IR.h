#ifndef QUANTA_IR_H
#define QUANTA_IR_H

#include <string>
#include <utility>
#include <vector>
#include <utility>

class IRNode;

class IRVisitor {
public:
    virtual void visit(IRNode *ir) = 0;
};

class IRNode {
public:
    virtual void accept(IRVisitor *visitor) {
        visitor->visit(this);
    }

    virtual std::string to_string() = 0;

    virtual ~IRNode() = default;
};

class Instruction;

class Label : public IRNode {
public:
    std::string ident;
    std::vector<Instruction *> instructions;

    explicit Label(std::string ident) : ident(std::move(ident)) {}

    void push_instruction(Instruction *instruction) {
        instructions.push_back(instruction);
    }

    std::string to_string() override;
};

class Type : public IRNode {
};

class TypeVariable : public Type {
public:
    std::string ident;
    std::vector<Type *> implements;

    explicit TypeVariable(std::string ident) : ident(std::move(ident)) {}

    void push_implement(Type *type) {
        implements.push_back(type);
    }

    std::string to_string() override;
};

class TypeDecl : public IRNode {
public:
    std::string ident;
    Type *type;

    TypeDecl(std::string ident, Type *type) : ident(std::move(ident)), type(type) {}

    std::string to_string() override;
};

class StructDecl : public IRNode {
public:
    std::string ident;
    std::vector<TypeVariable *> generics;
    std::vector<std::pair<std::string, Type *>> fields;

    explicit StructDecl(std::string ident) : ident(std::move(ident)) {}

    void push_field(const std::string& field_ident, Type *type) {
        fields.emplace_back(field_ident, type);
    }

    std::string to_string() override;
};

class Instruction : public IRNode {
};

class Goto : public Instruction {
public:
    Label *target;

    explicit Goto(Label *target) : target(target) {}

    std::string to_string() override;
};

class Constant;

class SwitchInt : public Instruction {
public:
    std::vector<std::pair<int, Label *>> cases;
    Label *default_case;
    Constant *value;


    SwitchInt(std::vector<std::pair<int, Label *>> cases, Label *default_case, Constant *value) : cases(std::move(cases)), default_case(default_case), value(value) {}

    std::string to_string() override;
};

class Function : public IRNode {
public:
    std::string ident;
    std::vector<TypeVariable *> generics;
    std::vector<std::pair<std::string, Type *>> params;
    Type *return_type;
    Label *entry;
    std::vector<Label *> labels;

    Function(std::string ident, std::vector<std::pair<std::string, Type *>> params, Type *return_type, Label *entry) : ident(std::move(ident)), params(std::move(params)), return_type(return_type), entry(entry) {}

    void push_label(Label *label);
    void push_generic(TypeVariable *type_variable);
    void push_param(const std::string& ident, Type *type);

    std::string to_string() override;
};

class Module : public IRNode {
public:
    std::vector<StructDecl *> structs;
    std::vector<TypeDecl *> type_decls;
    std::vector<Function *> functions;

    Module(std::vector<StructDecl *> structs, std::vector<Function *> functions) : structs(std::move(structs)), functions(std::move(functions)) {}
    Module() = default;

    void push_struct(StructDecl *struct_decl);
    void push_function(Function *function);
    void push_type_decl(TypeDecl *type_decl);

    std::string to_string() override;
};

class Constant : public IRNode {
};

class IntConstant : public Constant {
public:
    long value;

    explicit IntConstant(int value) : value(value) {}

    std::string to_string() override;
};

class BoolConstant : public Constant {
public:
    bool value;

    explicit BoolConstant(bool value) : value(value) {}

    std::string to_string() override;
};

class FloatConstant : public Constant {
public:
    double value;

    explicit FloatConstant(double value) : value(value) {}

    std::string to_string() override;
};

class StringConstant : public Constant {
public:
    std::string value;

    explicit StringConstant(std::string value) : value(std::move(value)) {}

    std::string to_string() override;
};

class ArrayConstant : public Constant {
public:
    std::vector<Constant *> values;

    explicit ArrayConstant(std::vector<Constant *> values) : values(std::move(values)) {}

    std::string to_string() override;
};

class StructConstant : public Constant {
public:
    std::vector<std::pair<std::string, Constant *>> values;

    explicit StructConstant(std::vector<std::pair<std::string, Constant *>> values) : values(std::move(values)) {}

    std::string to_string() override;
};

class BoolType : public Type {
public:
    std::string to_string() override;
};

class IntType : public Type {
public:
    int size;

    explicit IntType(int size) : size(size) {}

    std::string to_string() override;
};

class FloatType : public Type {
public:
    int size;

    explicit FloatType(int size) : size(size) {}

    std::string to_string() override;
};

class PointerType : public Type {
public:
    Type *base;

    explicit PointerType(Type *base) : base(base) {}

    std::string to_string() override;
};

class ArrayType : public Type {
public:
    Type *base;
    int size;

    ArrayType(Type *base, int size) : base(base), size(size) {}

    std::string to_string() override;
};

class CharType : public Type {
public:
    std::string to_string() override;
};

class VoidType : public Type {
public:
    std::string to_string() override;
};

class FunctionType : public Type {
public:
    Type *return_type;
    std::vector<Type *> generic_args;
    std::vector<Type *> arg_types;

    FunctionType(Type *return_type) : return_type(return_type) {}

    void push_arg(Type *type);
    void push_generic(Type *type);

    std::string to_string() override;
};

class UserType : public Type {
public:
    std::string ident;
    std::vector<Type *> generic_args;

    explicit UserType(std::string ident) : ident(std::move(ident)) {}

    void push_generic(Type *type) {
        generic_args.push_back(type);
    }

    std::string to_string() override;
};


#endif //QUANTA_IR_H
