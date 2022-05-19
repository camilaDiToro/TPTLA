#ifndef STRING_TO_AST_H_
#define STRING_TO_AST_H_

#include "expToAst.h"

typedef struct StringNode{
    ExpResultNode * exp;
    struct StringNode * next;
}StringNode;

StringNode * StringAction(StringNode * stringNode); 
StringNode * EmptyStringAction();
StringNode * NewNodeStringAction(ExpResultNode * expResultNode, StringNode * nextStringNode);

#endif