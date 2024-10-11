from typing import List, Optional
from enum import Enum, auto


class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col


class AstNode:
    def __init__(self, loc: Loc):
        self.loc = loc


class Atom(AstNode):
    def __init__(self, value: any, loc: Loc):
        super().__init__(loc)
        self.value = value

class Int(Atom):
    def __init__(self, value: int, loc: Loc):
        super().__init__(value, loc)


class Float(Atom):
    def __init__(self, value: float, loc: Loc):
        super().__init__(value, loc)


class String(Atom):
    def __init__(self, value: str, loc: Loc):
        super().__init__(value, loc)


class Bool(Atom):
    def __init__(self, value: bool, loc: Loc):
        super().__init__(value, loc)


class Symbol(Atom):
    def __init__(self, value: str, loc: Loc):
        super().__init__(value, loc)


class Delimiter(Enum):
    Paren = auto()
    Bracket = auto()
    Brace = auto()

    def open(self):
        if self == Delimiter.Paren:
            return '('
        elif self == Delimiter.Bracket:
            return '['
        elif self == Delimiter.Brace:
            return '{'

    def close(self):
        if self == Delimiter.Paren:
            return ')'
        elif self == Delimiter.Bracket:
            return ']'
        elif self == Delimiter.Brace:
            return '}'

    @staticmethod
    def as_delim(delim: str) -> Optional['Delimiter']:
        if delim == '(':
            return Delimiter.Paren
        elif delim == '[':
            return Delimiter.Bracket
        elif delim == '{':
            return Delimiter.Brace
        else:
            return None


class Form(AstNode):
    """
    A form is a list of elements
    Refined into an array, vec, function call, etc. during MIR generation
    """
    def __init__(self, elements: List[AstNode], delim: Delimiter, loc: Loc):
        super().__init__(loc)
        self.elements = elements
        self.delim = delim
