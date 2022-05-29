#ifndef EXP_TO_AST_H_
#define EXP_TO_AST_H_

#include "symbolTable.h"
#include "shared.h"

typedef struct ExpNode{
    SymbolType (*getMode)(SymbolTable* symbolTable, struct ExpNode * expNode);
    int   (*evaluateInteger)(SymbolTable* symbolTable, struct ExpNode * expNode);
    char* (*evaluateString)(SymbolTable* symbolTable, struct ExpNode * expNode);
    int ivalue;
    char * cvalue;
    struct ExpNode * left;
    struct ExpNode * right;
}ExpNode;

typedef SymbolType  (*getModeFunction)(SymbolTable* symbolTable, struct ExpNode * expNode);
typedef int         (*evaluateIntegerFunction)(SymbolTable* symbolTable, struct ExpNode * expNode);
typedef char*       (*evaluateStringFunction)(SymbolTable* symbolTable, struct ExpNode * expNode);

typedef struct ExpResultNode{
    char* (*evaluate)(SymbolTable* symbolTable, struct ExpResultNode * expNode);
    char* cvalue;
    struct ExpNode * exp;
}ExpResultNode;

typedef char*       (*evaluateFunction)(SymbolTable* symbolTable, struct ExpResultNode * expNode);


//Frees
void FreeExpResultNode(ExpResultNode * node);
void FreeExpNode(ExpNode * node);

//Expresions
ExpResultNode * ExpressionResultExpAction(ExpNode * exp);
ExpNode* AdditionExpressionExpAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* SubtractionExpressionExpAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* MultiplicationExpressionExpAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* DivisionExpressionExpAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* EqualityExpressionExpAction(ExpNode* exp1, ExpNode* exp2);
ExpNode* FactorExpressionExpAction(ExpNode* factor);

//Factor
ExpNode* ConstantFactorExpAction(ExpNode * node);
ExpNode* VariableFactorExpAction(ExpNode * node);

//Variable
ExpNode* VariableSubscriptExpAction(char * varName, int index);
ExpNode* VariableExpAction(char * varName);

//Integer
ExpNode* IntegerConstantExpAction(int value);

//String constant
ExpResultNode * StringConstantExpAction(char * string);

#endif