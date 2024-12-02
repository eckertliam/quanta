from dataclasses import dataclass
from typing import Optional

from quanta.parser import TokenType


@dataclass
class Node:
    pass


@dataclass
class Expr(Node):
    line: int


@dataclass
class TypeExpr(Expr):
    pass


@dataclass
class Statement(Node):
    line: int


@dataclass
class Program(Node):
    statements: list[Statement]

# Statements ========================

@dataclass
class Block(Statement, Expr):
    statements: list[Statement]


@dataclass
class FnParam(Node):
    """A function parameter"""
    name: str
    type: TypeExpr


@dataclass
class FnDef(Statement):
    """Defines a function"""
    name: str
    params: list[FnParam]
    return_type: TypeExpr
    body: Block


@dataclass
class ConstDecl(Statement):
    """Declares a constant"""
    name: str
    type: Optional[TypeExpr]
    value: Expr
    

@dataclass
class VarDecl(Statement):
    """Declares a variable"""
    name: str
    type: Optional[TypeExpr]
    value: Expr
    
    
@dataclass
class Assign(Statement):
    """Mutates a variable"""
    name: str
    value: Expr


@dataclass
class If(Statement):
    """An if statement"""
    condition: Expr
    body: Block
    else_body: Optional[Block]


@dataclass
class MatchCase(Node):
    """A match case"""
    # None if the case is a default
    pattern: Optional[Expr]
    body: Block


@dataclass
class Match(Statement):
    """A match statement"""
    value: Expr
    cases: list[MatchCase]
    

@dataclass
class Return(Statement):
    """Returns a value from a function"""
    value: Optional[Expr]


@dataclass
class StructField(Node):
    """A struct field"""
    name: str
    type: TypeExpr
    default: Optional[Expr]


@dataclass
class StructDecl(Statement):
    """Declares a struct"""
    name: str
    fields: list[StructField]
    

@dataclass
class EnumVariant(Node):
    pass


@dataclass
class UnitVariant(EnumVariant):
    name: str
    

@dataclass
class TupleVariant(EnumVariant):
    fields: list[TypeExpr]
    

@dataclass
class StructVariant(EnumVariant):
    name: str
    fields: list[StructField]


@dataclass
class EnumDecl(Statement):
    """Declares an enum"""
    name: str
    variants: list[EnumVariant]


@dataclass
class Import(Statement):
    """Imports a module"""
    # The names to import from the module
    imports: list[str]
    # The path to the module
    path: list[str]
    # The alias to use for the module
    alias: Optional[str]
    

@dataclass
class For(Statement):
    """A for loop"""
    const: str
    iterable: Expr
    body: Block


@dataclass
class While(Statement):
    """A while loop"""
    condition: Expr
    body: Block


@dataclass
class Break(Statement):
    """Breaks out of a loop"""
    pass


@dataclass
class Continue(Statement):
    """Continues a loop"""
    pass


@dataclass
class ExprStmt(Statement):
    """An expression statement"""
    expr: Expr


# Expressions ========================

@dataclass
class IntLit(Expr):
    """An integer literal"""
    value: int


@dataclass
class FloatLit(Expr):
    """A float literal"""
    value: float


@dataclass
class BoolLit(Expr):
    """A boolean literal"""
    value: bool


@dataclass
class StrLit(Expr):
    """A string literal"""
    value: str

@dataclass
class Ident(Expr):
    """An identifier"""
    name: str


@dataclass
class Call(Expr):
    """A function call"""
    callee: Expr
    args: list[Expr]


@dataclass
class ArrayLit(Expr):
    """An array literal"""
    elements: list[Expr]


@dataclass
class TupleLit(Expr):
    """A tuple literal"""
    elements: list[Expr]


@dataclass
class StructLit(Expr):
    """A struct literal"""
    name: str
    fields: list[StructField]


@dataclass
class EnumLit(Expr):
    """An enum literal"""
    name: str
    variant: EnumVariant
    

@dataclass
class FieldAccess(Expr):
    """Accesses a field of a struct or tuple"""
    value: Expr
    # the field access is an expr to handle a.b.c or a.b[0] etc
    field: Expr


@dataclass
class IndexAccess(Expr):
    """Accesses an element of an array or tuple"""
    value: Expr
    index: Expr


@dataclass
class Unary(Expr):
    """A unary expression"""
    op: TokenType
    value: Expr
    

@dataclass
class Binary(Expr):
    """A binary expression"""
    op: TokenType
    left: Expr
    right: Expr


@dataclass
class StmtExpr(Expr):
    """An expression statement"""
    stmt: Statement
    
    
# Types ==============================

@dataclass
class IntType(TypeExpr):
    """An integer type"""
    signed: bool
    size: int


@dataclass
class FloatType(TypeExpr):
    """A float type"""
    size: int


@dataclass
class BoolType(TypeExpr):
    """A boolean type"""
    pass


@dataclass
class StrType(TypeExpr):
    """A string type"""
    pass


@dataclass
class UdtType(TypeExpr):
    """A user-defined type"""
    name: str

@dataclass
class ArrayType(TypeExpr):
    """An array type"""
    element_type: TypeExpr
    size: Expr


@dataclass
class TupleType(TypeExpr):
    """A tuple type"""
    elements: list[TypeExpr]


@dataclass
class FnType(TypeExpr):
    """A function type"""
    params: list[TypeExpr]
    return_type: TypeExpr
