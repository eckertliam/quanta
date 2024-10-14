from enum import Enum, auto
from typing import List, Optional, Union


class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col

    def __str__(self):
        return f"{self.line}:{self.col}"

    def __repr__(self):
        return str(self)


class Span:
    def __init__(self, start: Loc, end: Loc):
        self.start = start
        self.end = end

    def __str__(self):
        return f"{self.start} - {self.end}"

    def __repr__(self):
        return str(self)


class Node:
    def __init__(self, start: Loc, end: Loc):
        self.span = Span(start, end)

class Program:
    def __init__(self, stmts: List['Statement']):
        self.stmts = stmts

    def push(self, stmt: 'Statement'):
        self.stmts.append(stmt)

class Statement(Node):
    pass


class Expression(Node):
    pass


class Type(Node):
    pass


class Block(Statement):
    def __init__(self, start: Loc, end: Loc, stmts: List[Statement]):
        super().__init__(start, end)
        self.stmts = stmts


class ConstDecl(Statement):
    def __init__(self, name: str, ty: Type, value: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.pub = pub
        self.name = name
        self.ty = ty
        self.value = value


class LetDecl(Statement):
    def __init__(self, name: str, ty: Type, value: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.ty = ty
        self.value = value


class VarDecl:
    @staticmethod
    def init(name: str, mut: bool, ty: Type, value: Expression, start: Loc, end: Loc) -> Union[ConstDecl, LetDecl]:
        if mut:
            return LetDecl(name, ty, value, start, end)
        else:
            return ConstDecl(name, ty, value, start, end)


class TypeDecl(Statement):
    def __init__(self, name: str, generics: List[str], ty: Type, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.generics = generics
        self.ty = ty


class StructField(Node):
    def __init__(self, name: str, ty: Type, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.ty = ty


class StructDecl(Statement):
    def __init__(self, name: str, generics: List[str], fields: List[StructField], start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.generics = generics
        self.fields = fields


class FnParam(Node):
    def __init__(self, name: str, ty: Type, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.ty = ty
        self.start = start

class FnDecl(Statement):
    def __init__(self, name: str, generics: List[str], params: List[FnParam], ret_ty: Type, body: Block, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.generics = generics
        self.params = params
        self.ret_ty = ret_ty
        self.body = body


class IfStmt(Statement):
    def __init__(self, cond: Expression, then_block: Block, else_block: Optional[Block], start: Loc, end: Loc):
        super().__init__(start, end)
        self.cond = cond
        self.then_block = then_block
        self.else_block = else_block


class LambdaStmt(Statement):
    def __init__(self, params: List[FnParam], ret_ty: Type, body: Block, start: Loc, end: Loc):
        super().__init__(start, end)
        self.params = params
        self.ret_ty = ret_ty
        self.body = body


class ReturnStmt(Statement):
    def __init__(self, value: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.value = value


class BinOpKind(Enum):
    ADD = auto()
    SUB = auto()
    MUL = auto()
    DIV = auto()
    MOD = auto()
    AND = auto()
    OR = auto()
    EQ = auto()
    NE = auto()
    LT = auto()
    LE = auto()
    GT = auto()
    GE = auto()


class BinOp(Expression):
    def __init__(self, kind: BinOpKind, lhs: Expression, rhs: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.kind = kind
        self.lhs = lhs
        self.rhs = rhs


class UnOpKind(Enum):
    NEG = auto()
    NOT = auto()


class UnOp(Expression):
    def __init__(self, kind: UnOpKind, expr: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.kind = kind
        self.expr = expr


class FnCall(Expression):
    def __init__(self, callee: Expression, args: List[Expression], start: Loc, end: Loc):
        super().__init__(start, end)
        self.callee = callee
        self.args = args


class Symbol(Expression):
    def __init__(self, name: str, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name


class Int(Expression):
    def __init__(self, value: int, start: Loc, end: Loc):
        super().__init__(start, end)
        self.value = value


class Float(Expression):
    def __init__(self, value: float, start: Loc, end: Loc):
        super().__init__(start, end)
        self.value = value


class Bool(Expression):
    def __init__(self, value: bool, start: Loc, end: Loc):
        super().__init__(start, end)
        self.value = value


class String(Expression):
    def __init__(self, value: str, start: Loc, end: Loc):
        super().__init__(start, end)
        self.value = value


class Array(Expression):
    def __init__(self, elements: List[Expression], start: Loc, end: Loc):
        super().__init__(start, end)
        self.elements = elements


class StructLiteral(Expression):
    def __init__(self, fields: List[Expression], start: Loc, end: Loc):
        super().__init__(start, end)
        self.fields = fields


class FieldAccess(Expression):
    def __init__(self, expr: Expression, field: str, start: Loc, end: Loc):
        super().__init__(start, end)
        self.expr = expr
        self.field = field


class IndexAccess(Expression):
    def __init__(self, expr: Expression, index: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.expr = expr
        self.index = index


class ExprBlock(Expression):
    def __init__(self, expr: Expression, start: Loc, end: Loc):
        super().__init__(start, end)
        self.expr = expr


class IntType(Type):
    def __init__(self, size: int, start: Loc, end: Loc):
        super().__init__(start, end)
        self.size = size
        assert size in [8, 16, 32, 64]


class FloatType(Type):
    def __init__(self, size: int, start: Loc, end: Loc):
        super().__init__(start, end)
        self.size = size
        assert size in [32, 64]


class BoolType(Type):
    def __init__(self, start: Loc, end: Loc):
        super().__init__(start, end)


class StringType(Type):
    def __init__(self, start: Loc, end: Loc):
        super().__init__(start, end)


class ArrayType(Type):
    def __init__(self, elem_ty: Type, start: Loc, end: Loc):
        super().__init__(start, end)
        self.elem_ty = elem_ty


class StructType(Type):
    def __init__(self, name: str, generics: List[Type], start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name
        self.generics = generics


class FnType(Type):
    def __init__(self, params: List[Type], ret_ty: Type, start: Loc, end: Loc):
        super().__init__(start, end)
        self.params = params
        self.ret_ty = ret_ty


class TypeVar(Type):
    def __init__(self, name: str, start: Loc, end: Loc):
        super().__init__(start, end)
        self.name = name


class VoidType(Type):
    def __init__(self, start: Loc, end: Loc):
        super().__init__(start, end)
