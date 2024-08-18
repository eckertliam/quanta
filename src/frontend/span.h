#ifndef QUANTA_SPAN_H
#define QUANTA_SPAN_H

#include <cstdio>
#include <string>

struct Loc {
    size_t line;
    size_t column;

    Loc(size_t line, size_t column) : line(line), column(column) {};

    Loc() : line(0), column(0) {}

    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] Loc add_column(size_t n) const;
    [[nodiscard]] Loc add_line(size_t n) const;
    [[nodiscard]] Loc add_loc(Loc loc) const;
};

struct Span {
    Loc start;
    Loc end;

    Span(Loc start, Loc end) : start(start), end(end) {};

    Span() : start(), end() {}

    [[nodiscard]] std::string to_string() const;
};

#endif //QUANTA_SPAN_H
