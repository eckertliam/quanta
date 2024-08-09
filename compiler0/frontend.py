from .generated import QuantaVisitor, QuantaParser, QuantaLexer
from antlr4 import *


def parse(src: str) -> QuantaParser.ProgramContext:
    lexer = QuantaLexer(StdinStream(src))
    stream = CommonTokenStream(lexer)
    parser = QuantaParser(stream)
    tree = parser.program()
    return tree
