#include "./include/genericToAst.h"


GenericNode * ProgramGenericAction(GenericNode ** program, GenericNode * node) {
    *program = node;
    return node;
}

GenericNode * NewNodeGenericAction(void * node, NodeType type){
    GenericNode * newNode = malloc(sizeof(GenericNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->node = node;
    newNode->nodeType = type;
	return newNode;
}