%{
#include "./include/exp-to-ast.h"
#include "y.tab.h"
#include <stdio.h>

int fileno(FILE * stream);

extern int reduced;

%}

%option yylineno

crlf			\r\n
digit			[0-9]
endline			\n
alpha           [a-zA-Z]
alphanumeric    [a-zA-Z0-9]
character       [^\f\n\r\t\v\"$]|\\\\$|\\\"
whitespace		[ \f\n\r\t\v]

%s STRING
%s MATH

%%

<STRING>"${"                                {BEGIN(MATH); return START_MATH;}
<MATH>"}"                                   {BEGIN(STRING); return END_MATH;}

":"                                         { return TPOINTS; }
"\"@type\""                                 { return TAG_TYPE; }
"\"@content\""                              { return TAG_CONTENT; }
"\"@if\""                                   { return TAG_IF; }
"\"@condition\""                            { return TAG_CONDITION; }
"\"@then\""                                 { return TAG_THEN; }
"\"@else\""                                 { return TAG_ELSE; }
"\"@for\""                                  { return TAG_FOR; }
"\"@var\""                                  { return TAG_VAR; }
"\"@in\""                                   { return TAG_IN; }
"\"@read\""                                 { return TAG_READ; }
"\"@inrange\""                              { return TAG_INRANGE; }

<MATH>"+"									{ return ADD; }
<MATH>"-"									{ return SUB; }
<MATH>"*"									{ return MUL; }
<MATH>"/"									{ return DIV; }
<MATH>{alpha}{alphanumeric}*                { return VAR; }

<MATH>"=="									{ return EQ; }
<MATH>"!="									{ return NEQ; }
<MATH>"<"									{ return LT; }
<MATH>"<="									{ return LEQ; }
<MATH>">"									{ return GT; }
<MATH>">="									{ return GEQ; }

<MATH>"&&"									{ return AND; }
<MATH>"||"									{ return OR; }
<MATH>"!"									{ return NOT; }

<STRING>","                                 { return CHARS; }
","                                         { return COM; }

"("											{ return OPEN_PARENTHESIS; }
")"											{ return CLOSE_PARENTHESIS; }
"{"											{ return OPEN_CURL; }
"}"											{ return CLOSE_CURL; }
"["                                         { return OPEN_BRA; }
"]"                                         { return CLOSE_BRA; }

<INITIAL>"\""                               { BEGIN(STRING); return QUOTE;  }
<STRING>"\""                                { BEGIN(INITIAL); return QUOTE;  }


<MATH>{digit}+								{ yylval.integer = atoi(yytext); return INTEGER; }

<STRING>({character}|{whitespace})+         { return CHARS; }

<MATH,INITIAL>{whitespace}					{ }

.											{ return yytext[0]; }

%%