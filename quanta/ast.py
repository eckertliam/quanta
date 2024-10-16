from typing import List
from enum import Enum, auto

class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col

    def __str__(self):
        return f"{self.line}:{self.col}"
    

class Span:
    def __init__(self, start: Loc, end: Loc):
        self.start = start
        self.end = end

    def __str__(self):
        return f"{self.start} - {self.end}"


class Expr:
    def __init__(self, span: Span):
        self.span = span


class Atom(Expr):
    pass


class Symbol(Atom):
    def __init__(self, span: Span, value: str):
        super().__init__(span)
        self.value = value


class Integer(Atom):
    def __init__(self, span: Span, value: int):
        super().__init__(span)
        self.value = value


class Float(Atom):
    def __init__(self, span: Span, value: float):
        super().__init__(span)
        self.value = value


class String(Atom):
    def __init__(self, span: Span, value: str):
        super().__init__(span)
        self.value = value


class Boolean(Atom):
    def __init__(self, span: Span, value: bool):
        super().__init__(span)
        self.value = value


class Nil(Atom):
    def __init__(self, span: Span):
        super().__init__(span)

class DelimKind(Enum):
    # ()
    PAREN = auto()
    # []
    BRACKET = auto()
    # {}
    BRACE = auto()

class Delim(Expr):
    def __init__(self, span: Span, elements: List[Expr], kind: DelimKind):
        super().__init__(span)
        self.elements = elements
        self.kind = kind

class Program:
    """
    Top-level program structure.
    """
    def __init__(self, body: List[Expr]=[]):
        self.body = body
