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
    | symbol
    | string
    | number
    | char
    ;


symbol:
     IDENTIFIER
     | OPERATOR;

number:
    FLOAT
    | INT
    | BYTE
    ;

string: STRING;

char: CHAR;

OPERATOR: '+' | '-' | '*' | '/' | '%' | '==' | '!=' | '<' | '<=' | '>' | '>=';
IDENTIFIER: ([a-zA-Z_][a-zA-Z_0-9-]*[?!]?);
FLOAT: ('+' | '-')? [0-9]+ '.' [0-9]+;
STRING: '"' ~[\r\n"]* '"';
INT: ('+' | '-')? [0-9]+;
BYTE: ('0x' [0-9a-fA-F]+) | ('0b' [01]+) | INT;
CHAR: '\'' ~[\r\n'] '\'';

WS: [ \t\r\n]+ -> skip;