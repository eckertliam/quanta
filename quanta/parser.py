import quanta.ast as ast
from quanta.ast import Loc
from quanta.lexer import Token, TokenKind, TokenIter
from typing import List, Optional

def parse_var_decl(tokens: TokenIter, pub: bool, mut: bool) -> ast.Statement:
    start_loc = tokens.peek().loc
    try:
        name = tokens.match(TokenKind.SYMBOL).lexeme
    except:
        token = tokens.peek()
        raise Exception(f"Expected symbol, got {token}")
    ty: Optional[ast.Type] = None
    if tokens.match(TokenKind.COLON):
        ty = parse_type(tokens)
    value = None
    if tokens.match(TokenKind.EQUAL):
        value = parse_expr(tokens)
    return ast.VarDecl.init(name, mut, ty, value, start_loc)

def parse_stmt(tokens: TokenIter) -> ast.Statement:
    # is it prefixed with pub?
    pub = None != tokens.match(TokenKind.PUB)
    if tokens.match(TokenKind.CONST):
        return parse_var_decl(tokens, pub, False)
    elif tokens.match(TokenKind.LET):
        return parse_var_decl(tokens, pub, True)

def parse_type(tokens: TokenIter) -> ast.Type:
    # TODO: unimplemented
    pass

def parse_expr(tokens: TokenIter) -> ast.Expression:
    # TODO: unimplemented
    pass

def parse(tokens: TokenIter) -> ast.Program:
    prog = ast.Program([])
    while not tokens.eof():
        stmt = parse_stmt(tokens)
        prog.push(stmt)
