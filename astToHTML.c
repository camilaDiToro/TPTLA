// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define _GNU_SOURCE
#include "include/astToHTML.h"
#include "include/basicTypeToAst.h"
#include "include/errorManager.h"
#include "include/expToAst.h"
#include "include/itoa.h"
#include "include/jsonToAst.h"
#include "include/commonToAst.h"
#include "include/shared.h"

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE* output;

static void stringToHTML(SymbolTable* table, StringNode* node, int align);
static void arrayToHTML(SymbolTable* table, ArrayNode* node, int align);
static void genericToHTML(SymbolTable* table, GenericNode* node, int align);
static void jsonIfToHTML(SymbolTable* table, IfNode* node, int align);
static void readToHTML(SymbolTable* table, ReadNode* node, int align);
static void commonToHTML(SymbolTable* table, CommonNode* node, int align);
static void forListToHTML(SymbolTable* table, ForListNode* node, int align);
static void forInRangeToHTML(SymbolTable* table, ForInRangeNode* node, int align);

static char* stringNodeToString(SymbolTable* table, StringNode* node);
static void attributesToHTML(SymbolTable* table, AttributeNode* node);

typedef void (*builderFunction)(SymbolTable* table, void* node, int align);

void treeToHTML(GenericNode* program, FILE* file, SymbolTable* table) {
    output = file;
    genericToHTML(table, program, -1);
}

static char* stringNodeToString(SymbolTable* table, StringNode* node) {
    int bufferSize = 10;
    char* buffer = malloc(bufferSize);
    if(buffer == NULL){
        outOfMemory(state.errorManager);
        return buffer;
    }
    int i = 0;
    StringNode* s = node;
    while (s != NULL) {
        char* returned = s->exp->evaluate(table, s->exp);
        for (int j = 0; returned[j]; j++) {
            if (i >= bufferSize) {
                bufferSize *= 2;
                char* aux = realloc(buffer, bufferSize);
                if(aux == NULL){
                    outOfMemory(state.errorManager);
                    return buffer;
                }else{
                    buffer = aux;
                }
            }
            buffer[i++] = returned[j];
        }
        s = s->next;
    }
    buffer[i] = 0;
    return buffer;
}

static void printAlignment(int align){
    for(int i=0 ; i<align ; ++i){
        P("  ");
    }
}

static void stringToHTML(SymbolTable* table, StringNode* node, int align) {
    printAlignment(align);
    while (node != NULL) {
        P("%s", node->exp->evaluate(table, node->exp));
        node = node->next;
    }
    P("\n");
}

static void arrayToHTML(SymbolTable* table, ArrayNode* node, int align) {
    while (node != NULL) {
        genericToHTML(table, node->json, align);
        node = node->next;
    }
}

static void jsonIfToHTML(SymbolTable* table, IfNode* node, int align) {
    StringNode* cond = node->condition;
    char* condValue = stringNodeToString(table, cond);
    if(condValue == NULL){
        return;
    }
    if (strcmp(condValue, "0") != 0) {
        genericToHTML(table, node->then, align);
    } else if (node->otherwise != NULL) {
        genericToHTML(table, node->otherwise, align);
    }
    free(condValue);
}

static void readToHTML(SymbolTable* table, ReadNode* node, int align) {
    printf("Please enter a line:\n");
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    line[strcspn(line, "\n")] = 0;
    addSymbolToTable(table, newSymbol(node->varName, line, STRING));
    genericToHTML(table, node->content, align);
    free(line);
}

static void forInRangeToHTML(SymbolTable* table, ForInRangeNode* node, int align) {
    int start = atoi(node->startEndWrapperNode->start->evaluate(table, node->startEndWrapperNode->start));
    int end = atoi(node->startEndWrapperNode->end->evaluate(table, node->startEndWrapperNode->end));
    if (node->varName != NULL) {
        SymbolEntry* entry = newSymbol(node->varName, itoa(start), INT); // TODO: Modify using TAD, no direct manipulation
        if (node->varName != NULL)
            addSymbolToTable(table, entry);
        for (int i = start; i < end; i++) {
            free(entry->value);
            entry->value = itoa(i);
            genericToHTML(table, node->content, align);
        }
        free(entry->value);
    } else {
        for (int i = start; i < end; i++) {
            genericToHTML(table, node->content, align);
        }
    }
}

static void forListToHTML(SymbolTable* table, ForListNode* node, int align) {
    SymbolEntry* entry = newSymbol(node->varName, NULL, STRING); // TODO: Modify using TAD, no direct manipulation
    addSymbolToTable(table, entry);
    ArrayNode* current = node->list;
    while (current != NULL) {
        if (current->json->nodeType == STRING_NODE) {
            StringNode* str = (StringNode*)(current->json->node);
            entry->value = stringNodeToString(table, str);
            genericToHTML(table, node->content, align);
            free(entry->value);
        } else {
            invalidVariableTypeInForLoop(state.errorManager, node->varName);
        }
        current = current->next;
    }
}

static void attributesToHTML(SymbolTable* table, AttributeNode* node){
    AttributeNode* aux = node;
    while(aux!=NULL){
        P(" %s=\"%s\" ", aux->attributeName->exp->evaluate(table, aux->attributeName->exp), aux->content->exp->evaluate(table, aux->content->exp));
        aux = aux->next;
    }
}

static void commonToHTML(SymbolTable* table, CommonNode* node, int align) {
    char * tag = stringNodeToString(table, node->tag);
    printAlignment(align);
    P("<%s", tag);
    attributesToHTML(table, node->attributeList);
    P(">\n");
    genericToHTML(table, node->content, align+1);
    printAlignment(align);
    P("</%s>\n", tag)
    free(tag);
}

static builderFunction buiders[] = {
    /* STRING_NODE */               (builderFunction)stringToHTML,
    /* ARRAY_NODE */                (builderFunction)arrayToHTML,
    /* JSON_IF_NODE */              (builderFunction)jsonIfToHTML,
    /* JSON_FOR_IN_RANGE_NODE */    (builderFunction)forInRangeToHTML,
    /* JSON_READ_NODE */            (builderFunction)readToHTML,
    /* JSON_FOR_LIST_NODE */        (builderFunction)forListToHTML,
    /* JSON_COMMON_NODE */          (builderFunction)commonToHTML
};

static void genericToHTML(SymbolTable* table, GenericNode* node, int align) {
    table = newScope(table);
    builderFunction builder = buiders[node->nodeType];
    builder(table, node->node, align+1);
    deleteScope(table);
}