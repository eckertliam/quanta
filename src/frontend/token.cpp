#include "token.h"

std::string tok_ty_to_string(TokenType type) {
    switch (type) {
        case TokenType::L_PAREN:
            return "L_PAREN";
        case TokenType::R_PAREN:
            return "R_PAREN";
        case TokenType::L_BRACE:
            return "L_BRACE";
        case TokenType::R_BRACE:
            return "R_BRACE";
        case TokenType::L_BRACKET:
            return "L_BRACKET";
        case TokenType::R_BRACKET:
            return "R_BRACKET";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        case TokenType::LT:
            return "LT";
        case TokenType::GT:
            return "GT";
        case TokenType::LE:
            return "LE";
        case TokenType::GE:
            return "GE";
        case TokenType::EQ:
            return "EQ";
        case TokenType::NE:
            return "NE";
        case TokenType::ASSIGN:
            return "ASSIGN";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::STAR:
            return "STAR";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::PERCENT:
            return "PERCENT";
        case TokenType::BANG:
            return "BANG";
        case TokenType::AMP:
            return "AMP";
        case TokenType::PIPE:
            return "PIPE";
        case TokenType::CARET:
            return "CARET";
        case TokenType::ARROW:
            return "ARROW";
        case TokenType::TYPE:
            return "TYPE";
        case TokenType::FN:
            return "FN";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::EOF_:
            return "EOF";
        case TokenType::ERROR:
            return "ERROR";
    }
}

std::string Token::to_string() const {
    if (lexeme.empty()) {
        return tok_ty_to_string(type) + " " + span.to_string();
    } else {
        return tok_ty_to_string(type) + " " + std::string(lexeme) + " " + span.to_string();
    }

}