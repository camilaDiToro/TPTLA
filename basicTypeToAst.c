// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "./include/basicTypeToAst.h"
#include "./include/genericToAst.h"

StringNode * StringAction(StringNode * stringNode) {
    return stringNode;
}

StringNode * EmptyStringAction() {
    return NewNodeStringAction(StringConstantExpAction(""),NULL);
}

StringNode * NewNodeStringAction(ExpResultNode * expResultNode, StringNode * nextStringNode) {
    StringNode * newNode = malloc(sizeof(StringNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->next = nextStringNode;
    newNode->exp = expResultNode;
	return newNode;
}

ArrayNode * ArrayAction(ArrayNode * arrayNode) {
    return arrayNode;
}
ArrayNode * EmptyArrayAction() {
    return NULL;
}
ArrayNode * NewNodeArrayAction(GenericNode * genericNode, ArrayNode * nextArrayNode) {
    ArrayNode * newNode = malloc(sizeof(ArrayNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->next = nextArrayNode;
    newNode->json = genericNode;
	return newNode;
}

void FreeStringNode(StringNode * node){
    if(node==NULL)
        return;
    FreeExpResultNode(node->exp);
    FreeStringNode(node->next);
    free(node);
}
void FreeArrayNode(ArrayNode * node){
    if(node==NULL)
        return;
    FreeGenericNode(node->json);
    FreeArrayNode(node->next);
    free(node);
}