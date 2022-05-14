#ifndef EXP_TO_AST_H_
#define EXP_TO_AST_H_

#define TRUE 1
#define FALSE 0

#include "symbolTable.h"

typedef enum{INTEGER_MODE = 0, STRING_MODE = 1} ExpressionMode;

typedef struct ExpNode{
    ExpressionMode mode;
    int   (*evaluateInteger)(SymbolTableP symbolTable, struct ExpNode * expNode);
    char* (*evaluateString)(SymbolTableP symbolTable, struct ExpNode * expNode);
    int ivalue;
    char * cvalue;
    struct ExpNode * left;
    struct ExpNode * right;
}ExpNode;


//Expresions
ExpNode* AdditionExpressionGrammarAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* FactorExpressionExpAction(ExpNode* factor);

//Factor
ExpNode* ConstantFactorExpAction(ExpNode * node);
ExpNode* VariableFactorGrammarAction(ExpNode * node);

//Variable
ExpNode* VariableSubscriptExpAction(char * varName, int index);
ExpNode* VariableExpAction(char * varName);

//Integer
ExpNode* IntegerConstantExpAction(int value);



//Program, this might be en another .c
ExpNode * ProgramGrammarAction(ExpNode ** program, ExpNode * expNode);

#endif