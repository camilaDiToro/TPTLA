#ifndef EXP_TO_AST_H_
#define EXP_TO_AST_H_

#define TRUE 1
#define FALSE 0

#include "symbolTable.h"

typedef struct ExpNode{
    SymbolType (*getMode)(SymbolTableP symbolTable, struct ExpNode * expNode);
    int   (*evaluateInteger)(SymbolTableP symbolTable, struct ExpNode * expNode);
    char* (*evaluateString)(SymbolTableP symbolTable, struct ExpNode * expNode);
    int ivalue;
    char * cvalue;
    struct ExpNode * left;
    struct ExpNode * right;
}ExpNode;

typedef SymbolType  (*getModeFunction)(SymbolTableP symbolTable, struct ExpNode * expNode);
typedef int         (*evaluateIntegerFunction)(SymbolTableP symbolTable, struct ExpNode * expNode);
typedef char*       (*evaluateStringFunction)(SymbolTableP symbolTable, struct ExpNode * expNode);

typedef struct ExpResultNode{
    char* (*evaluate)(SymbolTableP symbolTable, struct ExpResultNode * expNode);
    char* cvalue;
    struct ExpNode * exp;
}ExpResultNode;

//Expresions
ExpResultNode * ExpressionResultExpAction(ExpNode * exp);
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
ExpResultNode * ProgramGrammarAction(ExpResultNode ** program, ExpResultNode * expNode);

#endif