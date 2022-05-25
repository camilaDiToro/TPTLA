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

// Wraps the start and the end of an for iterator
typedef struct StartEndWrapperNode{
    ExpResultNode * start; 
    ExpResultNode * end; 
}StartEndWrapperNode;

typedef struct ForInRangeNode{
    GenericNode * content;    
    char * varName;
    StartEndWrapperNode * startEndWrapperNode; 
}ForInRangeNode;



IfNode * AddElseJsonIfAction(IfNode * baseNode, GenericNode * otherwise);
IfNode * NewNodeJsonIfAction(StringNode * condition, GenericNode * then);

ReadNode * NewNodeJsonReadAction(char * varName, GenericNode * content);
ForInRangeNode * NewNodeJsonForInRangeAction(StartEndWrapperNode * startEndWrapperNode, char * varName, GenericNode * content); 
StartEndWrapperNode * NewStartEndWrapperNode(ExpResultNode * startExpNode, ExpResultNode * endExpNode); 

void FreeReadNode(ReadNode* node);
void FreeIfNode(IfNode* node);

#endif