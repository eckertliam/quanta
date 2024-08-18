#include <iostream>
#include "frontend/lexer.h"
#include "frontend/token.h"

int main() {
    std::string src = "record { x: i32; y: i32; }";
    Lexer lexer(src);
    Token token = lexer.next_token();
    while (token.type != TokenType::EOF_) {
        std::cout << token.to_string() << std::endl;
        token = lexer.next_token();
    }
    return 0;
}