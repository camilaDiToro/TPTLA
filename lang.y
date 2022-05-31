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


%%

program: json																		{ printf("ProgramGenericAction\n"); $$ = ProgramGenericAction($1);}
	; 						

json: string																		{ printf("NewNodeGenericAction\n"); $$ = NewNodeGenericAction((void*)$1, STRING_NODE); }
    | array																			{ printf("JSON TYPE: Array\n"); 	$$ = NewNodeGenericAction((void*)$1, ARRAY_NODE);}
	| json_if																		{ printf("JSON TYPE: If\n"); 		$$ = NewNodeGenericAction((void*)$1, JSON_IF_NODE);}
	| json_read																		{ printf("JSON TYPE: Read\n"); 		$$ = NewNodeGenericAction((void*)$1, JSON_READ_NODE);}
	| json_for_in_range																{ printf("JSON TYPE: ForInRange\n");$$ = NewNodeGenericAction((void*)$1, JSON_FOR_IN_RANGE_NODE); }
	| json_for_list																	{ printf("JSON TYPE: ForList\n");   $$ = NewNodeGenericAction((void*)$1, JSON_FOR_LIST_NODE);}
	| json_common										        					{ printf("JSON TYPE: Common\n"); 	$$ = NewNodeGenericAction((void*)$1, JSON_COMMON_NODE);}
	;

json_if: OPEN_CURL json_if_body	CLOSE_CURL											{ printf("JSON IF THEN \n"); 		$$ = $2;}
	|	 OPEN_CURL json_if_body COM row_else CLOSE_CURL								{ printf("JSON IF ELSE \n"); 		$$ = AddElseJsonIfAction($2, $4);}
	;

json_if_body: row_type_if COM row_condition COM row_then        					{ printf("JSON IF BODY\n"); 		$$ = NewNodeJsonIfAction($3, $5);}
	;

json_read: OPEN_CURL json_read_body CLOSE_CURL										{ printf("JSON TYPE: Read\n");      $$ = $2;}
	;

json_read_body: row_type_read COM row_var COM row_content							{ printf("JSON READ BODY\n"); 		$$ = NewNodeJsonReadAction($3, $5);}
	;

json_for_in_range: OPEN_CURL json_for_in_range_body CLOSE_CURL        				{ printf("JSON TYPE: ForInRange\n");  $$ = $2;}
	;

json_for_in_range_body: row_type_for COM row_var COM row_inrange COM row_content	{ printf("JSON FOR VAR INRANGE BODY\n"); $$ = NewNodeJsonForInRangeAction($5, $3, $7); }
	|	row_type_for COM row_inrange COM row_content								{ printf("JSON FOR INRANGE BODY\n"); $$ = NewNodeJsonForInRangeAction($3, NULL, $5);}
	;

json_for_list: 	OPEN_CURL row_type_for COM 
				row_var COM row_in COM row_content CLOSE_CURL  						{ printf("JSON FOR LIST\n"); 		$$ = NewNodeJsonForListAction($6, $4, $8);  }
	;

json_common: 	OPEN_CURL row_type json_common_body row_content CLOSE_CURL 			{ printf("JSON TYPE: Generic\n"); 	$$ = NewNodeJsonCommonAction($2,$3,$4);}		
	;

json_common_body: COM																{ printf("JSON ROW EMPTY\n"); 					$$ = NULL;}
	|	COM string TPOINTS string json_common_body									{ printf("JSON ROW ENUMERATION\n"); 				$$ = newAttributeNode($2,$4,$5);}
	; 

/*************************************************************************************************************
**                                            TIPOS DE FILAS (ROWs)
*************************************************************************************************************/
row_type_if: TAG_TYPE TPOINTS TAG_IF							{ printf("JSON TYPE IF ROW\n"); }
	;

row_type_for: TAG_TYPE TPOINTS TAG_FOR							{ printf("JSON TYPE ROW FOR DETECTED\n"); }
	;

row_type_read: TAG_TYPE TPOINTS TAG_READ						{ printf("JSON TYPE ROW FOR \n"); }
	;

row_type: TAG_TYPE TPOINTS string								{ printf("JSON TYPE ROW DETECTED \n"); 	$$ = $3;}				
	;

row_condition: TAG_CONDITION TPOINTS string						{ printf("JSON CONDITION ROW\n"); 		$$ = $3;}
	;

row_then: TAG_THEN TPOINTS json									{ printf("JSON THEN ROW\n"); 			$$ = $3;}
	;

row_else: TAG_ELSE TPOINTS json									{ printf("JSON ELSE ROW\n"); 			$$ = $3;}
	;

row_var: TAG_VAR TPOINTS QUOTE CHARS QUOTE						{ printf("JSON VAR ROW \n"); 			strcpy($$,$4);}
	;

row_content: TAG_CONTENT TPOINTS json							{ printf("JSON CONTENT ROW \n"); 		$$ = $3;}
	;

row_inrange: 	TAG_INRANGE TPOINTS OPEN_BRA QUOTE 
				expression_result QUOTE COM QUOTE 
				expression_result QUOTE CLOSE_BRA 				{ printf("JSON INRANGE ROW DETECTED\n");  $$ = NewStartEndWrapperNode($5, $9);}

row_in: TAG_IN TPOINTS array									{ printf("JSON IN ROW DETECTED\n"); 	$$ = $3; }
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

expression: expression ADD expression							{ $$ = AdditionExpressionExpAction($1, $3); }
	| expression SUB expression									{ $$ = SubtractionExpressionExpAction($1, $3); }
	| expression MUL expression									{ $$ = MultiplicationExpressionExpAction($1, $3); }
	| expression DIV expression									{ $$ = DivisionExpressionExpAction($1, $3); }
	| expression EQ expression									{ $$ = EqualityExpressionExpAction($1, $3); }
	| expression NEQ expression									{ $$ = NequalityExpressionExpAction($1, $3); }
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