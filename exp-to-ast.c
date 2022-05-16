#include "./include/exp-to-ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ExpNode * newExpNode(getModeFunction gm, evaluateIntegerFunction ei, evaluateStringFunction es, int ivalue, char* cvalue, ExpNode* left, ExpNode * right){
    ExpNode * newNode = malloc(sizeof(ExpNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->getMode = gm;
    newNode->evaluateInteger = ei;
    newNode->evaluateString = es;
    newNode->ivalue = ivalue;
    newNode->cvalue = cvalue;
    newNode->left = left;
    newNode->right = right;
	return newNode;
}

SymbolType integerMode(SymbolTableP symbolTable, struct ExpNode * expNode){
    return INT;
}

SymbolType stringMode(SymbolTableP symbolTable, struct ExpNode * expNode){
    return STRING;
}
SymbolType varMode(SymbolTableP symbolTable, struct ExpNode * expNode){
    SymbolEntryP entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL) // No estaba definida la variable a la hora de consumirla 
        return -1; 

    return entry->type;
}

SymbolType orMode(SymbolTableP symbolTable, struct ExpNode * expNode){
    return expNode->left->getMode(symbolTable, expNode->left) | expNode->right->getMode(symbolTable, expNode->right);
}


//Expresions

int addIntegers(SymbolTableP symbolTable, ExpNode * expNode){
    return expNode->left->evaluateInteger(symbolTable, expNode->left) + expNode->right->evaluateInteger(symbolTable, expNode->right);
}

char * concatStrigns(SymbolTableP symbolTable, ExpNode * expNode){
    char * left =  expNode->left->evaluateString(symbolTable, expNode->left);
    char * right = expNode->right->evaluateString(symbolTable, expNode->right);
    expNode->cvalue = realloc(expNode->cvalue, strlen(left)+strlen(right) + 1);
    expNode->cvalue[0]=0;
    strcat(expNode->cvalue, left);
    strcat(expNode->cvalue, right);
    return expNode->cvalue;
}

ExpNode* AdditionExpressionGrammarAction(ExpNode* exp1, ExpNode* exp2){
    printf("AdditionExpressionGrammarAction \n");
    return newExpNode(&orMode,&addIntegers,&concatStrigns, -1, NULL, exp1, exp2);
}

ExpNode* FactorExpressionExpAction(ExpNode* factor){
    printf("FactorExpressionExpAction \n");
    return factor;
}

// Factors
ExpNode * ConstantFactorExpAction(ExpNode * expNode) {
    printf("ConstantFactorExpAction() \n");
	return expNode;
}

ExpNode* VariableFactorGrammarAction(ExpNode * expNode){
    printf("VariableFactorGrammarAction() \n");
	return expNode;
}

// Variables

char * returnStringVariable(SymbolTableP symbolTable, ExpNode * expNode){
    SymbolEntryP entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL){
        return -1; 
    } // No estaba definida la variable a la hora de consumirla 
    return entry->value;
}

int returnIntegerVariable(SymbolTableP symbolTable, ExpNode * expNode){
    return atoi(returnStringVariable(symbolTable, expNode));
}

ExpNode * VariableSubscriptExpAction(char * varName, int index){
    printf("VariableSubscriptExpAction(%s[%d]) \n", varName, index);
    char * cvalue = malloc(strlen(varName) + 1);
    strcpy(cvalue, varName);
	return newExpNode(&varMode,&returnIntegerVariable,&returnStringVariable, index, cvalue, NULL, NULL);
}

ExpNode * VariableExpAction(char * varName){
    printf("VariableExpAction(%s) \n", varName);
    char * cvalue = malloc(strlen(varName) + 1);
    strcpy(cvalue, varName);
	return newExpNode(&varMode,&returnIntegerVariable,&returnStringVariable, -1, cvalue, NULL, NULL);
}


// Constants
int returnIntegerValue(SymbolTableP symbolTable, ExpNode * expNode){
    return expNode->ivalue;
}

char* returnIntegerAsString(SymbolTableP symbolTable, ExpNode * expNode){
    if(expNode->cvalue == NULL){
        int length = snprintf( NULL, 0, "%d", expNode->ivalue );
        expNode->cvalue = malloc( length + 1 );
        snprintf(expNode->cvalue, length + 1, "%d", expNode->ivalue );
    }
    return expNode->cvalue;
}

ExpNode * IntegerConstantExpAction(const int value) {
    printf("IntegerConstantGrammarAction(%d) \n", value);
    return newExpNode(&integerMode,&returnIntegerValue, &returnIntegerAsString, value, NULL, NULL, NULL );
}


ExpNode * ProgramGrammarAction(ExpNode ** program, ExpNode * expNode) {
    *program = expNode;
    return expNode;
}