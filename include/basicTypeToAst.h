#ifndef STRING_TO_AST_H_
#define STRING_TO_AST_H_

#include "expToAst.h"
#include "genericToAst.h"

typedef struct StringNode{
    ExpResultNode * exp;
    struct StringNode * next;
}StringNode;

typedef struct ArrayNode{
    struct GenericNode * json;
    struct ArrayNode * next;
}ArrayNode;

StringNode* StringAction(StringNode * stringNode); 
StringNode* EmptyStringAction();
StringNode* NewNodeStringAction(ExpResultNode * expResultNode, StringNode * nextStringNode);

ArrayNode* ArrayAction(ArrayNode * arrayNode); 
ArrayNode * EmptyArrayAction();
ArrayNode * NewNodeArrayAction(GenericNode * genericNode, ArrayNode * nextArrayNode);

void FreeStringNode(StringNode* node);
void FreeArrayNode(ArrayNode* node);

#endif