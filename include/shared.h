#ifndef SHARED_HEADER
#define SHARED_HEADER

#include <stdio.h>
#include "symbolTable.h"
#include "errorManager.h"

#define FALSE 0
#define TRUE !FALSE


extern FILE * yyin;
extern FILE * yyout;
extern int yylineno;
extern char * yytext;
extern void yyerror(char* err_msg);
extern int yylex(void);
extern char *optarg;
extern int optind, opterr, optopt;

// Application global state
typedef struct {
	int succeed;
	ErrorManager* errorManager;
	void* program;
	SymbolTable * table;
} CompilerState;

// The state is defined in "main.c":
extern CompilerState state;

#endif
