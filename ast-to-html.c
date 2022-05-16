#include "include/exp-to-ast.h"
#include "include/ast-to-html.h"

#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE *output;

void yyerror(ExpResultNode **program, char *s);

void tree_to_html(ExpResultNode *program, FILE *file) {

    SymbolTable * table = newEmptySymbolTable(); 
    SymbolEntry * s1 = newSymbol("i", "hola", STRING); 
    SymbolEntry * s2 = newSymbol("s", "122", INT); 
    SymbolEntry * s3 = newSymbol("j", "this is a string test", STRING); 
    SymbolEntry * s4 = newSymbol("t", "TEST", STRING); 
    SymbolEntry * s5 = newSymbol("p", "BYE", STRING); 

    addSymbolToTable(table, s1); 
    addSymbolToTable(table, s2); 
    addSymbolToTable(table, s3); 
    addSymbolToTable(table, s4); 
    addSymbolToTable(table, s5); 

    table = newScope(table); 

    output = file;
    P("%s",program->evaluate(table, program));
}
