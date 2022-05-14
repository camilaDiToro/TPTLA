#include "./include/exp-to-ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    ExpNode * newNode = calloc(1,sizeof(ExpNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->mode = exp1->mode | exp2->mode;
    newNode->evaluateInteger = &addIntegers;
    newNode->evaluateString = &concatStrigns;
    newNode->left = exp1;
    newNode->right = exp2;
	return newNode;
}

ExpNode* FactorExpressionExpAction(ExpNode* factor){
    printf("FactorExpressionExpAction \n");
    return factor;
}

// Factors
ExpNode * ConstantFactorExpAction(ExpNode * expNode) {
    printf("ConstantFactorExpAction(%d) \n", expNode->evaluateInteger(NULL, expNode));
	return expNode;
}

ExpNode* VariableFactorGrammarAction(ExpNode * expNode){
    printf("VariableFactorGrammarAction(%s) \n", expNode->evaluateString(NULL, expNode));
	return expNode;
}

// Variables

char * returnVariable(SymbolTableP symbolTable, ExpNode * expNode){
    // Look up in the table the name of the variable and return its value
     char * mockReturn = malloc(strlen("varsimple")+1);
     strcpy(mockReturn, "varsimple");
     return mockReturn;
}

ExpNode * VariableSubscriptExpAction(char * varName, int index){
    printf("VariableSubscriptExpAction(%s) \n", varName);
    ExpNode * newNode = calloc(1,sizeof(ExpNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    //Variables are now trated as strings, check how to manage the integer case
    newNode->mode = STRING_MODE;
    newNode->evaluateString = &returnVariable;
    newNode->ivalue = -1;
    newNode->cvalue = malloc(strlen(varName) + 1);
    strcpy(newNode->cvalue, varName);
	return newNode;
}

ExpNode * VariableExpAction(char * varName){
    printf("VariableExpAction(%s) \n", varName);
    ExpNode * newNode = calloc(1,sizeof(ExpNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    //Variables are now trated as strings, check how to manage the integer case
    newNode->mode = STRING_MODE;
    newNode->evaluateString = &returnVariable;
    newNode->ivalue = -1;
    newNode->cvalue = malloc(strlen(varName) + 1);
    strcpy(newNode->cvalue, varName);
	return newNode;
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
    ExpNode * newNode = calloc(1,sizeof(ExpNode));
    if(newNode == NULL){
        printf("Error while trying to allocate memory \n");
        return NULL;
    }
    newNode->mode = INTEGER_MODE;
    newNode->evaluateInteger = &returnIntegerValue;
    newNode->evaluateString = &returnIntegerAsString;
    newNode->ivalue = value;
	return newNode;
}

ExpNode * ProgramGrammarAction(ExpNode ** program, ExpNode * expNode) {
    *program = expNode;
    return expNode;
}