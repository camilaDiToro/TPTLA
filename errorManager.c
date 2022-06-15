// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/errorManager.h"
#include "include/shared.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction);
static void printError(int errorNumber, ErrorNode* node);
static void printErrorVaraible(int errorNumber, ErrorNode* node);

ErrorManager * newErrorManager(){
    return calloc(1,sizeof(ErrorManager));
}

static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction){
    ErrorNode * newNode = malloc(sizeof(ErrorNode));
    if(newNode == NULL){
        outOfMemory(em);
        return;
    }
    newNode->msg = msg;
    newNode->type = type;
    newNode->next = NULL;
    newNode->printError = errorFunction;

    if(em->first == NULL){
        em->first = newNode;
        em->last = newNode;
    }else{
        em->last->next = newNode;
    }
    em->errorCount += 1;
}

static void printError(int errorNumber, ErrorNode* node){
    static char* messages[] = { "Invalid type inside the array of a for loop. You can only iterate through strings. Ommited iteration.",
                                "Division by 0. The returned result was 0.",
                                "The operation * cannot be applyed over tow strings. The result was \"{undefined}\" due to an invalid type.",
                                "The operations -, /, &&, || and ! are only defined over integers. The result was \"{undefined}\" due to an invalid type."
                              };
    
    fprintf(stderr, "\033[31m[Error %i]: %s \033[0m\n", errorNumber, messages[node->type]);
    return;
}

static void printErrorVaraible(int errorNumber, ErrorNode* node){
    printf("\033[31m[Error %i]: Undefined reference to variable \"%s\". \033[0m\n", errorNumber, node->msg);
    return;
}

void addUndefindedVariable(ErrorManager* em, char * varName){
    state.succeed = FALSE;
    char * nodeMsg = malloc(strlen(varName) + 1);
    if(nodeMsg == NULL){
        outOfMemory(em);
        return;
    }
    strcpy(nodeMsg,varName);
    newErrorNode(em, UNDEFINED_VAR, nodeMsg, printErrorVaraible);
}

void* outOfMemory(ErrorManager* em){
    em->errorCount+= 1;
    state.succeed = FALSE;
    em->outOfMemory = TRUE;
    return NULL;
}

void invalidVariableTypeInForLoop(ErrorManager* em, char * varName){
    state.succeed = FALSE;
    newErrorNode(em, INVALID_TYPE_FOR_LOOP, NULL, printError);
}

void invalidSubOperation(ErrorManager* em){
    state.succeed = FALSE;
    newErrorNode(em, INVALID_TYPE_JUST_INTEGER, NULL, printError);
}

void invalidMulOperation(ErrorManager* em){
    state.succeed = FALSE;
    newErrorNode(em, INVALID_TYPE_TWO_STRINGS, NULL, printError);
}

void divByZero(ErrorManager* em){
    state.succeed = FALSE;
    newErrorNode(em, DIV_BY_ZERO, NULL, printError);
}

void showErrors(ErrorManager* em){
    if(em->errorsShown){
        return;
    }
    em->errorsShown = TRUE;
    if (em->outOfMemory) {
       printf("\033[31m[Deadly error]: out of memory while trying to build de AST \033[0m\n");
    }
    
    ErrorNode * aux = em->first;
    for(int i=1 ; aux!=NULL ; ++i){
        aux->printError(i,aux);
        aux = aux->next;
    }
}

void freeErrorManager(ErrorManager* em){
    ErrorNode * en = em->first;
    while(en!=NULL){
        ErrorNode * aux = en->next;
        if(en->msg!=NULL)
            free(en->msg);
        free(en);
        en = aux;
    }
    free(em);
}