#include "span.h"

std::string Loc::to_string() const {
    return std::to_string(line) + ":" + std::to_string(column);
}

Loc Loc::add_column(size_t n) const {
    return {line, column + n};
}

Loc Loc::add_line(size_t n) const {
    return {line + n, column};
}

Loc Loc::add_loc(Loc loc) const {
    return {line + loc.line, column + loc.column};
}

std::string Span::to_string() const {
    return start.to_string() + "-" + end.to_string();
}