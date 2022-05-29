#define  _GNU_SOURCE
#include "include/astToHTML.h"
#include "include/expToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"
#include "include/shared.h"
#include "include/itoa.h"
#include "include/errorManager.h"


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

static char * stringNodeToString(SymbolTable * table, StringNode * node) {
    int bufferSize = 10; 
    char * buffer = malloc(bufferSize); 
    int i = 0; 
    StringNode* s = node;
    while(s!=NULL) {
        char * returned = s->exp->evaluate(table, s->exp);
        for (int j = 0; returned[j]; j++) {
            if (i>=bufferSize) {
                bufferSize *= 2; 
                buffer = realloc(buffer, bufferSize); 
            }
            buffer[i++] = returned[j]; 
        }
        s = s->next;
    } 
    buffer[i] = 0; 
    return buffer; 
}

static void stringToHTML(SymbolTable * table, void * node) {
    StringNode* s = (StringNode*)node;
    while(s!=NULL) {
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
    char * condValue = stringNodeToString(table, cond);  
    if(strcmp(condValue,"0") != 0) {
        genericToHTML(table, ifn->then);
    }
    else if (ifn->otherwise != NULL) {
        genericToHTML(table, ifn->otherwise);
    }
    free(condValue);
}

static void readToHTML(SymbolTable * table, void * node){
    ReadNode* r = (ReadNode*)node;
    printf("Please enter a line:\n");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    line[strcspn(line, "\n")] = 0;
    addSymbolToTable(table, newSymbol(r->varName, line,  STRING));
    genericToHTML(table, r->content);
    free(line);
}

static void forInRangeToHTML(SymbolTable * table, void * node) {
    ForInRangeNode* f = (ForInRangeNode*) node; 
    int start = atoi(f->startEndWrapperNode->start->evaluate(table, f->startEndWrapperNode->start));
    int end = atoi(f->startEndWrapperNode->end->evaluate(table, f->startEndWrapperNode->end));
    if (f->varName != NULL) {
        SymbolEntry * entry = newSymbol(f->varName, itoa(start), INT);  // TODO: Modify using TAD, no direct manipulation
        if (f->varName != NULL)
            addSymbolToTable(table, entry);
        for (int i=start; i<end; i++) {
            free(entry->value); 
            entry->value = itoa(i);  
            genericToHTML(table, f->content);
        }
    } else {
        for (int i=start; i<end; i++) {
            genericToHTML(table, f->content);
        }
    }
}

static void forListToHTML(SymbolTable * table, void * node) {
    ForListNode* f = (ForListNode*) node; 
    SymbolEntry * entry = newSymbol(f->varName, NULL, STRING);  // TODO: Modify using TAD, no direct manipulation
    addSymbolToTable(table, entry);
    ArrayNode * current = f->list; 
    while (current != NULL) {
        if (current->json->nodeType == STRING_NODE) {
            StringNode * str = (StringNode *)(current->json->node);   
            entry->value =  stringNodeToString(table, str); 
            genericToHTML(table, f->content);
            current = current->next; 
            free(entry->value); 
        }else{
            invalidVariableTypeInForLoop(state.errorManager, f->varName);
        }
        current = current->next; 
    }

}

static builderFunction buiders[] = {
    /* STRING_NODE */               (builderFunction)stringToHTML,
    /* ARRAY_NODE */                (builderFunction)arrayToHTML,
    /* JSON_IF_NODE */              (builderFunction)jsonIfToHTML,
    /* JSON_FOR_IN_RANGE_NODE */    (builderFunction)forInRangeToHTML,
    /* JSON_READ_NODE */            (builderFunction)readToHTML, 
    /* JSON_FOR_LIST_NODE */        (builderFunction)forListToHTML,
    /* JSON_GENERIC_NODE */         (builderFunction)NULL
};


static void genericToHTML(SymbolTable * table, GenericNode * node) {
    table = newScope(table); 
    printf("genericToHTML %d\n", node->nodeType);
    builderFunction builder = buiders[node->nodeType];
    builder(table,node->node);
    table = deleteScope(table); 
}