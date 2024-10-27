from dataclasses import dataclass
from enum import Enum, auto
from typing import Optional
@dataclass
class Loc:
    col: int
    line: int


class TokenType(Enum):
    EOF = auto()
    NEWLINE = auto()
    INDENT = auto()
    DEDENT = auto()
    SYMBOL = auto()
    INTEGER = auto()
    FLOAT = auto()
    STRING = auto()
    BOOLEAN = auto()
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
    ARROW = auto()
    FAT_ARROW = auto()
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    PERCENT = auto()
    AMP = auto()
    AMP_AMP = auto()
    PIPE = auto()
    PIPE_PIPE = auto()
    TILDE = auto()
    CARET = auto()
    EQ = auto()
    EQ_EQ = auto()
    BANG = auto()
    BANG_EQ = auto()
    LT = auto()
    LT_EQ = auto()
    LT_LT = auto()
    GT = auto()
    GT_EQ = auto()
    GT_GT = auto()
    UNDERSCORE = auto()
    ERROR = auto()

@dataclass
class Token:
    type: TokenType
    loc: Loc
    value: Optional[str]


@dataclass
class Tokenizer:
    src: str
    line: int = 1
    col: int = 1
    idx: int = 0


def tokenizer(src: str) -> Tokenizer:
    return Tokenizer(src)


def peek(tokenizer: Tokenizer, offset: int = 0) -> Token:
    return tokenizer.src[tokenizer.idx + offset]


def consume(tokenizer: Tokenizer) -> Token:
    token = peek(tokenizer)
    tokenizer.idx += 1
    if token == "\n":
        tokenizer.line += 1
        tokenizer.col = 1
    else:
        tokenizer.col += 1
    return token


def take(tokenizer: Tokenizer, start: int, end: int) -> str:
    return tokenizer.src[start:end]


def read_number(tokenizer: Tokenizer) -> Token:
    start_loc = Loc(tokenizer.col, tokenizer.line)
    start_idx = tokenizer.idx
    while peek(tokenizer).isdigit():
        consume(tokenizer)
    if peek(tokenizer) == ".":
        consume(tokenizer)
        while peek(tokenizer).isdigit():
            consume(tokenizer)
        return Token(TokenType.FLOAT, start_loc, take(tokenizer, start_idx, tokenizer.idx))
    else:
        return Token(TokenType.INTEGER, start_loc, take(tokenizer, start_idx, tokenizer.idx))


