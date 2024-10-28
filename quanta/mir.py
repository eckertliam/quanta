# a minimal IR before lowering to LLVM

from typing import List, Optional


class Node:
    pass


class Instruction(Node):
    pass


class Function(Node):
    name: str
    params: List[str]
    body: List['BasicBlock']


class BasicBlock(Node): 
    name: str
    instructions: List[Instruction]


class Branch(Instruction):
    target: BasicBlock


class Call(Instruction):
    func: Function
    args: List[str]


class Ret(Instruction):
    value: Optional[str]


class Assign(Instruction):
    lhs: str
    rhs: Instruction


