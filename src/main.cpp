#include <iostream>
#include "frontend/lexer.h"
#include "frontend/token.h"
#include "frontend/parser.h"

int main() {
    std::string src = "record Point { x: Int, y: Int }";
    Parser parser(src);
    auto program = parser.parse_program();
    std::cout << program.body.size() << std::endl;
    return 0;
}