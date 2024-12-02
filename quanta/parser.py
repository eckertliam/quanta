from dataclasses import dataclass
from enum import Enum, auto
from typing import Callable, Dict, Optional

from quanta.ast import ArrayType, Binary, BoolLit, Expr, FloatLit, FnType, Ident, IntLit, Statement, BoolType, FloatType, IntType, StrLit, StrType, TupleType, TypeExpr, UdtType, Unary


# helper functions
def is_whitespace(char: str) -> bool:
    return char == ' ' or char == '\t'


def is_identifier_start(char: str) -> bool:
    return char.isalpha() or char == '_'


def is_identifier_char(char: str) -> bool:
    return char.isalnum() or char in '_?!'


class TokenType(Enum):
    LPAREN = auto()
    RPAREN = auto()
    LBRACE = auto()
    RBRACE = auto()
    LBRACKET = auto()
    RBRACKET = auto()
    COMMA = auto()
    DOT = auto()
    SEMICOLON = auto()
    COLON = auto()
    COLON_COLON = auto()
    ARROW = auto()
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    PERCENT = auto()
    AMP = auto()
    AMP_AMP = auto()
    BAR = auto()
    BAR_BAR = auto()
    BANG = auto()
    BANG_EQ = auto()
    EQ = auto()
    EQ_EQ = auto()
    LT = auto()
    LT_EQ = auto()
    GT = auto()
    GT_EQ = auto()
    # Keywords
    IF = auto()
    ELSE = auto()
    MATCH = auto()
    TRUE = auto()
    FALSE = auto()
    FN = auto()
    RETURN = auto()
    STRUCT = auto()
    ENUM = auto()
    TYPE = auto()
    IMPORT = auto()
    FROM = auto()
    AS = auto()
    FOR = auto()
    IN = auto()
    WHILE = auto()
    BREAK = auto()
    CONTINUE = auto()
    CONST = auto()
    VAR = auto()
    # Literals
    IDENTIFIER = auto()
    STRING = auto()
    NUMBER = auto()
    # Special
    NEWLINE = auto()
    INDENT = auto()
    DEDENT = auto()
    EOF = auto()
    ERROR = auto()
    
    
@dataclass
class Token:
    type: TokenType
    lexeme: str
    line: int
    

