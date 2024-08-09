from typing import List, Union, Optional, Dict
# Contains an SSA IR for the compiler

class IRVisitor:
    def visit(self, node):
        pass


class IRNode:
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Context(IRNode):
    def __init__(self):
        self.parent: Optional[Context] = None
        self.types: Dict[str, Type] = []
        self.functions: Dict[str, Function] = {}
        self.current_function: Optional[Function] = None

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Type(IRNode):
    pass

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)

    def type_judge(self, constant: 'Constant'):
        pass


class IntType(Type):
    def __init__(self, size: int):
        if size not in [8, 16, 32, 64]:
            raise ValueError("Invalid size for int type")
        self.size: int = size

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class FloatType(Type):
    def __init__(self, size: int):
        if size not in [32, 64]:
            raise ValueError("Invalid size for float type")
        self.size: int = size

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class BoolType(Type):
    def __init__(self):
        pass

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Function(IRNode):
    def __init__(self, ident: str, return_type: Type, arguments: List[Type]):
        self.ident: str = ident
        self.return_type: Type = return_type
        self.arguments: List[Type] = arguments
        self.body: List[Instruction] = []

    def accept(self, visitor: IRVisitor):
        (visitor.visit(self))


class Instruction(IRNode):
    pass

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Constant(Instruction):
    def type_judge(self, _type: Type):
        _type.type_judge(self)
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)