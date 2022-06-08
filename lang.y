%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/expToAst.h"
#include "include/genericToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"
#include "include/shared.h"
#include "include/commonToAst.h"

// Extern prototypes
extern FILE * out;

%}

%union {
    char basic_string[1024];
    int integer;
    struct ExpNode* expNode;
    struct ExpResultNode* expResultNode;
	struct GenericNode* genericNode;
	struct StringNode* stringNode;
	struct ArrayNode* arrayNode;
	struct IfNode* ifNode;
	struct ReadNode* readNode;
	struct ForInRangeNode* forInRangeNode;
	struct ForListNode* forListNode; 
	struct StartEndWrapperNode* startEndWrapperNode;
	struct AttributeNode* attributeNode;
	struct CommonNode* commonNode;
}

%type <expNode> constant factor variable expression
%type <expResultNode> expression_result string_constant
%type <genericNode> program json row_then row_else row_content
%type <stringNode> string string_body row_condition row_type
%type <arrayNode> array array_body row_in
%type <ifNode> json_if json_if_body
%type <readNode> json_read json_read_body
%type <forInRangeNode> json_for_in_range json_for_in_range_body
%type <forListNode>	json_for_list
%type <startEndWrapperNode> row_inrange
%type <basic_string> row_var 
%type <attributeNode> json_common_body
%type <commonNode> json_common

// IDs de los tokens generados desde Flex:
%token ADD
%token SUB
%token MUL
%token DIV
%token <basic_string> VAR

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
%token <basic_string> CHARS

%token START_MATH
%token END_MATH

// Reglas de asociatividad y precedencia (de menor a mayor):
%left ADD SUB
%left MUL DIV
%left EQ NEQ LT LEQ GT GEQ AND OR NOT


%%

program: json																		{ $$ = ProgramGenericAction($1);}
	; 						

json: string																		{ $$ = NewNodeGenericAction((void*)$1, STRING_NODE); }
    | array																			{ $$ = NewNodeGenericAction((void*)$1, ARRAY_NODE);}
	| json_if																		{ $$ = NewNodeGenericAction((void*)$1, JSON_IF_NODE);}
	| json_read																		{ $$ = NewNodeGenericAction((void*)$1, JSON_READ_NODE);}
	| json_for_in_range																{ $$ = NewNodeGenericAction((void*)$1, JSON_FOR_IN_RANGE_NODE); }
	| json_for_list																	{ $$ = NewNodeGenericAction((void*)$1, JSON_FOR_LIST_NODE);}
	| json_common										        					{ $$ = NewNodeGenericAction((void*)$1, JSON_COMMON_NODE);}
	;

json_if: OPEN_CURL json_if_body	CLOSE_CURL											{ $$ = $2;}
	|	 OPEN_CURL json_if_body COM row_else CLOSE_CURL								{ $$ = AddElseJsonIfAction($2, $4);}
	;

json_if_body: row_type_if COM row_condition COM row_then        					{ $$ = NewNodeJsonIfAction($3, $5);}
	;

json_read: OPEN_CURL json_read_body CLOSE_CURL										{ $$ = $2;}
	;

json_read_body: row_type_read COM row_var COM row_content							{ $$ = NewNodeJsonReadAction($3, $5);}
	;

json_for_in_range: OPEN_CURL json_for_in_range_body CLOSE_CURL        				{ $$ = $2;}
	;

json_for_in_range_body: row_type_for COM row_var COM row_inrange COM row_content	{ $$ = NewNodeJsonForInRangeAction($5, $3, $7); }
	|	row_type_for COM row_inrange COM row_content								{ $$ = NewNodeJsonForInRangeAction($3, NULL, $5);}
	;

json_for_list: 	OPEN_CURL row_type_for COM 
				row_var COM row_in COM row_content CLOSE_CURL  						{ $$ = NewNodeJsonForListAction($6, $4, $8);  }
	;

json_common: 	OPEN_CURL row_type json_common_body row_content CLOSE_CURL 			{ $$ = NewNodeJsonCommonAction($2,$3,$4);}		
	;

json_common_body: COM																{ $$ = NULL;}
	|	COM string TPOINTS string json_common_body									{ $$ = newAttributeNode($2,$4,$5);}
	; 

/*************************************************************************************************************
**                                            TIPOS DE FILAS (ROWs)
*************************************************************************************************************/
row_type_if: TAG_TYPE TPOINTS TAG_IF							{  }
	;

