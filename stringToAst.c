#include "./include/stringToAst.h"

StringNode * StringAction(StringNode * stringNode){
    return stringNode;
}

StringNode * EmptyStringAction(){
    return NewNodeStringAction(StringConstantExpAction(""),NULL);
}

StringNode * NewNodeStringAction(ExpResultNode * expResultNode, StringNode * nextStringNode){
    StringNode * newNode = malloc(sizeof(StringNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->next = nextStringNode;
    newNode->exp = expResultNode;
	return newNode;
}