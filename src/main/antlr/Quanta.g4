grammar Quanta;

// statements
program: statement*;

// types

type: SYMBOL
    | fn_type
    | array_type
    | generic_type
    ;

fn_type: '(' type (',' type)* ')' '->' type;

array_type: '[' type ',' expr ']';

generic_type: SYMBOL '<' type (',' type)* '>';

// statements

statement: (expr
           | assignment
           | mut_assignment)
           (NEWLINE | EOF | ';')
           ;

assignment: SYMBOL (':'type )? '=' expr;

mut_assignment: 'mut' assignment;



// control structures

if_statement: 'if' expr 'then' statement*
               (('elif' expr 'then' statement*) | ('else' statement*))?
               'end'
               ;

match_case: (expr | '_') '->' statement*;

match_statement: 'match' expr 'with' match_case+ 'end';

loop_statement: 'loop' statement* 'end';

for_statement: 'for' SYMBOL 'in' expr 'do' statement* 'end';

do_while_statement: 'do' statement* 'while' expr 'end';

while_statement: 'while' expr 'do' statement* 'end';

// expressions
expr
    : primary_expr (op=('*' | '/' | '%') expr)*
    | primary_expr (op=('+' | '-') expr)*
    | primary_expr (op=('==' | '!=' | '<' | '<=' | '>' | '>=') expr)*
    | primary_expr (op=('and' | 'or') primary_expr)*
    | op=('not' | '-') expr
    | primary_expr postfix_op
    ;

primary_expr
    : '(' expr ')'
    | fn_call
    | array
    | SYMBOL
    | INT
    | FLOAT
    | BOOLEAN
    | STRING
    ;

 postfix_op:
    | '.' SYMBOL
    | '::' SYMBOL;

fn_call: SYMBOL '(' expr (',' expr)* ')';

array: '[' expr (',' expr)* ']';

// atoms
BOOLEAN: 'true' | 'false';
SYMBOL: [a-zA-Z_][a-zA-Z_0-9]*;
FLOAT: ('+' | '-')? [0-9]+ '.' [0-9]+;
STRING: '"' ~[\r\n"]* '"';
INT: ('+' | '-')? [0-9]+;
NEWLINE: ('\r' | '\n')+;

WS: [ \t\r\n]+ -> skip;