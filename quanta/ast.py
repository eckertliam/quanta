from typing import Union, List
from enum import Enum, auto


class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col


class ExprType(Enum):
    INT = auto()
    FLOAT = auto()
    STRING = auto()
    SYMBOL = auto()
    BOOL = auto()
    LIST = auto()
    NULL = auto()
    

ExprValue = Union[int, float, str, bool, None, List['Expr']]
    

class Expr:
    def __init__(self, value: ExprValue, expr_type: ExprType, start: Loc, end: Loc):
        self.start = start
        self.end = end
        self.value = value
        self.expr_type = expr_type
        
    def __str__(self):
        if self.expr_type == ExprType.LIST:
            return f"({' '.join(map(str, self.value.elements))})"
        elif self.expr_type == ExprType.STRING:
            return f'"{self.value.value}"'
        else:
            return str(self.value.value)
    