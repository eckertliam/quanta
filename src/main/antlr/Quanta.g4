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
           | mut_assignment
           | begin_statement
           | return_statement
           | fn_declaration
           | class_declaration
           | impl_declaration
           | trait_declaration
           | type_declaration)
           (NEWLINE | EOF | ';')
           ;

assignment: SYMBOL (':'type )? '=' expr;

mut_assignment: 'mut' assignment;

begin_statement: 'begin' statement* 'end';

return_statement: 'return' expr;

generics: '<' type (',' type)* '>';

fn_parameter: SYMBOL ':' type;

fn_declaration: 'fn' SYMBOL generics? '(' fn_parameter (',' fn_parameter)* ')' '->' type 'begin' statement* 'end';

class_member: SYMBOL ':' type;

class_declaration: 'class' SYMBOL generics? 'begin' class_member* 'end';

impl_declaration: 'impl' type ('for' type)? 'begin' fn_declaration* 'end';

trait_declaration: 'trait' SYMBOL generics? 'begin' (class_member | fn_declaration)* 'end';

type_declaration: 'type' SYMBOL generics? '=' type;

// control structures

if_statement: 'if' expr 'then' statement*
               (('elif' expr 'then' statement*) | ('else' statement*))?
               'end'
               ;

atom_pattern: SYMBOL
              | INT
              | FLOAT
              | BOOLEAN
              | STRING
              | '_'
              ;

match_pattern: atom_pattern
              | atom_pattern ('|' match_pattern)*
              ;

match_case: match_pattern '->' statement*;

match_statement: 'match' expr 'with' match_case+ 'end';

loop_statement: 'loop' statement* 'end';

for_statement: 'for' SYMBOL 'in' expr 'do' statement* 'end';

do_while_statement: 'do' statement* 'while' expr 'end';

while_statement: 'while' expr 'do' statement* 'end';

// expressions
expr: primary_expr (op=('*' | '/' | '%') expr)*
      | primary_expr (op=('+' | '-') expr)*
      | primary_expr (op=('==' | '!=' | '<' | '<=' | '>' | '>=') expr)*
      | primary_expr (op=('and' | 'or') primary_expr)*
      | op=('not' | '-') expr
      | postfix_expr
      | primary_expr
      ;

postfix_expr: field_access
             | module_access
             | fn_call
             | array
             ;

primary_expr: SYMBOL
              | INT
              | FLOAT
              | BOOLEAN
              | STRING
              | '(' expr ')'
              ;

field_access: primary_expr '.' expr;

module_access: primary_expr '::' SYMBOL;

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