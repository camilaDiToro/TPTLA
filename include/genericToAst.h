#ifndef GENERIC_TO_AST_H_
#define GENERIC_TO_AST_H_
#include <stdlib.h>
#include <stdio.h>

typedef enum NodeType {
    STRING_NODE = 0, 
    ARRAY_NODE = 1,
    JSON_IF_NODE = 2, 
    JSON_FOR_NODE = 3,
    JSON_READ_NODE = 4,

    //Must be the last one
    JSON_GENERIC_NODE = 5 
} NodeType;

typedef struct GenericNode{
    NodeType nodeType;
    void * node;
}GenericNode;

GenericNode * ProgramGenericAction(GenericNode ** program, GenericNode * node);
GenericNode * NewNodeGenericAction(void * node, NodeType type);

#endif