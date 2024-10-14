import quanta.ast as ast
from quanta.ast import Loc
from quanta.lexer import Token, TokenKind, TokenIter
from typing import List, Optional

def parse_var_decl(tokens: TokenIter, mut: bool) -> ast.Statement:
    start_loc = tokens.peek(-1).loc
    try:
        name = tokens.match(TokenKind.SYMBOL).lexeme
    except:
        token = tokens.peek()
        raise Exception(f"Expected symbol, got {token}")
    ty: Optional[ast.Type] = None
    if tokens.match(TokenKind.COLON):
        try:
            ty = parse_type(tokens)
        except Exception as e:
            raise Exception(f"Expected a type after semicolon at {tokens.peek().loc} {e}")
    if not tokens.match(TokenKind.EQUAL):
        raise Exception(f"Expected = after {"let" if mut else "const"} declaration at {tokens.peek().loc}")
    try:
        value = parse_expr(tokens)
    except Exception as e:
        raise Exception(f"Expected an expression after = at {tokens.peek().loc} {e}")
    end_loc = tokens.peek(-1).loc
    return ast.VarDecl.init(name, mut, ty, value, start_loc, end_loc)

def parse_block(tokens: TokenIter) -> ast.Block:
    start_loc = tokens.peek(-1).loc
    stmts = []
    while not (tokens.match(TokenKind.END) or tokens.eof()):
        stmts.append(parse_stmt(tokens))
    if tokens.eof():
        raise Exception(f"Expected end of block at {tokens.peek().loc}")
    else:
        end_loc = tokens.peek(-1).loc
        return ast.Block(start_loc, end_loc, stmts)

def parse_if(tokens: TokenIter) -> ast.IfStmt:
    start_loc = tokens.peek(-1).loc
    try:
        cond = parse_expr(tokens)
    except Exception as e:
        raise Exception(f"Expected an expression after if at {tokens.peek().loc} {e}")
    try:
        then_block = parse_block(tokens)
    except Exception as e:
        raise Exception(f"Expected a block after if condition at {tokens.peek().loc} {e}")
    else_block = None
    if tokens.match(TokenKind.ELSE):
        try:
            if tokens.match(TokenKind.IF):
                else_block = parse_if(tokens)
            else:
                else_block = parse_block(tokens)
        except Exception as e:
            raise Exception(f"Expected a block or if-else statement after else at {tokens.peek().loc} {e}")
    end_loc = tokens.peek(-1).loc
    return ast.IfStmt(cond, then_block, else_block, start_loc, end_loc)

def parse_generic_list(tokens: TokenIter) -> List[str]:
    generics = []
    if tokens.match(TokenKind.LT):
        while not (tokens.match(TokenKind.GT) or tokens.eof()):
            try:
                generics.append(tokens.match(TokenKind.SYMBOL).lexeme)
            except Exception as e:
                raise Exception(f"Expected a symbol in generics list at {tokens.peek().loc} {e}")
        if tokens.eof():
            raise Exception(f"Expected > at {tokens.peek().loc}")
    return generics

def parse_type_decl(tokens: TokenIter) -> ast.TypeDecl:
    start_loc = tokens.peek(-1).loc
    try:
        name = tokens.match(TokenKind.SYMBOL).lexeme
    except:
        token = tokens.peek()
        raise Exception(f"Expected symbol, got {token}")
    generics = parse_generic_list(tokens)
    if not tokens.match(TokenKind.EQUAL):
        raise Exception(f"Expected = after type declaration at {tokens.peek().loc}")
    try:
        ty = parse_type(tokens)
    except Exception as e:
        raise Exception(f"Expected a type after = at {tokens.peek().loc} {e}")
    end_loc = tokens.peek(-1).loc
    return ast.TypeDecl(name, generics, ty, start_loc, end_loc)

def parse_stmt(tokens: TokenIter) -> ast.Statement:
    # is it prefixed with pub?
    pub = None != tokens.match(TokenKind.PUB)
    if tokens.match(TokenKind.CONST):
        return parse_var_decl(tokens, False)
    elif tokens.match(TokenKind.LET):
        return parse_var_decl(tokens, True)
    elif tokens.match(TokenKind.BEGIN):
        return parse_block(tokens)
    elif tokens.match(TokenKind.IF):
        return parse_if(tokens)
    elif tokens.match(TokenKind.TYPE):
        return parse_type_decl(tokens)


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
