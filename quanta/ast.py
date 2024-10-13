from typing import List, Optional, Union
from enum import Enum, auto


class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col


class Node:
    def __init__(self, loc: Loc):
        self.loc = loc


class Statement(Node):
    pass

class Expression(Node):
    pass

class Type(Node):
    pass

class Block(Statement):
    def __init__(self, stmts: List[Statement], loc: Loc):
        super().__init__(loc)
        self.stmts = stmts

    def push(self, stmt: Statement):
        self.stmts.append(stmt)

class ConstDecl(Statement):
    def __init__(self, name: str, ty: Type, value: Expression, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.ty = ty
        self.value = value

class LetDecl(Statement):
    def __init__(self, name: str, ty: Type, value: Expression, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.ty = ty
        self.value = value

class VarDecl:
    @staticmethod
    def __call__(name: str, mut: bool, ty: Type, value: Expression, loc: Loc) -> Union[ConstDecl, LetDecl]:
        if mut:
            return LetDecl(name, ty, value, loc)
        else:
            return ConstDecl(name, ty, value, loc)

class TypeDecl(Statement):
    def __init__(self, name: str, generics: List[str], ty: Type, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.ty = ty

class StructField:
    def __init__(self, name: str, ty: Type, loc: Loc):
        self.name = name
        self.ty = ty
        self.loc = loc

class StructDecl(Statement):
    def __init__(self, name: str, generics: List[str], fields: List[StructField], loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.fields = fields

class FnParam:
    def __init__(self, name: str, ty: Type, loc: Loc):
        self.name = name
        self.ty = ty
        self.loc = loc

class FnDecl(Statement):
    def __init__(self, name: str, generics: List[str], params: List[FnParam], ret_ty: Type, body: Block, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.params = params
        self.ret_ty = ret_ty
        self.body = body

class IfStmt(Statement):
    def __init__(self, cond: Expression, then_block: Block, else_block: Optional[Block], loc: Loc):
        super().__init__(loc)
        self.cond = cond
        self.then_block = then_block
        self.else_block = else_block

class LambdaStmt(Statement):
    def __init__(self, params: List[FnParam], ret_ty: Type, body: Block, loc: Loc):
        super().__init__(loc)
        self.params = params
        self.ret_ty = ret_ty
        self.body = body

class ReturnStmt(Statement):
    def __init__(self, value: Expression, loc: Loc):
        super().__init__(loc)
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
    def __init__(self, kind: BinOpKind, lhs: Expression, rhs: Expression, loc: Loc):
        super().__init__(loc)
        self.kind = kind
        self.lhs = lhs
        self.rhs = rhs

class UnOpKind(Enum):
    NEG = auto()
    NOT = auto()

class UnOp(Expression):
    def __init__(self, kind: UnOpKind, expr: Expression, loc: Loc):
        super().__init__(loc)
        self.kind = kind
        self.expr = expr

class FnCall(Expression):
    def __init__(self, callee: Expression, args: List[Expression], loc: Loc):
        super().__init__(loc)
        self.callee = callee
        self.args = args

class Symbol(Expression):
    def __init__(self, name: str, loc: Loc):
        super().__init__(loc)
        self.name = name

class Int(Expression):
    def __init__(self, value: int, loc: Loc):
        super().__init__(loc)
        self.value = value

class Float(Expression):
    def __init__(self, value: float, loc: Loc):
        super().__init__(loc)
        self.value = value

class Bool(Expression):
    def __init__(self, value: bool, loc: Loc):
        super().__init__(loc)
        self.value = value

class String(Expression):
    def __init__(self, value: str, loc: Loc):
        super().__init__(loc)
        self.value = value

class Array(Expression):
    def __init__(self, elements: List[Expression], loc: Loc):
        super().__init__(loc)
        self.elements = elements

class StructLiteral(Expression):
    def __init__(self, fields: List[Expression], loc: Loc):
        super().__init__(loc)
        self.fields = fields

class FieldAccess(Expression):
    def __init__(self, expr: Expression, field: str, loc: Loc):
        super().__init__(loc)
        self.expr = expr
        self.field = field

class IndexAccess(Expression):
    def __init__(self, expr: Expression, index: Expression, loc: Loc):
        super().__init__(loc)
        self.expr = expr
        self.index = index

class ExprBlock(Expression):
    def __init__(self, expr: Expression, loc: Loc):
        super().__init__(loc)
        self.expr = expr

class IntType(Type):
    def __init__(self, size: int, loc: Loc):
        super().__init__(loc)
        self.size = size
        assert size in [8, 16, 32, 64]

class FloatType(Type):
    def __init__(self, size: int, loc: Loc):
        super().__init__(loc)
        self.size = size
        assert size in [32, 64]

class BoolType(Type):
    def __init__(self, loc: Loc):
        super().__init__(loc)

class StringType(Type):
    def __init__(self, loc: Loc):
        super().__init__(loc)

class ArrayType(Type):
    def __init__(self, elem_ty: Type, loc: Loc):
        super().__init__(loc)
        self.elem_ty = elem_ty

class StructType(Type):
    def __init__(self, name: str, generics: List[Type], loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics

class FnType(Type):
    def __init__(self, params: List[Type], ret_ty: Type, loc: Loc):
        super().__init__(loc)
        self.params = params
        self.ret_ty = ret_ty

class TypeVar(Type):
    def __init__(self, name: str, loc: Loc):
        super().__init__(loc)
        self.name = name

class VoidType(Type):
    def __init__(self, loc: Loc):
        super().__init__(loc)
