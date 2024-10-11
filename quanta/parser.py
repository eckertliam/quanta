from typing import List

from quanta.ast import AstNode, Loc, Delimiter, Form, Atom, Int, Float, String, Bool, Symbol

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

def parse_form(chars: Chars, delim: Delimiter) -> Form:
    start_loc = chars.loc()
    elements = []
    while not chars.eof():
        skip_whitespace(chars)
        char = chars.peek()
        if char == delim.close():
            chars.next()
            return Form(elements, delim, start_loc)
        elif Delimiter.as_delim(char) is not None:
            # consume the delimiter
            chars.next()
            elements.append(parse_form(chars, Delimiter.as_delim(char)))
        else:
            elements.append(parse_atom(chars))
    raise Exception(f'Unmatched delimiter at {start_loc}')

SYMBOL_SPECIALS = '?:!-_><=/*+'

def parse_atom(chars: Chars) -> Atom:
    start_loc = chars.loc()
    char = chars.peek()
    if char.isdigit():
        acc = chars.next()
        while not chars.eof() and chars.peek().isdigit():
            acc += chars.next()
        if not chars.eof() and chars.peek() == '.':
            acc += chars.next()
            while not chars.eof() and chars.peek().isdigit():
                acc += chars.next()
            return Float(float(acc), start_loc)
        else:
            return Int(int(acc), start_loc)
    elif char == '"':
        chars.next()
        acc = ''
        while not chars.eof() and chars.peek() != '"':
            acc += chars.next()
        chars.next()
        return String(acc, start_loc)
    elif char == '#':
        chars.next()
        if chars.peek() == 't':
            chars.next()
            return Bool(True, start_loc)
        elif chars.peek() == 'f':
            chars.next()
            return Bool(False, start_loc)
        else:
            raise Exception(f'Unknown boolean literal at {start_loc}')
    elif char.isalpha() or char in SYMBOL_SPECIALS:
        acc = chars.next()
        while not chars.eof() and (chars.peek().isalnum() or chars.peek() in SYMBOL_SPECIALS):
            acc += chars.next()
        return Symbol(acc, start_loc)
    else:
        raise Exception(f'Unknown token at {start_loc}')

def parse(src: str) -> List[AstNode]:
    chars = Chars(src)
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