row_type_for: TAG_TYPE TPOINTS TAG_FOR							{  }
	;

row_type_read: TAG_TYPE TPOINTS TAG_READ						{  }
	;

row_type: TAG_TYPE TPOINTS string								{ $$ = $3;}				
	;

row_condition: TAG_CONDITION TPOINTS string						{ $$ = $3;}
	;

row_then: TAG_THEN TPOINTS json									{ $$ = $3;}
	;

row_else: TAG_ELSE TPOINTS json									{ $$ = $3;}
	;

row_var: TAG_VAR TPOINTS QUOTE CHARS QUOTE						{ strcpy($$,$4);}
	;

row_content: TAG_CONTENT TPOINTS json							{ $$ = $3;}
	;

row_inrange: 	TAG_INRANGE TPOINTS OPEN_BRA QUOTE 
				expression_result QUOTE COM QUOTE 
				expression_result QUOTE CLOSE_BRA 				{ $$ = NewStartEndWrapperNode($5, $9);}

row_in: TAG_IN TPOINTS array									{ $$ = $3; }
	;

/*************************************************************************************************************
**                                         TIPOS BASICOS - STRING Y ARRAY
**************************************************************************************************************/

array: OPEN_BRA array_body CLOSE_BRA							{ $$ = ArrayAction($2);}
	|  OPEN_BRA CLOSE_BRA										{ $$ = EmptyArrayAction();}
	;

array_body: json                                          		{ $$ = NewNodeArrayAction($1, NULL);}
	| json COM array_body                                       { $$ = NewNodeArrayAction($1, $3);}
	;

string: QUOTE string_body QUOTE 								{ $$ = StringAction($2); }
	| QUOTE QUOTE												{ $$ = EmptyStringAction(); }
	;

string_body: string_constant									{ $$ = NewNodeStringAction($1, NULL); }
	| expression_result											{ $$ = NewNodeStringAction($1, NULL); }
	| string_constant string_body 								{ $$ = NewNodeStringAction($1, $2); }
	| expression_result string_body 							{ $$ = NewNodeStringAction($1, $2); }
	;

string_constant: CHARS                                          { $$ = StringConstantExpAction($1); }
	;

/************************************************************************************************************
**                                                MATH EXPRESSIONS
*************************************************************************************************************/

expression_result:	START_MATH expression END_MATH      		{ $$ = ExpressionResultExpAction($2); }
	;

expression: expression ADD expression							{ $$ = AdditionExpressionExpAction($1, $3); }
	| expression SUB expression									{ $$ = SubtractionExpressionExpAction($1, $3); }
	| expression MUL expression									{ $$ = MultiplicationExpressionExpAction($1, $3); }
	| expression DIV expression									{ $$ = DivisionExpressionExpAction($1, $3); }
	| expression EQ expression									{ $$ = BooleanExpressionExpAction($1, $3, EQUALITY); }
	| expression NEQ expression									{ $$ = BooleanExpressionExpAction($1, $3, NEQUALITY); }
	| expression LT expression									{ $$ = BooleanExpressionExpAction($1, $3, LESS); }
	| expression LEQ expression									{ $$ = BooleanExpressionExpAction($1, $3, LESS_OR_EQUAL); }
	| expression GT expression									{ $$ = BooleanExpressionExpAction($1, $3, GREATER); }
	| expression GEQ expression									{ $$ = BooleanExpressionExpAction($1, $3, GREATER_OR_EQUAL); }
	| expression AND expression									{ $$ = BooleanExpressionExpAction($1, $3, BOOLEAN_AND); }
	| expression OR expression									{ $$ = BooleanExpressionExpAction($1, $3, BOOLEAN_OR); }
	| NOT expression											{ $$ = NotExpressionExpAction($2); }
	| factor													{ $$ = FactorExpressionExpAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS			{ $$ = $2; }
	| constant													{ $$ = ConstantFactorExpAction($1); }
	| variable													{ $$ = VariableFactorExpAction($1); }
	;

variable: VAR													{ $$ = VariableExpAction($1); }
	| VAR OPEN_BRA INTEGER CLOSE_BRA						    { $$ = VariableSubscriptExpAction($1,$3); }
	;

constant: INTEGER												{ $$ = IntegerConstantExpAction($1); }
	;
%%