#ifndef GENERIC_TO_AST_H_
#define GENERIC_TO_AST_H_
#include <stdlib.h>
#include <stdio.h>

#include "shared.h"


typedef enum NodeType {
    STRING_NODE = 0, 
    ARRAY_NODE = 1,
    JSON_IF_NODE = 2, 
    JSON_FOR_IN_RANGE_NODE = 3,
    JSON_READ_NODE = 4,
    JSON_FOR_LIST_NODE = 5,
    JSON_GENERIC_NODE = 6 
} NodeType;

typedef struct GenericNode{
    NodeType nodeType;
    void * node;
}GenericNode;

GenericNode * ProgramGenericAction(GenericNode * node);
GenericNode * NewNodeGenericAction(void * node, NodeType type);
void FreeGenericNode(GenericNode * node);


#endif