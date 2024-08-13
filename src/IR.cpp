#include "../include/IR.h"

#define QUANTA_IR_CPP

const char *TAB = "    ";

std::string Label::to_string() {
    std::string result = "label " + ident + "{\n";
    for (auto &instruction : instructions) {
        result += TAB + instruction->to_string() + "\n";
    }
    result += "}";
}

std::string StructDecl::to_string() {
    std::string result = "struct " + ident + " {\n";
    for (auto &field : fields) {
        result += TAB + field.first + ": " + field.second->to_string() + "\n";
    }
    result += "}";
    return result;
}

std::string Goto::to_string() {
    return "goto " + target->ident;
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
    std::string result = "fn " + ident + "(";
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