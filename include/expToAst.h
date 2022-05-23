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

//String constant
ExpResultNode * StringConstantExpAction(char * string);

#endif