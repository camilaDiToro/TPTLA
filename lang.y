%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/expToAst.h"
#include "include/genericToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"
#include "include/shared.h"

// Extern prototypes

extern FILE * out;

%}

%union {
    char string[1024];
    int integer;
    struct ExpNode * expNode;
    struct ExpResultNode * expResultNode;
	struct GenericNode * genericNode;
	struct StringNode * stringNode;
	struct ArrayNode * arrayNode;
	struct IfNode * ifNode;
	struct ReadNode * readNode;
}

%type <expNode> constant factor variable expression
%type <expResultNode> expression_result string_constant
%type <genericNode> program json row_then row_else row_content
%type <stringNode> string string_body row_condition 
%type <arrayNode> array array_body
%type <ifNode> json_if json_if_body
%type <readNode> json_read json_read_body
%type <string> row_var

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


%%

program: json													{ printf("ProgramGenericAction\n"); $$ = ProgramGenericAction($1);}
	; 						

json: string													{ printf("NewNodeGenericAction\n"); $$ = NewNodeGenericAction((void*)$1, STRING_NODE); }
    | array														{ printf("JSON TYPE: Array\n"); 	$$ = NewNodeGenericAction((void*)$1, ARRAY_NODE);}
	| json_if													{ printf("JSON TYPE: If\n"); 		$$ = NewNodeGenericAction((void*)$1, JSON_IF_NODE);}
	| json_read													{ printf("JSON TYPE: Read\n"); 		$$ = NewNodeGenericAction((void*)$1, JSON_READ_NODE);}
	;

json_if: OPEN_CURL json_if_body	CLOSE_CURL						{ printf("JSON IF THEN \n"); 		$$ = $2;}
	|	 OPEN_CURL json_if_body COM row_else CLOSE_CURL			{ printf("JSON IF ELSE \n"); 		$$ = AddElseJsonIfAction($2, $4);}
	;

json_if_body: row_type_if COM row_condition COM row_then        { printf("JSON IF BODY\n"); 		$$ = NewNodeJsonIfAction($3, $5);}
	;

json_read: OPEN_CURL json_read_body CLOSE_CURL					{ printf("JSON TYPE: Read\n");      $$ = $2;}
	;

json_read_body: row_type_read COM row_var COM row_content		{ printf("JSON READ BODY\n"); 		$$ = NewNodeJsonReadAction($3, $5);}
	;

/*************************************************************************************************************
**                                            TIPOS DE FILAS (ROWs)
*************************************************************************************************************/
row_type_if: TAG_TYPE TPOINTS TAG_IF							{ printf("JSON TYPE IF ROW\n"); }
	;

row_type_read: TAG_TYPE TPOINTS TAG_READ						{ printf("JSON TYPE ROW FOR \n"); }
	;

row_condition: TAG_CONDITION TPOINTS string						{ printf("JSON CONDITION ROW\n"); 	$$ = $3;}
	;

row_then: TAG_THEN TPOINTS json									{ printf("JSON THEN ROW\n"); 		$$ = $3;}
	;

row_else: TAG_ELSE TPOINTS json									{ printf("JSON ELSE ROW\n"); 		$$ = $3;}
	;

row_var: TAG_VAR TPOINTS QUOTE CHARS QUOTE						{ printf("JSON VAR ROW \n"); 		strcpy($$,$4);}
	;

row_content: TAG_CONTENT TPOINTS json							{ printf("JSON CONTENT ROW \n"); 	$$ = $3;}
	;

/*************************************************************************************************************
**                                         TIPOS BASICOS - STRING Y ARRAY
**************************************************************************************************************/

array: OPEN_BRA array_body CLOSE_BRA							{ printf("NON EMPTY ARRAY\n"); 		$$ = ArrayAction($2);}
	|  OPEN_BRA CLOSE_BRA										{ printf("EMPTY ARRAY\n"); 			$$ = EmptyArrayAction();}
	;

array_body: json                                          		{ printf("body array\n"); 			$$ = NewNodeArrayAction($1, NULL);}
	| json COM array_body                                       { printf("body array concat\n"); 	$$ = NewNodeArrayAction($1, $3);}
	;

string: QUOTE string_body QUOTE 								{ printf("StringAction\n");			$$ = StringAction($2); }
	| QUOTE QUOTE												{ printf("EmptyStringAction\n"); 	$$ = EmptyStringAction(); }
	;

string_body: string_constant									{ printf("NewNodeStringAction\n"); 	$$ = NewNodeStringAction($1, NULL); }
	| expression_result											{ printf("NewNodeStringAction\n"); 	$$ = NewNodeStringAction($1, NULL); }
	| string_constant string_body 								{ printf("NewNodeStringAction\n"); 	$$ = NewNodeStringAction($1, $2); }
	| expression_result string_body 							{ printf("NewNodeStringAction\n"); 	$$ = NewNodeStringAction($1, $2); }
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