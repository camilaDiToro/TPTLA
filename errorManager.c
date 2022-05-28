#include "include/errorManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


ErrorManager * newErrorManager(){
    return calloc(1,sizeof(ErrorManager));
}

void addUndefindedVariable(ErrorManager* em, char * varName){
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

void showErrors(ErrorManager* em){
    ErrorNode * aux = em->first;
    for(int i=1 ; aux!=NULL ; ++i){
        printf("[Error %i]: %s \n", i, aux->msg);
        aux = aux->next;
    }
}