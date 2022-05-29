#ifndef COMMON_TO_AST_H_
#define COMMON_TO_AST_H_

#include "basicTypeToAst.h"
#include "genericToAst.h"

typedef struct AttributeNode{
    StringNode* attributeName;
    StringNode* content;
    struct AttributeNode* next;
}AttributeNode;

typedef struct CommonNode{
    StringNode* tag;
    AttributeNode* attributeList;
    GenericNode* content;
}CommonNode;

AttributeNode* newAttributeNode(StringNode* attributeName, StringNode* content, AttributeNode* next);
CommonNode* NewNodeJsonCommonAction(StringNode* tag, AttributeNode* attributeList, GenericNode* content);

void FreeCommonNode(CommonNode * node);
void FreeAttributeNode(AttributeNode* node);

#endif