#ifndef JSON_TO_AST_H_
#define JSON_TO_AST_H_

#include "genericToAst.h"
#include "basicTypeToAst.h"

typedef struct IfNode{
    StringNode * condition;
    GenericNode * then;
    GenericNode * otherwise;
}IfNode;

typedef struct ReadNode{
    char * varName;
    GenericNode * content;
}ReadNode;

IfNode * AddElseJsonIfAction(IfNode * baseNode, GenericNode * otherwise);
IfNode * NewNodeJsonIfAction(StringNode * condition, GenericNode * then);

ReadNode * NewNodeJsonReadAction(char * varName, GenericNode * content);

#endif