class Lexer:
    def __init__(self, source: str):
        self.source: str = source
        self.tokens: list[Token] = []
        self.indent_stack: list[int] = [0]
        self.start: int = 0
        self.current: int = 0
        self.line: int = 1
        
    def tokenize(self) -> list[Token]:
        self.next_token()
        while self.current < len(self.source):
            self.next_token()
        return self.tokens
        
    def peek(self) -> Optional[str]:
        """Peek at the next character in the source code without consuming it."""
        if self.current < len(self.source):
            return self.source[self.current]
        return None
    
    def peek_back(self) -> Optional[str]:
        """Peek at the previous character in the source code without consuming it."""
        if self.current > 0:
            return self.source[self.current - 1]
        return None
    
    def advance(self) -> Optional[str]:
        """Advance to the next character in the source code and return it."""
        char = self.peek()
        self.current += 1
        if char == '\n':
            self.line += 1
        return char
    
    def get_lexeme(self) -> str:
        """Get the current lexeme."""
        return self.source[self.start:self.current]
    
    def add_token(self, token_type: TokenType):
        self.tokens.append(Token(token_type, self.get_lexeme(), self.line))
    
    def whitespace(self) -> int:
        char = self.peek()
        # for the purposes of indentation
        depth = 0
        while is_whitespace(char):
            consumed = self.advance()
            if consumed == ' ':
                depth += 1
            else:
                depth += 4
            char = self.peek()
        if char == '\n':
            self.advance()
            self.newline()
        return depth
        
    def newline(self):
        # called when the newline has been advanced over
        self.line += 1
        self.add_token(TokenType.NEWLINE)
        # reset the start of the lexeme
        self.start = self.current
        # track the depth of the newline to determine the indentation level
        depth = self.whitespace()
        if depth > self.indent_stack[-1]:
            self.add_token(TokenType.INDENT)
            self.indent_stack.append(depth)
        elif depth < self.indent_stack[-1]:
            self.add_token(TokenType.DEDENT)
            self.indent_stack.pop()
            
    def identifier(self):
        # consume all identifier characters
        while is_identifier_char(self.peek()):
            self.advance()
        lexeme = self.get_lexeme()
        # check for keywords
        token_type = {
            'if': TokenType.IF,
            'else': TokenType.ELSE,
            'match': TokenType.MATCH,
            'true': TokenType.TRUE,
            'false': TokenType.FALSE,
            'fn': TokenType.FN,
            'return': TokenType.RETURN,
            'struct': TokenType.STRUCT,
            'enum': TokenType.ENUM,
            'type': TokenType.TYPE,
            'import': TokenType.IMPORT,
            'from': TokenType.FROM,
            'as': TokenType.AS,
            'for': TokenType.FOR,
            'in': TokenType.IN,
            'while': TokenType.WHILE,
            'break': TokenType.BREAK,
            'continue': TokenType.CONTINUE,
            'const': TokenType.CONST,
            'var': TokenType.VAR,
        }.get(lexeme, TokenType.IDENTIFIER)
        self.tokens.append(Token(token_type, lexeme, self.line))
        
    def string(self):
        while self.peek() != '"':
            char = self.advance()
            # advance over escaped characters
            if char == '\\':
                self.advance()
        self.add_token(TokenType.STRING)
        
    def number(self):
        while self.peek().isdigit():
            self.advance()
        # handle floating point numbers
        if self.peek() == '.':
            self.advance()
            while self.peek().isdigit():
                self.advance()
        self.add_token(TokenType.NUMBER)

    def next_token(self):
        self.whitespace()
        self.start = self.current
        char = self.advance()
        match char:
            case None:
                self.add_token(TokenType.EOF)
            case '(':
                self.add_token(TokenType.LPAREN)
            case ')':
                self.add_token(TokenType.RPAREN)
            case '{':
                self.add_token(TokenType.LBRACE)
            case '}':
                self.add_token(TokenType.RBRACE)
            case '[':
                self.add_token(TokenType.LBRACKET)
            case ']':
                self.add_token(TokenType.RBRACKET)
            case ',':
                self.add_token(TokenType.COMMA)
            case '.':
                self.add_token(TokenType.DOT)
            case ';':
                self.add_token(TokenType.SEMICOLON)
            case ':':
                if self.peek() == ':':
                    self.advance()
                    self.add_token(TokenType.COLON_COLON)
                else:
                    self.add_token(TokenType.COLON)
            case '+':
                self.add_token(TokenType.PLUS)
            case '-':
                if self.peek() == '>':
                    self.advance()
                    self.add_token(TokenType.ARROW)
                else:
                    self.add_token(TokenType.MINUS)
            case '*':
                self.add_token(TokenType.STAR)
            case '/':
                self.add_token(TokenType.SLASH)
            case '%':
                self.add_token(TokenType.PERCENT)
            case '&':
                if self.peek() == '&':
                    self.advance()
                    self.add_token(TokenType.AMP_AMP)
                else:
                    self.add_token(TokenType.AMP)
            case '|':
                if self.peek() == '|':
                    self.advance()
                    self.add_token(TokenType.BAR_BAR)
                else:
                    self.add_token(TokenType.BAR)
            case '!':
                if self.peek() == '=':
                    self.advance()
                    self.add_token(TokenType.BANG_EQ)
                else:
                    self.add_token(TokenType.BANG)
            case '=':
                if self.peek() == '=':
                    self.advance()
                    self.add_token(TokenType.EQ_EQ)
                else:
                    self.add_token(TokenType.EQ)
            case '<':
                if self.peek() == '=':
                    self.advance()
                    self.add_token(TokenType.LT_EQ)
                else:
                    self.add_token(TokenType.LT)
            case '>':
                if self.peek() == '=':
                    self.advance()
                    self.add_token(TokenType.GT_EQ)
                else:
                    self.add_token(TokenType.GT)
            case '"':
                self.string()
            case _:
                if is_identifier_start(char):
                    self.identifier()
                elif char.isdigit():
                    self.number()
                else:
                    msg = f'Unexpected character: {char}'
                    self.tokens.append(Token(TokenType.ERROR, msg, self.line))
        
    def tokenize(self) -> list[Token]:
        while self.current < len(self.source):
            self.next_token()
        return self.tokens
    

