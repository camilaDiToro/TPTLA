#include "include/genericToAst.h"
#include "include/basicTypeToAst.h"
#include "include/jsonToAst.h"

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
        /* JSON_FOR_IN_RANGE_NODE */    (FreeNode)NULL,
        /* JSON_READ_NODE */            (FreeNode)FreeReadNode,
        /* JSON_FOR */                  (FreeNode)NULL,
        /* JSON_GENERIC_NODE */         (FreeNode)NULL
};

void FreeGenericNode(GenericNode* node) {
    FreeNode freer = freeNode[node->nodeType];
    freer(node->node);
    free(node);
}