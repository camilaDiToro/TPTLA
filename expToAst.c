#include "./include/expToAst.h"
#include "./include/shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void undefinedVariable(SymbolTable* symbolTable, char * varName);


/*************************************************************************************
**                       NODE BUILDING
**************************************************************************************/

ExpNode * newExpNode(getModeFunction gm, evaluateIntegerFunction ei, evaluateStringFunction es, int ivalue, char* cvalue, ExpNode* left, ExpNode* right){
    ExpNode * newNode = malloc(sizeof(ExpNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
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

ExpResultNode * newExpResultNode(evaluateFunction e, char* cvalue, ExpNode* exp){
    ExpResultNode * newNode = calloc(1,sizeof(ExpNode));
    if(newNode == NULL){
        return outOfMemory(state.errorManager);
    }
    newNode->evaluate = e;
    newNode->cvalue = cvalue;
    newNode->exp = exp;
	return newNode;
}

/*************************************************************************************
**                       TYPE DEFINITION
**************************************************************************************/

SymbolType integerMode(SymbolTable* symbolTable, struct ExpNode* expNode){
    return INT;
}

SymbolType stringMode(SymbolTable* symbolTable, struct ExpNode* expNode){
    return STRING;
}
SymbolType varMode(SymbolTable* symbolTable, struct ExpNode* expNode){
    SymbolEntry* entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL) {
        undefinedVariable(symbolTable, expNode->cvalue);
        return varMode(symbolTable,expNode);
    }
    return entry->type;
}

SymbolType orMode(SymbolTable* symbolTable, struct ExpNode * expNode){
    return expNode->left->getMode(symbolTable, expNode->left) | expNode->right->getMode(symbolTable, expNode->right);
}

/*************************************************************************************
**                       EXPRESSION RESULT
**************************************************************************************/

char * evaluate(SymbolTable* symbolTable, struct ExpResultNode * expNode){
    ExpNode * e = expNode->exp;
    if(e->getMode(symbolTable, e) == STRING){
        expNode->cvalue = e->evaluateString(symbolTable, e);
    }else{
        int ans = e->evaluateInteger(symbolTable, e);
        int length = snprintf( NULL, 0, "%d", ans );
        expNode->cvalue = realloc( expNode->cvalue, length + 1 );
        snprintf(expNode->cvalue, length + 1, "%d", ans );
    }
    return expNode->cvalue;
}
    

ExpResultNode * ExpressionResultExpAction(ExpNode * exp){
    return newExpResultNode(&evaluate, NULL, exp);
}


/*************************************************************************************
**                       EXPRESSIONS
**************************************************************************************/

int addIntegers(SymbolTable* symbolTable, ExpNode * expNode){
    return expNode->left->evaluateInteger(symbolTable, expNode->left) + expNode->right->evaluateInteger(symbolTable, expNode->right);
}

char * concatStrigns(SymbolTable* symbolTable, ExpNode * expNode){
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


/*************************************************************************************
**                       FACTORS
**************************************************************************************/

ExpNode* FactorExpressionExpAction(ExpNode* factor){
    printf("FactorExpressionExpAction \n");
    return factor;
}

ExpNode * ConstantFactorExpAction(ExpNode * expNode) {
    printf("ConstantFactorExpAction() \n");
	return expNode;
}

ExpNode* VariableFactorGrammarAction(ExpNode * expNode){
    printf("VariableFactorGrammarAction() \n");
	return expNode;
}

/*************************************************************************************
**                      VARIABLES
**************************************************************************************/

static void undefinedVariable(SymbolTable* symbolTable, char * varName){
    state.succeed = FALSE;
    addUndefindedVariable(state.errorManager, varName);
    addSymbolToTable(symbolTable, newSymbol(varName, "{undefined}", STRING));
}

char * returnStringVariable(SymbolTable* symbolTable, ExpNode * expNode){
    SymbolEntry* entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL){ // The variable was not defined
        undefinedVariable(symbolTable, expNode->cvalue);
        return returnStringVariable(symbolTable,expNode);
    } 
    return entry->value;
}

int returnIntegerVariable(SymbolTable* symbolTable, ExpNode * expNode){
    return atoi(returnStringVariable(symbolTable, expNode));
}

ExpNode * VariableSubscriptExpAction(char * varName, int index){
    printf("VariableSubscriptExpAction(%s[%d]) \n", varName, index);
    char * cvalue = malloc(strlen(varName) + 8);
    sprintf(cvalue, "%s[%d]", varName, index);
    printf("%s",cvalue);
	return newExpNode(&varMode,&returnIntegerVariable,&returnStringVariable, index, cvalue, NULL, NULL);
}

ExpNode * VariableExpAction(char * varName){
    printf("VariableExpAction(%s) \n", varName);
    char * cvalue = malloc(strlen(varName) + 1);
    strcpy(cvalue, varName);
	return newExpNode(&varMode,&returnIntegerVariable,&returnStringVariable, -1, cvalue, NULL, NULL);
}


/*************************************************************************************
**                      CONSTANTS
**************************************************************************************/
int returnIntegerValue(SymbolTable* symbolTable, ExpNode * expNode){
    return expNode->ivalue;
}

char* returnIntegerAsString(SymbolTable* symbolTable, ExpNode * expNode){
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

char * returnString(SymbolTable* symbolTable, struct ExpResultNode * expNode){
    return expNode->cvalue;
}

ExpResultNode * StringConstantExpAction(char * string){
    char * cvalue = malloc(strlen(string) + 1);
    strcpy(cvalue, string);
    return newExpResultNode(&returnString, cvalue, NULL);
}


/*************************************************************************************
**                           Free
**************************************************************************************/

void FreeExpResultNode(ExpResultNode * node){
    if(node == NULL)
        return;
    if(node->cvalue != NULL)
        free(node->cvalue);
    
    FreeExpNode(node->exp);
    free(node);
}

void FreeExpNode(ExpNode * node){
    if(node == NULL)
        return;
    if(node->cvalue != NULL)
        free(node->cvalue);
    FreeExpNode(node->right);
    FreeExpNode(node->left);
    free(node);
}