#define  _GNU_SOURCE
#include "include/astToHTML.h"
#include "include/expToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"
#include "include/shared.h"

#include <stdio.h>
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>


#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE *output;

static void stringToHTML(SymbolTable * table, void * node);
static void arrayToHTML(SymbolTable * table, void * node);
static void genericToHTML(SymbolTable * table, GenericNode * node);
static void jsonIfToHTML(SymbolTable * table, void * node);
static void readToHTML(SymbolTable * table, void * node);

typedef void (*builderFunction)(SymbolTable * table, void * node);

void treeToHTML(GenericNode *program, FILE *file, SymbolTable * table) {
    output = file;
    genericToHTML(table, program);
}

static void stringToHTML(SymbolTable * table, void * node) {
    StringNode* s = (StringNode*)node;
    while(s!=NULL) {
        s->exp->evaluate(table, s->exp);
        P("%s", s->exp->evaluate(table, s->exp));
        s = s->next;
    }    
}

static void arrayToHTML(SymbolTable * table, void * node) {
    ArrayNode* a = (ArrayNode*)node;
    while(a!=NULL){
        genericToHTML(table, a->json);
        a = a->next;
    }    
}

static void jsonIfToHTML(SymbolTable * table, void * node) {
    IfNode* ifn = (IfNode*)node;
    StringNode* cond = ifn->condition;
    char * condValue = cond->exp->evaluate(table, cond->exp);
    if(strcmp(condValue,"1") == 0) {
        genericToHTML(table, ifn->then);
    }
    else {
        genericToHTML(table, ifn->otherwise);
    }
}

static void readToHTML(SymbolTable * table, void * node){
    ReadNode* r = (ReadNode*)node;
    table = newScope(table);

    printf("Please enter a line:\n");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    line[strcspn(line, "\n")] = 0;
    addSymbolToTable(table, newSymbol(r->varName, line,  STRING));
    genericToHTML(table, r->content);
    free(line);
}

static builderFunction buiders[] = {
    /* STRING_NODE */       (builderFunction)stringToHTML,
    /* ARRAY_NODE */        (builderFunction)arrayToHTML,
    /* JSON_IF_NODE */      (builderFunction)jsonIfToHTML,
    /* JSON_FOR_NODE */     (builderFunction)NULL,
    /* JSON_READ_NODE */    (builderFunction)readToHTML
};


static void genericToHTML(SymbolTable * table, GenericNode * node) {
    printf("genericToHTML %d\n", node->nodeType);
    builderFunction builder = buiders[node->nodeType];
    builder(table,node->node);
}