from .generated import QuantaParser, QuantaLexer, QuantaVisitor
from antlr4 import *


def parse(src: str) -> QuantaParser.ProgramContext:
    lexer = QuantaLexer(InputStream(src))
    stream = CommonTokenStream(lexer)
    parser = QuantaParser(stream)
    tree = parser.program()
    return tree

def parse_debug(src: str) -> QuantaParser.ProgramContext:
    lexer = QuantaLexer(InputStream(src))
    stream = CommonTokenStream(lexer)
    parser = QuantaParser(stream)
    tree = parser.program()
    print(tree.toStringTree(recog=parser))
    return tree
