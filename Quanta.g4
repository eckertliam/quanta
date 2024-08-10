grammar Quanta;

// statements
program: expression*;

list:
    '(' expression* ')'
    | '[' expression* ']'
    | '{' expression* '}'
    ;

expression:
    list
    | BOOLEAN
    | SYMBOL
    | FLOAT
    | STRING
    | INT
    | BYTE
    | CHAR
    ;

BOOLEAN: 'true' | 'false';
SYMBOL: [a-zA-Z_][a-zA-Z_0-9-]*[?!]?;
FLOAT: ('+' | '-')? [0-9]+ '.' [0-9]+;
STRING: '"' ~[\r\n"]* '"';
INT: ('+' | '-')? [0-9]+;
BYTE: ('0x' [0-9a-fA-F]+) | ('0b' [01]+) | INT;
CHAR: '\'' ~[\r\n'] '\'';
NEWLINE: ('\r' | '\n')+;

WS: [ \t\r\n]+ -> skip;