#include "include/astToHTML.h"
#include "include/expToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"

#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE *output;

void yyerror(GenericNode **program, char *s);

static void string_to_html(SymbolTable * table, void * node);
static void array_to_html(SymbolTable * table, void * node);
static void generic_to_html(SymbolTable * table, GenericNode * node);
static void jsonIfToHTML(SymbolTable * table, void * node);

void tree_to_html(GenericNode *program, FILE *file, SymbolTable * table) {

    /*SymbolTable * table = newEmptySymbolTable(); 
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

    SymbolEntry * s6 = newSymbol("s", "1", INT); 
    addSymbolToTable(table, s6);*/

    output = file;
    generic_to_html(table, program);
}

static void generic_to_html(SymbolTable * table, GenericNode * node) {
    if(node->nodeType == STRING_NODE) {
        string_to_html(table, node->node);
    }
    else if(node->nodeType == ARRAY_NODE) {
        array_to_html(table, node->node);
    }else if(node->nodeType == JSON_IF_NODE) {
        jsonIfToHTML(table, node->node);
    }
}

static void string_to_html(SymbolTable * table, void * node) {
    StringNode* s = (StringNode*)node;
    while(s!=NULL) {
        s->exp->evaluate(table, s->exp);
        P("%s", s->exp->evaluate(table, s->exp));
        s = s->next;
    }    
}

static void array_to_html(SymbolTable * table, void * node) {
    ArrayNode* a = (ArrayNode*)node;
    while(a!=NULL){
        generic_to_html(table, a->json);
        a = a->next;
    }    
}

static void jsonIfToHTML(SymbolTable * table, void * node) {
    IfNode* ifn = (IfNode*)node;
    StringNode* cond = ifn->condition;
    char * condValue = cond->exp->evaluate(table, cond->exp);
    if(strcmp(condValue,"1") == 0) {
        generic_to_html(table, ifn->then);
    }
    else {
        generic_to_html(table, ifn->otherwise);
    }
}
