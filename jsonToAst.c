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