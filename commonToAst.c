#include "include/commonToAst.h"

AttributeNode* newAttributeNode(StringNode* attributeName, StringNode* content, AttributeNode* next){
    AttributeNode * newNode = malloc(sizeof(AttributeNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->attributeName = attributeName;
    newNode->content = content;
    newNode->next = next;
	return newNode;
}
CommonNode* NewNodeJsonCommonAction(StringNode* tag, AttributeNode* attributeList, GenericNode* content){
    CommonNode * newNode = malloc(sizeof(CommonNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->tag = tag;
    newNode->attributeList = attributeList;
    newNode->content = content;
	return newNode;
}

void FreeCommonNode(CommonNode * node){
    if(node==NULL)
        return;
    FreeStringNode(node->tag);
    FreeAttributeNode(node->attributeList);
    FreeGenericNode(node->content);
    free(node);
}
void FreeAttributeNode(AttributeNode* node){
    if(node==NULL)
        return;
    FreeStringNode(node->attributeName);
    FreeStringNode(node->content);
    FreeAttributeNode(node->next);
    free(node);
}