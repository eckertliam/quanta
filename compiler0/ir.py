from typing import List, Union, Optional, Dict
# Contains an SSA IR for the compiler


class IRVisitor:
    def visit(self, node):
        pass


class IRNode:
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Type(IRNode):
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


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
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class StringType(Type):
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class CharType(Type):
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class ArrayType(Type):
    def __init__(self, element_type: Type, size: int):
        self.element_type: Type = element_type
        self.size: int = size

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class PointerType(Type):
    def __init__(self, pointer_type: Type):
        self.pointer_type: Type = pointer_type

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class FunctionType(Type):
    def __init__(self, return_type: Type, arguments: List[Type]):
        self.return_type: Type = return_type
        self.arguments: List[Type] = arguments

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Variable(IRNode):
    def __init__(self, ident: str, _type: Type, value: Optional['Const'] = None):
        self.ident: str = ident
        self.type: Type = _type
        self.value: Optional[Const] = value

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
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class Const(IRNode):
    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class IntConst(Const):
    def __init__(self, value: int, size: int):
        self.value: int = value
        self.size: int = size

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class FloatConst(Const):
    def __init__(self, value: float, size: int):
        self.value: float = value
        self.size: int = size

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


class BoolConst(IntConst):
    def __init__(self, value: bool):
        super().__init__(1 if value else 0, 1)

    def accept(self, visitor: IRVisitor):
        visitor.visit(self)


