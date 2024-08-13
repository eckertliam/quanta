//
// Created by Liam Eckert on 8/12/24.
//

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