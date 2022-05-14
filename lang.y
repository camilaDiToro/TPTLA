%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/exp-to-ast.h"

// Extern prototypes

extern int yylex();
void yyerror(ExpNode ** program, char *s);

extern FILE * out;

%}

%union {
    char string[1024];
    int integer;
    struct ExpNode * expNode;
    struct ExpNode * list;
}

%type <expNode> constant factor program variable expression

// IDs de los tokens generados desde Flex:
%token ADD
%token SUB
%token MUL
%token DIV
%token <string> VAR

%token EQ
%token NEQ
%token LT
%token LEQ
%token GT
%token GEQ

%token AND
%token OR
%token NOT

%token TAG_TYPE
%token TAG_CONTENT
%token TAG_IF
%token TAG_CONDITION
%token TAG_THEN
%token TAG_ELSE
%token TAG_FOR
%token TAG_VAR
%token TAG_IN
%token TAG_READ
%token TAG_INRANGE

%token QUOTE
%token DOLLAR
%token COM
%token TPOINTS
%token AT_SIGN

%token OPEN_PARENTHESIS
%token CLOSE_PARENTHESIS
%token OPEN_CURL
%token CLOSE_CURL
%token OPEN_BRA
%token CLOSE_BRA

%token <integer> INTEGER
%token CHARS

%token START_MATH
%token END_MATH

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV

%parse-param {ExpNode ** program}

%%
program: QUOTE START_MATH expression END_MATH QUOTE             { $$ = ProgramGrammarAction(program, $3);}

expression: expression ADD expression							{ $$ = AdditionExpressionGrammarAction($1, $3); }
	| factor													{ $$ = FactorExpressionExpAction($1); }
	;

factor: constant												{ $$ = ConstantFactorExpAction($1); }
	| variable													{ $$ = VariableFactorGrammarAction($1); }
	;

variable: VAR													{ $$ = VariableExpAction($1); }
	| VAR OPEN_BRA INTEGER CLOSE_BRA						    { $$ = VariableSubscriptExpAction($1,$3); }
	;

constant: INTEGER												{ $$ = IntegerConstantExpAction($1); }
	;
%%