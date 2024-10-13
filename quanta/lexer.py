from dataclasses import dataclass
from enum import Enum, auto
from typing import Optional, List, Tuple

from quanta.ast import Loc


class TokenKind(Enum):
    # Special tokens
    EOF = auto()
    ERROR = auto()
    # Literals
    SYMBOL = auto()
    INT = auto()
    FLOAT = auto()
    STRING = auto()
    CHAR = auto()
    # Keywords
    FN = auto()
    STRUCT = auto()
    ENUM = auto()
    TRAIT = auto()
    IMPL = auto()
    IF = auto()
    ELSE = auto()
    MATCH = auto()
    RETURN = auto()
    WHILE = auto()
    FOR = auto()
    BREAK = auto()
    CONTINUE = auto()
    CONST = auto()
    LET = auto()
    TYPE = auto()
    TRUE = auto()
    FALSE = auto()
    # Regular tokens
    LPAREN = auto()
    RPAREN = auto()
    LBRACE = auto()
    RBRACE = auto()
    LBRACKET = auto()
    RBRACKET = auto()
    COMMA = auto()
    COLON = auto()
    COLON_COLON = auto()
    DOT = auto()
    ELLIPSIS = auto()
    SEMICOLON = auto()
    EQUAL = auto()
    FAT_ARROW = auto()
    ARROW = auto()
    PLUS = auto()
    PLUS_EQ = auto()
    MINUS = auto()
    MINUS_EQ = auto()
    STAR = auto()
    STAR_EQ = auto()
    SLASH = auto()
    SLASH_EQ = auto()
    PERCENT = auto()
    PERCENT_EQ = auto()
    AMPER = auto()
    AMPER_EQ = auto()
    AMPER_AMPER = auto()
    PIPE = auto()
    PIPE_EQ = auto()
    PIPE_PIPE = auto()
    EQ_EQ = auto()
    BANG = auto()
    BANG_EQ = auto()
    LT = auto()
    LT_EQ = auto()
    GT = auto()
    GT_EQ = auto()
    LT_LT = auto()
    GT_GT = auto()

    def __str__(self):
        return self.name.lower()

    def __repr__(self):
        return str(self)


@dataclass
class Token:
    lexeme: Optional[str]
    kind: TokenKind
    loc: Loc

    @staticmethod
    def simple(kind: TokenKind, loc: Loc) -> 'Token':
        return Token(None, kind, loc)

    def __str__(self):
        if self.lexeme is not None:
            return f"{self.kind}({self.lexeme}) at {self.loc}"
        return f"{self.kind} at {self.loc}"

    def __repr__(self):
        return str(self)


class Chars:
    def __init__(self, src: str):
        self.src = src
        self.idx = 0
        self.line = 1
        self.col = 1

    def is_end(self) -> bool:
        return self.idx >= len(self.src)

    def peek(self, offset: Optional[int] = None) -> Optional[str]:
        if self.is_end():
            return None
        elif offset is not None:
            return self.src[self.idx + offset]
        else:
            return self.src[self.idx]

    def next(self) -> Optional[str]:
        if self.is_end():
            return None
        c = self.src[self.idx]
        self.idx += 1
        if c == '\n':
            self.line += 1
            self.col = 1
        else:
            self.col += 1
        return c

    def skip_whitespace(self):
        while not self.is_end() and self.peek().isspace():
            self.next()

    def get_loc(self) -> Loc:
        return Loc(self.line, self.col)


def lex_symbol(chars: Chars) -> str:
    symbol = chars.peek()
    while not chars.is_end() and chars.peek().isalnum() or chars.peek() == '_':
        symbol += chars.next()
    return symbol


def lex_int(chars: Chars) -> str:
    integer = chars.peek()
    while not chars.is_end() and chars.peek().isdigit():
        integer += chars.next()
    return integer


def lex_number(chars: Chars) -> Tuple[str, TokenKind]:
    integer = lex_int(chars)
    if chars.peek() == '.' and chars.peek(1).isdigit():
        chars.next()
        decimal = lex_int(chars)
        return f"{integer}.{decimal}", TokenKind.FLOAT
    return integer, TokenKind.INT


def lex_string(chars: Chars) -> str:
    string = ""
    while not chars.is_end() and chars.peek() != '"':
        if chars.peek() == '\\':
            chars.next()
            if chars.peek() == 'n':
                string += '\n'
            elif chars.peek() == 't':
                string += '\t'
            else:
                string += chars.next()
        string += chars.next()
    chars.next()
    return string


