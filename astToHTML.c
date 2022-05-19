#include "include/astToHTML.h"
#include "include/expToAst.h"
#include "include/stringToAst.h"

#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE *output;

void yyerror(GenericNode **program, char *s);

static void string_to_html(SymbolTable * table, void * node);

void tree_to_html(GenericNode *program, FILE *file) {

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
    if(program->nodeType == STRING_NODE){
        string_to_html(table, program->node);
    }
}

static void string_to_html(SymbolTable * table, void * node){
    StringNode* s = (StringNode*)node;
    while(s!=NULL){
        s->exp->evaluate(table, s->exp);
        P("%s", s->exp->evaluate(table, s->exp));
        s = s->next;
    }    
}