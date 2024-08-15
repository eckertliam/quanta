#include "../include/IR.h"

#define QUANTA_IR_CPP

const char *TAB = "    ";

std::string Label::to_string() {
    std::string result = "label " + ident + "{\n";
    for (auto &instruction : instructions) {
        result += TAB + instruction->to_string() + "\n";
    }
    result += "}\n";
    return result;
}

std::string TypeVariable::to_string() {
    std::string result = ident;
    if (!implements.empty()) {
        result += " : ";
        for (int i = 0; i < implements.size(); i++) {
            result += implements[i]->to_string();
            if (i != implements.size() - 1) {
                result += ", ";
            }
        }
    }
    return result;
}

std::string TypeDecl::to_string() {
    std::string result = "type " + ident + " = " + type->to_string() + ";\n";
    return result;
}

std::string StructDecl::to_string() {
    std::string result = "struct " + ident;
    if (!generics.empty()) {
        result += "<";
        for (int i = 0; i < generics.size(); i++) {
            result += generics[i]->to_string();
            if (i != generics.size() - 1) {
                result += ", ";
            }
        }
        result += ">";
    }
    result += " {\n";
    for (auto &field : fields) {
        result += TAB + field.first + ": " + field.second->to_string() + "\n";
    }
    result += "}\n";
    return result;
}

std::string Goto::to_string() {
    return "goto " + target->ident + ";\n";
}

std::string SwitchInt::to_string() {
    std::string result = "switchInt " + value->to_string() + " {\n";
    for (auto &case_ : cases) {
        result += TAB + std::to_string(case_.first) + " => " + case_.second->ident + "\n";
    }
    result += TAB;
    result += "default => " + default_case->ident + "\n";
    result += "}\n";
    return result;
}

std::string Function::to_string() {
    std::string result = "fn " + ident;
    if (!generics.empty()) {
        result += "<";
        for (int i = 0; i < generics.size(); i++) {
            result += generics[i]->to_string();
            if (i != generics.size() - 1) {
                result += ", ";
            }
        }
        result += ">";
    }
    result += "(";
    // add comma until the last parameter
    for (int i = 0; i < params.size(); i++) {
        result += params[i].first + ": " + params[i].second->to_string();
        if (i != params.size() - 1) {
            result += ", ";
        }
    }
    result += ") -> " + return_type->to_string() + " {\n";
    for (auto &label : labels) {
        result += TAB + label->to_string() + "\n";
    }
    result += "}\n";
    return result;
}

std::string Module::to_string() {
    std::string result;
    for (auto &struct_decl : structs) {
        result += struct_decl->to_string() + "\n";
    }
    for (auto &type_decl : type_decls) {
        result += type_decl->to_string() + "\n";
    }
    for (auto &function : functions) {
        result += function->to_string() + "\n";
    }
    return result;
}

std::string IntConstant::to_string() {
    return std::to_string(value);
}

std::string BoolConstant::to_string() {
    return value ? "true" : "false";
}

std::string FloatConstant::to_string() {
    return std::to_string(value);
}

std::string StringConstant::to_string() {
    return "\"" + value + "\"";
}

std::string ArrayConstant::to_string() {
    std::string result = "[";
    for (int i = 0; i < values.size(); i++) {
        result += values[i]->to_string();
        if (i != values.size() - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}

std::string StructConstant::to_string() {
    std::string result = "{";
    for (int i = 0; i < values.size(); i++) {
        result += values[i].first + ": " + values[i].second->to_string();
        if (i != values.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

std::string BoolType::to_string() {
    return "bool";
}

std::string IntType::to_string() {
    return "i" + std::to_string(size);
}

std::string FloatType::to_string() {
    return "f" + std::to_string(size);
}

std::string PointerType::to_string() {
    return "ptr<" + base->to_string() + ">";
}

std::string ArrayType::to_string() {
    return "[" + std::to_string(size) + " : " + base->to_string() + "]";
}

std::string CharType::to_string() {
    return "char";
}

std::string VoidType::to_string() {
    return "void";
}

std::string FunctionType::to_string() {
    std::string result = "Fn";
    if (!generic_args.empty()) {
        result += "<";
        for (int i = 0; i < generic_args.size(); i++) {
            result += generic_args[i]->to_string();
            if (i != generic_args.size() - 1) {
                result += ", ";
            }
        }
        result += ">";
    }
    result += "(";
    for (int i = 0; i < arg_types.size(); i++) {
        result += arg_types[i]->to_string();
        if (i != arg_types.size() - 1) {
            result += ", ";
        }
    }
    result += ") -> " + return_type->to_string();
    return result;
}