class Precedence(Enum):
    NONE = 0
    OR = 1
    AND = 2
    EQUALITY = 3
    COMPARISON = 4
    TERM = 5
    FACTOR = 6
    UNARY = 7
    CALL = 8
    PRIMARY = 9
    
    def increment(self) -> 'Precedence':
        if self == Precedence.NONE:
            return Precedence.OR
        elif self == Precedence.PRIMARY:
            return self
        return Precedence(self.value + 1)
    
    @staticmethod
    def from_token_type(type: TokenType) -> 'Precedence':
        return {
            TokenType.BAR_BAR: Precedence.OR,
            TokenType.AMP_AMP: Precedence.AND,
            TokenType.EQ_EQ: Precedence.EQUALITY,
            TokenType.BANG_EQ: Precedence.EQUALITY,
            TokenType.LT: Precedence.COMPARISON,
            TokenType.LT_EQ: Precedence.COMPARISON,
            TokenType.GT: Precedence.COMPARISON,
            TokenType.GT_EQ: Precedence.COMPARISON,
            TokenType.PLUS: Precedence.TERM,
            TokenType.MINUS: Precedence.TERM,
            TokenType.STAR: Precedence.FACTOR,
            TokenType.SLASH: Precedence.FACTOR,
            TokenType.PERCENT: Precedence.FACTOR,
        }.get(type, Precedence.NONE)


PrefixFn = Callable[['Parser'], Expr]
InfixFn = Callable[['Parser', Expr], Expr]


@dataclass
class ParseRule:
    prefix: Optional[PrefixFn]
    infix: Optional[InfixFn]
    precedence: Precedence
    
    
def binary(parser: 'Parser', lhs: Expr) -> Expr:
    operator_type = parser.previous.type
    line = parser.previous.line
    rule = PARSE_RULES.get(operator_type, None)
    assert rule is not None, f'No binary infix rule for {operator_type}'
    rhs = parser.parse_precedence(rule.precedence.increment())
    return Binary(op=operator_type, left=lhs, right=rhs, line=line)


def unary(parser: 'Parser') -> Expr:
    operator_type = parser.previous.type
    line = parser.previous.line
    operand = parser.parse_precedence(Precedence.UNARY)
    return Unary(op=operator_type, value=operand, line=line)


def identifier(parser: 'Parser') -> Expr:
    return Ident(name=parser.previous.lexeme, line=parser.previous.line)


def number(parser: 'Parser') -> Expr:
    lexeme = parser.previous.lexeme
    if '.' in lexeme:
        return FloatLit(value=float(lexeme), line=parser.previous.line)
    else:
        return IntLit(value=int(lexeme), line=parser.previous.line)

    
def string(parser: 'Parser') -> Expr:
    return StrLit(value=parser.previous.lexeme, line=parser.previous.line)


def boolean(parser: 'Parser') -> Expr:
    return BoolLit(value=parser.previous.lexeme == 'true', line=parser.previous.line)


PARSE_RULES: Dict[TokenType, ParseRule] = {
    TokenType.IDENTIFIER: ParseRule(prefix=identifier, infix=None, precedence=Precedence.NONE),
    TokenType.NUMBER: ParseRule(prefix=number, infix=None, precedence=Precedence.NONE),
    TokenType.STRING: ParseRule(prefix=string, infix=None, precedence=Precedence.NONE),
    TokenType.TRUE: ParseRule(prefix=boolean, infix=None, precedence=Precedence.NONE),
    TokenType.FALSE: ParseRule(prefix=boolean, infix=None, precedence=Precedence.NONE),
    TokenType.BANG: ParseRule(prefix=unary, infix=None, precedence=Precedence.UNARY),
    TokenType.BAR_BAR: ParseRule(prefix=None, infix=binary, precedence=Precedence.OR),
    TokenType.AMP_AMP: ParseRule(prefix=None, infix=binary, precedence=Precedence.AND),
    TokenType.EQ_EQ: ParseRule(prefix=None, infix=binary, precedence=Precedence.EQUALITY),
    TokenType.BANG_EQ: ParseRule(prefix=None, infix=binary, precedence=Precedence.EQUALITY),
    TokenType.LT: ParseRule(prefix=None, infix=binary, precedence=Precedence.COMPARISON),
    TokenType.LT_EQ: ParseRule(prefix=None, infix=binary, precedence=Precedence.COMPARISON),
    TokenType.GT: ParseRule(prefix=None, infix=binary, precedence=Precedence.COMPARISON),
    TokenType.GT_EQ: ParseRule(prefix=None, infix=binary, precedence=Precedence.COMPARISON),
    TokenType.PLUS: ParseRule(prefix=None, infix=binary, precedence=Precedence.TERM),
    TokenType.MINUS: ParseRule(prefix=unary, infix=binary, precedence=Precedence.TERM),
    TokenType.STAR: ParseRule(prefix=None, infix=binary, precedence=Precedence.FACTOR),
    TokenType.SLASH: ParseRule(prefix=None, infix=binary, precedence=Precedence.FACTOR),
    TokenType.PERCENT: ParseRule(prefix=None, infix=binary, precedence=Precedence.FACTOR),
    # TODO: handle statement expressions such as ifs and matches that resolve to an expression
    # TODO: handle function calls
    # TODO: handle grouping
    # TODO: handle array literals
    # TODO: handle tuple literals
    # TODO: handle struct literals
    # TODO: handle enum literals
}
    

