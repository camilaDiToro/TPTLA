#include "include/errorManager.h"
#include "include/shared.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


ErrorManager * newErrorManager(){
    return calloc(1,sizeof(ErrorManager));
}

void addUndefindedVariable(ErrorManager* em, char * varName){
    state.succeed = FALSE;
    static char * msg = "Undefined reference to variable \"%s\"";
    ErrorNode * newNode = malloc(sizeof(ErrorNode));
    newNode->msg = malloc(strlen(msg) + strlen(varName) + 1);
    sprintf(newNode->msg, msg, varName);
    newNode->type = UNDEFINED_VAR;
    newNode->next = NULL;

    if(em->first == NULL){
        em->first = newNode;
        em->last = newNode;
    }else{
        em->last->next = newNode;
    }
    em->errorCount += 1;
}

void* outOfMemory(ErrorManager* em){
    em->errorCount+= 1;
    state.succeed = FALSE;
    em->outOfMemory = TRUE;
    return NULL;
}

void invalidVariableTypeInForLoop(ErrorManager* em, char * varName){
    state.succeed = FALSE;
    static char * msg = "Invalid type inside the array of a for loop. You can only iterate through strings. Ommited iteration\n";
    ErrorNode * newNode = malloc(sizeof(ErrorNode));
    newNode->msg = malloc(strlen(msg) + 1);
    strcpy(newNode->msg, msg);

    newNode->type = INVALID_TYPE;
    newNode->next = NULL;

    if(em->first == NULL){
        em->first = newNode;
        em->last = newNode;
    }else{
        em->last->next = newNode;
    }
    em->errorCount += 1;
}

void showErrors(ErrorManager* em){
    if(em->errorsShown){
        return;
    }
    em->errorsShown = TRUE;
    if (em->outOfMemory) {
       printf("[Deadly error]: out of memory while trying to build de AST \n");
    }
    
    ErrorNode * aux = em->first;
    for(int i=1 ; aux!=NULL ; ++i){
        printf("[Error %i]: %s \n", i, aux->msg);
        aux = aux->next;
    }
}

void freeErrorManager(ErrorManager* em){
    ErrorNode * en = em->first;
    while(en!=NULL){
        ErrorNode * aux = en->next;
        free(en->msg);
        free(en);
        en = aux;
    }
    free(em);
}