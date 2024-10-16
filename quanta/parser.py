from typing import Optional, Callable, List
import quanta.ast as ast


class Chars:
    """
    A Char iterator that can be used to parse characters from a string.
    """

    def __init__(self, src: str):
        self.src = src
        self.pos = 0
        self.line = 1
        self.col = 1

    def loc(self) -> ast.Loc:
        return ast.Loc(self.line, self.col)

    def is_done(self) -> bool:
        return self.pos >= len(self.src)

    def peek(self) -> Optional[str]:
        """
        Peek the next character without consuming it.
        """
        if self.is_done():
            return None
        return self.src[self.pos]

    def consume(self) -> Optional[str]:
        """
        Consume the next character and return it.
        """
        if self.is_done():
            return None
        self.pos += 1
        self.col += 1
        if self.src[self.pos - 1] == "\n":
            self.line += 1
            self.col = 1
        return self.src[self.pos - 1]
    
    def consume_whitespace(self) -> None:
        """
        Consume whitespace characters from the current position.
        """
        while not self.is_done() and self.peek().isspace():
            self.consume()

def parse_sexpr(chars: Chars, close_char: str, start_loc: ast.Loc, delim_kind: ast.DelimKind) -> ast.SExpr:
    chars.consume()
    chars.consume_whitespace()
    elements: List[ast.Expr] = []
    while True:
        chars.consume_whitespace()
        if chars.peek() == close_char:
            chars.consume()
            break
        elif chars.peek() == None:
            raise Exception(f"Unexpected EOF while parsing {delim_kind} delimited expression at {start_loc}")
        else:
            elements.append(parse_expr(chars))
    return ast.SExpr(ast.Span(start_loc, chars.loc()), elements, delim_kind)
    

def parse_string(chars: Chars, start_loc: ast.Loc) -> ast.String:
    # skip the opening quote
    chars.consume()
    value = ""
    while not chars.is_done() and chars.peek() != "\"":
        if chars.peek() == "\\":
            # skip the escape character
            chars.consume()
            # catch the unlikely case where the file ends with an escape character in the middle of a string :/
            if chars.is_done():
                raise Exception(f"Unexpected EOF while parsing STRING at {start_loc}")
        # consume the character
        value += chars.consume()
    # confirm there is a closing quote
    if chars.peek() != "\"":
        raise Exception(f"Unterminated string at {start_loc}")
    else:
        # consume the closing quote
        chars.consume()
        return ast.String(ast.Span(start_loc, chars.loc()), value)


def parse_expr(chars: Chars) -> ast.Expr:
    chars.consume_whitespace()
    char: str = chars.peek()
    start_loc: ast.Loc = chars.loc()
    match char:
        case "(":
            try:
                return parse_sexpr(chars, ")", start_loc, ast.DelimKind.PAREN)
            except Exception as e:
                raise Exception(f"Unexpected error while parsing a list at {start_loc}: {e}")
        case "[":
            try:
                return parse_sexpr(chars, "]", start_loc, ast.DelimKind.BRACKET)
            except Exception as e:
                raise Exception(f"Unexpected error while parsing a list at {start_loc}: {e}")
        case "{":
            try:
                return parse_sexpr(chars, "}", start_loc, ast.DelimKind.BRACE)
            except Exception as e:
                raise Exception(f"Unexpected error while parsing a list at {start_loc}: {e}")
        case "\"":
            try:
                return parse_string(chars, start_loc)
            except Exception as e:
                raise Exception(f"Unexpected error while parsing a string at {start_loc}: {e}")