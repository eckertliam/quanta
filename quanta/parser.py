from typing import List

from quanta.ast import AstNode, Loc, Delimiter


class Chars:
    def __init__(self, src: str):
        self.src = src
        self.index = 0
        self.col = 1
        self.line = 1

    def peek(self) -> str:
        return self.src[self.index]

    def next(self) -> str:
        char = self.src[self.index]
        self.index += 1
        if char == '\n':
            self.line += 1
            self.col = 1
        else:
            self.col += 1
        return char

    def eof(self) -> bool:
        return self.index >= len(self.src)

    def loc(self) -> Loc:
        return Loc(self.line, self.col)


def skip_whitespace(chars: Chars):
    while not chars.eof() and chars.peek().isspace():
        chars.next()

def parse_form(chars: Chars, delim: Delimiter) -> List[AstNode]:
    skip_whitespace(chars)
    elements = []
    while not chars.eof():
        char = chars.peek()
        if char == delim.close():
            chars.next()
            return elements
        elif Delimiter.as_delim(char) is not None:
            # consume the delimiter
            chars.next()
            elements.append(parse_form(chars, Delimiter.as_delim(char)))
        else:
            elements.append(parse_atom(chars))


def parse_atom(chars: Chars) -> AstNode:
    pass

def parse(chars: Chars) -> List[AstNode]:
    nodes = []
    while not chars.eof():
        skip_whitespace(chars)
        char = chars.peek()
        if Delimiter.as_delim(char) is not None:
            # consume the delimiter
            chars.next()
            nodes.append(parse_form(chars, Delimiter.as_delim(char)))
        else:
            nodes.append(parse_atom(chars))
    return nodes