#include "include/jsonToAst.h"


IfNode * AddElseJsonIfAction(IfNode * baseNode, GenericNode * otherwise){
    baseNode->otherwise = otherwise;
    return baseNode;
}

IfNode * NewNodeJsonIfAction(StringNode * condition, GenericNode * then){
    IfNode * newNode = calloc(1, sizeof(IfNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->condition = condition;
    newNode->then = then;
	return newNode;
}

ReadNode * NewNodeJsonReadAction(char * varName, GenericNode * content){
    ReadNode * newNode = calloc(1, sizeof(ReadNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->varName = malloc(strlen(varName) +1);
    strcpy(newNode->varName, varName);
    newNode->content = content;
	return newNode;
}

ForInRangeNode * NewNodeJsonForInRangeAction(StartEndWrapperNode * startEndWrapperNode, char * varName, GenericNode * content) {
   
    ForInRangeNode * newNode = calloc(1, sizeof(ForInRangeNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
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
        return outOfMemory(state.errorManager);
    }
    startEndWrapperNode->start = startExpNode;
    startEndWrapperNode->end = endExpNode; 
    return startEndWrapperNode; 
}

ForListNode * NewNodeJsonForListAction(ArrayNode * arrayNode, char * varName, GenericNode * content) {
    ForListNode * newNode = calloc(1, sizeof(ForListNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }

    newNode->varName = malloc(strlen(varName) +1);
    strcpy(newNode->varName, varName);
    newNode->content = content;
    newNode->list = arrayNode; 
    return newNode; 
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
    if(node->otherwise != NULL){
        FreeGenericNode(node->otherwise);
    }
    free(node);
}

void FreeForListNode(ForListNode * node){
    if(node == NULL)    
        return;
    FreeGenericNode(node->content);
    FreeArrayNode(node->list); 
    if (node->varName != NULL)
        free(node->varName); 
    free(node);
}

void FreeStartEndWrapperNode(StartEndWrapperNode * node){
    if(node == NULL)    
        return;
    FreeExpNode(node->end);
    FreeExpNode(node->start);
    free(node); 
}

void FreeForInRangeNode(ForInRangeNode * node){
    if(node == NULL)    
        return;
    if (node->varName != NULL)
        free(node->varName); 
    FreeStartEndWrapperNode(node->startEndWrapperNode); 
    FreeGenericNode(node->content); 
    free(node);
}

