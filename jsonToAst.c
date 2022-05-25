#include "include/jsonToAst.h"


IfNode * AddElseJsonIfAction(IfNode * baseNode, GenericNode * otherwise){
    baseNode->otherwise = otherwise;
    return baseNode;
}

IfNode * NewNodeJsonIfAction(StringNode * condition, GenericNode * then){
    IfNode * newNode = calloc(1, sizeof(IfNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->condition = condition;
    newNode->then = then;
	return newNode;
}

ReadNode * NewNodeJsonReadAction(char * varName, GenericNode * content){
    ReadNode * newNode = calloc(1, sizeof(ReadNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->varName = malloc(strlen(varName) +1);
    strcpy(newNode->varName, varName);
    newNode->content = content;
	return newNode;
}

ForInRangeNode * NewNodeJsonForInRangeAction(StartEndWrapperNode * startEndWrapperNode, char * varName, GenericNode * content) {
   
    ForInRangeNode * newNode = calloc(1, sizeof(ForInRangeNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }

    if (varName != NULL) {
        newNode->varName = malloc(strlen(varName) +1);
        strcpy(newNode->varName, varName);
    }

    newNode->content = content;
    newNode->startEndWrapperNode = startEndWrapperNode; 
    return newNode;
} 

StartEndWrapperNode * NewStartEndWrapperNode(ExpResultNode * startExpNode, ExpResultNode * endExpNode) {
    StartEndWrapperNode * startEndWrapperNode = calloc(1, sizeof(StartEndWrapperNode)); 
    if(startEndWrapperNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    startEndWrapperNode->start = startExpNode;
    startEndWrapperNode->end = endExpNode; 
    return startEndWrapperNode; 
}


void FreeReadNode(ReadNode* node){
    if(node==NULL)
        return;

    if(node->varName!=NULL)
        free(node->varName);

    FreeGenericNode(node->content);
    free(node);
}

void FreeIfNode(IfNode* node){
    if(node == NULL)    
        return;
    FreeStringNode(node->condition);
    FreeGenericNode(node->then);
    FreeGenericNode(node->otherwise);
    free(node);
}
