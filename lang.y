%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/expToAst.h"
#include "include/genericToAst.h"
#include "include/stringToAst.h"


// Extern prototypes

extern int yylex();
void yyerror(GenericNode ** program, char *s);

extern FILE * out;

%}

%union {
    char string[1024];
    int integer;
    struct ExpNode * expNode;
    struct ExpResultNode * expResultNode;
	struct GenericNode * genericNode;
	struct StringNode * stringNode;
}

%type <expNode> constant factor variable expression
%type <expResultNode> expression_result string_constant
%type <genericNode> program json
%type <stringNode> string string_body

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
%token <string> CHARS

%token START_MATH
%token END_MATH

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV

%parse-param {GenericNode ** program}

%%

program: json													{ printf("ProgramGenericAction\n"); $$ = ProgramGenericAction(program, $1);}
	; 						

json: string													{ printf("NewNodeGenericAction\n"); $$ = NewNodeGenericAction((void*)$1, STRING_NODE); }
	;

/*************************************************************************************************************
**                                            TIPOS DE FILAS (ROWs)
*************************************************************************************************************/



/*************************************************************************************************************
**                                         TIPOS BASICOS - STRING Y ARRAY
**************************************************************************************************************/

string: QUOTE string_body QUOTE 								{ printf("StringAction\n"); $$ = StringAction($2); }
	| QUOTE QUOTE												{ printf("EmptyStringAction\n"); $$ = EmptyStringAction(); }
	;

string_body: string_constant									{ printf("NewNodeStringAction\n"); $$ = NewNodeStringAction($1, NULL); }
	| expression_result											{ printf("NewNodeStringAction\n"); $$ = NewNodeStringAction($1, NULL); }
	| string_constant string_body 								{ printf("NewNodeStringAction\n"); $$ = NewNodeStringAction($1, $2); }
	| expression_result string_body 							{ printf("NewNodeStringAction\n"); $$ = NewNodeStringAction($1, $2); }
	;

string_constant: CHARS                                          { printf("StringConstantExpAction %s\n", $1); $$ = StringConstantExpAction($1); }
	;

/************************************************************************************************************
**                                                MATH EXPRESSIONS
*************************************************************************************************************/

expression_result:	START_MATH expression END_MATH      		{ $$ = ExpressionResultExpAction($2); }
	;

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