class Parser:
    def __init__(self, src: str):
        self.idx: int = 0
        self.tokens: list[Token] = Lexer(src).tokenize()
        self.previous: Token = None
        self.current: Token = None
        self.had_error: bool = False
        self.panic_mode: bool = False
        # advance to the first token
        self.advance()
    
    def advance(self) -> Token:
        self.previous = self.current
        self.current = self.tokens[self.idx]
        self.idx += 1
        return self.previous
    
    def consume(self, type: TokenType, message: str) -> Token:
        if self.current.type == type:
            return self.advance()
        else:
            self.error(message)
            
    def error(self, message: str):
        if self.panic_mode:
            return
        self.panic_mode = True
        print(f'{message} at line {self.previous.line}')
        self.had_error = True
        
    def expression(self) -> Expr:
        return self.parse_precedence(Precedence.PRIMARY)
    
    def parse_precedence(self, precedence: Precedence) -> Expr:
        # advance over the first token moving it to previous
        self.advance()
        # parse the prefix
        parse_rule = PARSE_RULES.get(self.previous.type, None)
        assert parse_rule is not None, f'No prefix rule for {self.previous.type}'
        prefix_rule = parse_rule.prefix
        if prefix_rule is None:
            self.error(f'Expected expression, got {self.previous.type}')
            return
        left = prefix_rule(self)
        # parse the infix
        while precedence.value <= parse_rule.precedence.value:
            self.advance()
            parse_rule = PARSE_RULES.get(self.previous.type, None)
            assert parse_rule is not None, f'No infix rule for {self.previous.type}'
            infix_rule = parse_rule.infix
            if infix_rule is None:
                self.error(f'Expected a valid infix expression, got {self.previous.type}')
                return
            left = infix_rule(self, left)
        return left
            
            
    def statement(self) -> Statement:
        pass

    def type_expr(self) -> TypeExpr:
        if self.previous.type == TokenType.IDENTIFIER:
            lexeme = self.previous.lexeme
            type_expr = None
            match lexeme:
                case 'i8' | 'i16' | 'i32' | 'i64' | 'i128':
                    type_expr = IntType(signed=True, size=int(lexeme[1:]))
                case 'u8' | 'u16' | 'u32' | 'u64' | 'u128':
                    type_expr = IntType(signed=False, size=int(lexeme[1:]))
                case 'f32' | 'f64':
                    type_expr = FloatType(size=int(lexeme[1:]))
                case 'bool':
                    type_expr = BoolType()
                case 'str':
                    type_expr = StrType()
                case 'fn':
                    self.advance()
                    params = []
                    while self.current.type != TokenType.RPAREN:
                        params.append(self.type_expr())
                        if self.current.type == TokenType.COMMA:
                            self.advance()
                    self.consume(TokenType.RPAREN, f'Expected ) after function type parameters {params}')
                    self.consume(TokenType.ARROW, f'Expected -> after function type parameters {params}')
                    return_type = self.type_expr()
                    return FnType(params=params, return_type=return_type)
                case _:
                    type_expr = UdtType(name=lexeme)
            return type_expr
        elif self.previous.type == TokenType.LBRACKET:
            self.advance()
            element_type = self.type_expr()
            self.consume(TokenType.COLON, f'Expected : after array element type {element_type}')
            self.advance()
            size = self.expression()
            self.consume(TokenType.RBRACKET, f'Expected ] after array type {element_type} {size}')
            return ArrayType(element_type=element_type, size=size)
        elif self.previous.type == TokenType.LPAREN:
            self.advance()
            elements = []
            while self.current.type != TokenType.RPAREN:
                elements.append(self.type_expr())
                if self.current.type == TokenType.COMMA:
                    self.advance()
            self.consume(TokenType.RPAREN, f'Expected ) after tuple type {elements}')
            return TupleType(elements=elements)
        else:
            self.error(f'Expected type expression, got {self.previous.lexeme}')
        