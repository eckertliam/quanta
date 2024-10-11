from typing import List, Optional, Union
from enum import Enum, auto


class Loc:
    def __init__(self, line: int, col: int):
        self.line = line
        self.col = col


class AstNode:
    def __init__(self, loc: Loc):
        self.loc = loc


class AstStmt(AstNode):
    pass

class AstExpr(AstNode):
    pass

class AstType(AstNode):
    pass

class AstBlock(AstStmt):
    def __init__(self, stmts: List[AstStmt], loc: Loc):
        super().__init__(loc)
        self.stmts = stmts

    def push(self, stmt: AstStmt):
        self.stmts.append(stmt)

class AstConstDecl(AstStmt):
    def __init__(self, name: str, ty: AstType, value: AstExpr, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.ty = ty
        self.value = value

class AstLetDecl(AstStmt):
    def __init__(self, name: str, ty: AstType, value: AstExpr, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.ty = ty
        self.value = value

class AstVarDecl:
    @staticmethod
    def __call__(name: str, mut: bool, ty: AstType, value: AstExpr, loc: Loc) -> Union[AstConstDecl, AstLetDecl]:
        if mut:
            return AstLetDecl(name, ty, value, loc)
        else:
            return AstConstDecl(name, ty, value, loc)

class AstTypeDecl(AstStmt):
    def __init__(self, name: str, generics: List[str], ty: AstType, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.ty = ty

class AstStructField:
    def __init__(self, name: str, ty: AstType, loc: Loc):
        self.name = name
        self.ty = ty
        self.loc = loc

class AstStructDecl(AstStmt):
    def __init__(self, name: str, generics: List[str], fields: List[AstStructField], loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.fields = fields

class AstFnParam:
    def __init__(self, name: str, ty: AstType, loc: Loc):
        self.name = name
        self.ty = ty
        self.loc = loc

class AstFnDecl(AstStmt):
    def __init__(self, name: str, generics: List[str], params: List[AstFnParam], ret_ty: AstType, body: AstBlock, loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics
        self.params = params
        self.ret_ty = ret_ty
        self.body = body

class AstIfStmt(AstStmt):
    def __init__(self, cond: AstExpr, then_block: AstBlock, else_block: Optional[AstBlock], loc: Loc):
        super().__init__(loc)
        self.cond = cond
        self.then_block = then_block
        self.else_block = else_block

class AstLambdaStmt(AstStmt):
    def __init__(self, params: List[AstFnParam], ret_ty: AstType, body: AstBlock, loc: Loc):
        super().__init__(loc)
        self.params = params
        self.ret_ty = ret_ty
        self.body = body

class AstReturnStmt(AstStmt):
    def __init__(self, value: AstExpr, loc: Loc):
        super().__init__(loc)
        self.value = value

class AstBinOpKind(Enum):
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

class AstBinOp(AstExpr):
    def __init__(self, kind: AstBinOpKind, lhs: AstExpr, rhs: AstExpr, loc: Loc):
        super().__init__(loc)
        self.kind = kind
        self.lhs = lhs
        self.rhs = rhs

class AstUnOpKind(Enum):
    NEG = auto()
    NOT = auto()

class AstUnOp(AstExpr):
    def __init__(self, kind: AstUnOpKind, expr: AstExpr, loc: Loc):
        super().__init__(loc)
        self.kind = kind
        self.expr = expr

class AstCall(AstExpr):
    def __init__(self, callee: AstExpr, args: List[AstExpr], loc: Loc):
        super().__init__(loc)
        self.callee = callee
        self.args = args

class AstSymbol(AstExpr):
    def __init__(self, name: str, loc: Loc):
        super().__init__(loc)
        self.name = name

class AstInt(AstExpr):
    def __init__(self, value: int, loc: Loc):
        super().__init__(loc)
        self.value = value

class AstFloat(AstExpr):
    def __init__(self, value: float, loc: Loc):
        super().__init__(loc)
        self.value = value

class AstBool(AstExpr):
    def __init__(self, value: bool, loc: Loc):
        super().__init__(loc)
        self.value = value

class AstString(AstExpr):
    def __init__(self, value: str, loc: Loc):
        super().__init__(loc)
        self.value = value

class AstArray(AstExpr):
    def __init__(self, elements: List[AstExpr], loc: Loc):
        super().__init__(loc)
        self.elements = elements

class AstStructLiteral(AstExpr):
    def __init__(self, fields: List[AstExpr], loc: Loc):
        super().__init__(loc)
        self.fields = fields

class AstFieldAccess(AstExpr):
    def __init__(self, expr: AstExpr, field: str, loc: Loc):
        super().__init__(loc)
        self.expr = expr
        self.field = field

class AstIndexAccess(AstExpr):
    def __init__(self, expr: AstExpr, index: AstExpr, loc: Loc):
        super().__init__(loc)
        self.expr = expr
        self.index = index

class AstExprBlock(AstExpr):
    def __init__(self, expr: AstExpr, loc: Loc):
        super().__init__(loc)
        self.expr = expr

class AstTypeInt(AstType):
    def __init__(self, size: int, loc: Loc):
        super().__init__(loc)
        self.size = size
        assert size in [8, 16, 32, 64]

class AstTypeFloat(AstType):
    def __init__(self, size: int, loc: Loc):
        super().__init__(loc)
        self.size = size
        assert size in [32, 64]

class AstTypeBool(AstType):
    def __init__(self, loc: Loc):
        super().__init__(loc)

class AstTypeString(AstType):
    def __init__(self, loc: Loc):
        super().__init__(loc)

class AstTypeArray(AstType):
    def __init__(self, elem_ty: AstType, loc: Loc):
        super().__init__(loc)
        self.elem_ty = elem_ty

class AstTypeStruct(AstType):
    def __init__(self, name: str, generics: List[AstType], loc: Loc):
        super().__init__(loc)
        self.name = name
        self.generics = generics

class AstTypeFn(AstType):
    def __init__(self, params: List[AstType], ret_ty: AstType, loc: Loc):
        super().__init__(loc)
        self.params = params
        self.ret_ty = ret_ty

class AstTypeGeneric(AstType):
    def __init__(self, name: str, loc: Loc):
        super().__init__(loc)
        self.name = name

class AstTypeVoid(AstType):
    def __init__(self, loc: Loc):
        super().__init__(loc)

class AstTypeAny(AstType):
    def __init__(self, loc: Loc):
        super().__init__(loc)
