from dataclasses import dataclass
from typing import List, Tuple, Optional, Union
from quanta.tokenizer import Loc


@dataclass
class Node:
    loc: Loc


@dataclass
class Expression(Node):
    pass


@dataclass
class Statement(Node):
    pass


@dataclass
class TypeAnnotation(Node):
    pass


@dataclass
class Program(Node):
    statements: List[Statement]


@dataclass
class Block(Node):
    statements: List[Statement]


FnParam = Tuple[str, Optional[TypeAnnotation]]


@dataclass
class FnDef(Statement):
    name: str
    params: List[FnParam]
    body: Block


@dataclass
class VarDef(Statement):
    mut: bool
    name: str
    ty: Optional[TypeAnnotation]
    value: Expression


@dataclass
class If(Statement):
    cond: Expression
    body: Block
    else_body: Optional[Block]


@dataclass
class Match(Statement):
    expr: Expression
    cases: List[Tuple[Expression, Block]]



@dataclass
class While(Statement):
    cond: Expression
    body: Block


@dataclass
class Symbol(Expression):
    name: str

@dataclass
class Integer(Expression):
    value: int


@dataclass
class Float(Expression):
    value: float


@dataclass
class String(Expression):
    value: str


@dataclass
class Boolean(Expression):
    value: bool


@dataclass
class Array(Expression):
    values: List[Expression]


@dataclass
class ObjectField(Expression):
    name: Optional[str]
    value: Expression


@dataclass
class Object(Expression):
    fields: List[ObjectField]


@dataclass
class Array(Expression):
    values: List[Expression]


@dataclass
class FieldAccess(Expression):
    record: Expression
    field: Expression


@dataclass
class Call(Expression):
    callee: Expression
    args: List[Expression]


@dataclass
class Index(Expression):
    array: Expression
    index: Expression
