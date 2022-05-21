#ifndef JSON_TO_AST_H_
#define JSON_TO_AST_H_

#include "genericToAst.h"
#include "basicTypeToAst.h"

typedef struct IfNode{
    StringNode * condition;
    GenericNode * then;
    GenericNode * otherwise;
}IfNode;

IfNode * AddElseJsonIfAction(IfNode * baseNode, GenericNode * otherwise);
IfNode * NewNodeJsonIfAction(StringNode * condition, GenericNode * then);

#endif