KEYWORDS = {
    "fn": TokenKind.FN,
    "struct": TokenKind.STRUCT,
    "enum": TokenKind.ENUM,
    "trait": TokenKind.TRAIT,
    "impl": TokenKind.IMPL,
    "if": TokenKind.IF,
    "else": TokenKind.ELSE,
    "match": TokenKind.MATCH,
    "return": TokenKind.RETURN,
    "while": TokenKind.WHILE,
    "for": TokenKind.FOR,
    "break": TokenKind.BREAK,
    "continue": TokenKind.CONTINUE,
    "const": TokenKind.CONST,
    "let": TokenKind.LET,
    "type": TokenKind.TYPE,
    "true": TokenKind.TRUE,
    "false": TokenKind.FALSE,
}


def lex(src: str) -> List[Token]:
    chars = Chars(src)
    tokens = []
    start_loc = chars.get_loc()

    def push(tk: TokenKind, l: Optional[str]) -> None:
        tokens.append(Token(l, tk, start_loc))

    def push_simple(tk: TokenKind) -> None:
        chars.next()
        push(tk, None)

    while not chars.is_end():
        chars.skip_whitespace()
        char = chars.peek()
        start_loc = chars.get_loc()
        match char:
            case None:
                break
            case '(':
                push_simple(TokenKind.LPAREN)
            case ')':
                push_simple(TokenKind.RPAREN)
            case '{':
                push_simple(TokenKind.LBRACE)
            case '}':
                push_simple(TokenKind.RBRACE)
            case '[':
                push_simple(TokenKind.LBRACKET)
            case ']':
                push_simple(TokenKind.RBRACKET)
            case ',':
                push_simple(TokenKind.COMMA)
            case ':':
                if chars.peek(1) == ':':
                    chars.next()
                    push(TokenKind.COLON_COLON, None)
                else:
                    push_simple(TokenKind.COLON)
            case '.':
                if chars.peek(1) == '.' and chars.peek(2) == '.':
                    chars.next()
                    chars.next()
                    push(TokenKind.ELLIPSIS, None)
                else:
                    push_simple(TokenKind.DOT)
            case ';':
                push_simple(TokenKind.SEMICOLON)
            case '=':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.EQ_EQ, None)
                elif chars.peek(1) == '>':
                    chars.next()
                    push(TokenKind.FAT_ARROW, None)
                else:
                    push_simple(TokenKind.EQUAL)
            case '+':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.PLUS_EQ, None)
                else:
                    push_simple(TokenKind.PLUS)
            case '-':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.MINUS_EQ, None)
                elif chars.peek(1) == '>':
                    chars.next()
                    push(TokenKind.ARROW, None)
                else:
                    push_simple(TokenKind.MINUS)
            case '*':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.STAR_EQ, None)
                else:
                    push_simple(TokenKind.STAR)
            case '/':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.SLASH_EQ, None)
                else:
                    push_simple(TokenKind.SLASH)
            case '%':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.PERCENT_EQ, None)
                else:
                    push_simple(TokenKind.PERCENT)
            case '&':
                if chars.peek(1) == '&':
                    chars.next()
                    push(TokenKind.AMPER_AMPER, None)
                elif chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.AMPER_EQ, None)
                else:
                    push_simple(TokenKind.AMPER)
            case '|':
                if chars.peek(1) == '|':
                    chars.next()
                    push(TokenKind.PIPE_PIPE, None)
                elif chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.PIPE_EQ, None)
                else:
                    push_simple(TokenKind.PIPE)
            case '!':
                if chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.BANG_EQ, None)
                else:
                    push_simple(TokenKind.BANG)
            case '<':
                if chars.peek(1) == '<':
                    chars.next()
                    push(TokenKind.LT_LT, None)
                elif chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.LT_EQ, None)
                else:
                    push_simple(TokenKind.LT)
            case '>':
                if chars.peek(1) == '>':
                    chars.next()
                    push(TokenKind.GT_GT, None)
                elif chars.peek(1) == '=':
                    chars.next()
                    push(TokenKind.GT_EQ, None)
                else:
                    push_simple(TokenKind.GT)
            case c if c.isalpha() or c == '_':
                symbol = lex_symbol(chars)
                kind = KEYWORDS.get(symbol, TokenKind.SYMBOL)
                push(kind, symbol)
            case c if c.isdigit():
                lexeme, kind = lex_number(chars)
                push(kind, lexeme)
            case '"':
                chars.next()
                string = lex_string(chars)
                if len(string) == 1:
                    push(TokenKind.CHAR, string)
                else:
                    push(TokenKind.STRING, string)
            case _:
                push(TokenKind.ERROR, f"[-] Unexpected character: {chars.next()}")
    return tokens
