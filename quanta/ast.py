from typing import Union
from enum import Enum, auto

class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col


class AstNode:
    def __init__(self, start: Loc, end: Loc):
        self.start = start
        self.end = end
        
        
class AtomType(Enum):
    INT = auto()
    FLOAT = auto()
    STRING = auto()
    SYMBOL = auto()
    BOOL = auto()
    NULL = auto()    
    
def well_formed_atom(value: any, atom_type: AtomType) -> bool:
    if atom_type == AtomType.INT:
        return isinstance(value, int)
    elif atom_type == AtomType.FLOAT:
        return isinstance(value, float)
    elif atom_type == AtomType.STRING:
        return isinstance(value, str)
    elif atom_type == AtomType.SYMBOL:
        return isinstance(value, str)
    elif atom_type == AtomType.BOOL:
        return isinstance(value, bool)
    elif atom_type == AtomType.NULL:
        return value is None
    else:
        return False
    
class Atom(AstNode):
    def __init__(self, value: any, atom_type: AtomType, start: Loc, end: Loc):
        super().__init__(start, end)
        assert well_formed_atom(value, atom_type)
        self.value = value
        self.atom_type = atom_type
        

class List(AstNode):
    def __init__(self, elements: list, start: Loc, end: Loc):
        super().__init__(start, end)
        self.elements = elements
