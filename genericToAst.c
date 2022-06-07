// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/genericToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"
#include "include/commonToAst.h"

typedef void (*FreeNode)(void* node);

GenericNode* ProgramGenericAction(GenericNode* node) {
    state.program = node;
    return node;
}

GenericNode* NewNodeGenericAction(void* node, NodeType type) {
    GenericNode* newNode = malloc(sizeof(GenericNode));
    if (newNode == NULL) {
        return outOfMemory(state.errorManager);
    }
    newNode->node = node;
    newNode->nodeType = type;
    return newNode;
}

static FreeNode freeNode[] = {
        /* STRING_NODE */               (FreeNode)FreeStringNode,
        /* ARRAY_NODE */                (FreeNode)FreeArrayNode,
        /* JSON_IF_NODE */              (FreeNode)FreeIfNode,
        /* JSON_FOR_IN_RANGE_NODE */    (FreeNode)FreeForInRangeNode,
        /* JSON_READ_NODE */            (FreeNode)FreeReadNode,
        /* JSON_FOR_LIST_NODE */        (FreeNode)FreeForListNode,
        /* JSON_COMMON_NODE */          (FreeNode)FreeCommonNode
};

void FreeGenericNode(GenericNode* node) {
    if(node == NULL)
        return;
    FreeNode freer = freeNode[node->nodeType];
    freer(node->node);
    free(node);
}