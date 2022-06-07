// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "./include/expToAst.h"
#include "./include/shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void undefinedVariable(SymbolTable* symbolTable, char* varName);

/*************************************************************************************
**                       NODE BUILDING
**************************************************************************************/

ExpNode* newExpNode(getModeFunction gm, evaluateIntegerFunction ei, evaluateStringFunction es, int ivalue, char* cvalue, ExpNode* left, ExpNode* right) {
    ExpNode* newNode = malloc(sizeof(ExpNode));
    if (newNode == NULL) {
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

ExpResultNode* newExpResultNode(evaluateFunction e, char* cvalue, ExpNode* exp) {
    ExpResultNode* newNode = calloc(1, sizeof(ExpNode));
    if (newNode == NULL) {
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

SymbolType integerMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    return INT;
}

SymbolType stringMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    return STRING;
}
SymbolType varMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    SymbolEntry* entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL) {
        undefinedVariable(symbolTable, expNode->cvalue);
        return varMode(symbolTable, expNode);
    }
    return entry->type;
}

SymbolType orMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    return expNode->left->getMode(symbolTable, expNode->left) | expNode->right->getMode(symbolTable, expNode->right);
}

SymbolType justIntegerMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    if (expNode->left->getMode(symbolTable, expNode->left) + expNode->right->getMode(symbolTable, expNode->right)) {
        invalidSubOperation(state.errorManager);
        return INVALID;
    }
    return INT;
}

SymbolType notTwoStringsMode(SymbolTable* symbolTable, struct ExpNode* expNode) {
    int ans = expNode->left->getMode(symbolTable, expNode->left) + expNode->right->getMode(symbolTable, expNode->right);
    if (ans == INVALID) {
        invalidMulOperation(state.errorManager);
    }
    return ans;
}

/*************************************************************************************
**                       EXPRESSION RESULT
**************************************************************************************/

char* evaluate(SymbolTable* symbolTable, struct ExpResultNode* expNode) {
    ExpNode* e = expNode->exp;
    SymbolType s = (e->getMode(symbolTable, e));
    if (s == STRING) {
        char * ans = e->evaluateString(symbolTable, e);
        char* aux = realloc(expNode->cvalue, strlen(ans) + 1);
        if(aux == NULL){
            outOfMemory(state.errorManager);
            return expNode->cvalue;
        }else{
            expNode->cvalue = aux;
        }
        strcpy(expNode->cvalue, ans);
    } else if (s == INT) {
        int ans = e->evaluateInteger(symbolTable, e);
        int length = snprintf(NULL, 0, "%d", ans);
        char* aux = realloc(expNode->cvalue, length + 1);
        if(aux == NULL){
            outOfMemory(state.errorManager);
            return expNode->cvalue;
        }else{
            expNode->cvalue = aux;
        }
        snprintf(expNode->cvalue, length + 1, "%d", ans);
    } else {
        char* msg = "{undefined}";
        char* aux = realloc(expNode->cvalue, strlen(msg) + 1);
        if(aux == NULL){
            outOfMemory(state.errorManager);
            return expNode->cvalue;
        }else{
            expNode->cvalue = aux;
        }
        strcpy(expNode->cvalue, msg);
    }
    return expNode->cvalue;
}

ExpResultNode* ExpressionResultExpAction(ExpNode* exp) {
    return newExpResultNode(&evaluate, NULL, exp);
}

/*************************************************************************************
**                       EXPRESSIONS
**************************************************************************************/

int addIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    return expNode->left->evaluateInteger(symbolTable, expNode->left) + expNode->right->evaluateInteger(symbolTable, expNode->right);
}

char* concatStrigns(SymbolTable* symbolTable, ExpNode* expNode) {
    char* left = expNode->left->evaluateString(symbolTable, expNode->left);
    char* right = expNode->right->evaluateString(symbolTable, expNode->right);
    char* aux = realloc(expNode->cvalue, strlen(left) + strlen(right) + 1);
    if(aux == NULL){
        outOfMemory(state.errorManager);
        return expNode->cvalue;
    }else{
        expNode->cvalue = aux;
    }
    expNode->cvalue[0] = 0;
    strcat(expNode->cvalue, left);
    strcat(expNode->cvalue, right);
    return expNode->cvalue;
}

ExpNode* AdditionExpressionExpAction(ExpNode* exp1, ExpNode* exp2) {
    printf("AdditionExpressionExpAction \n");
    return newExpNode(&orMode, &addIntegers, &concatStrigns, -1, NULL, exp1, exp2);
}

int subIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    return expNode->left->evaluateInteger(symbolTable, expNode->left) - expNode->right->evaluateInteger(symbolTable, expNode->right);
}

ExpNode* SubtractionExpressionExpAction(ExpNode* exp1, ExpNode* exp2) {
    printf("SubtractionExpressionExpAction \n");
    return newExpNode(&justIntegerMode, &subIntegers, NULL, -1, NULL, exp1, exp2);
}

char* mulStringInt(SymbolTable* symbolTable, ExpNode* expNode) {
    char* s;
    int value;
    if (expNode->left->getMode(symbolTable, expNode->left) == INT) {
        value = expNode->left->evaluateInteger(symbolTable, expNode->left);
        s = expNode->right->evaluateString(symbolTable, expNode->right);
    } else {
        value = expNode->right->evaluateInteger(symbolTable, expNode->right);
        s = expNode->left->evaluateString(symbolTable, expNode->left);
    }
    char* aux = realloc(expNode->cvalue, strlen(s) * value + 1);
    if(aux == NULL){
        outOfMemory(state.errorManager);
        return expNode->cvalue;
    }else{
        expNode->cvalue = aux;
    }

    int i = 0;
    for (int j = 0; j < value; ++j) {
        for (int k = 0; s[k]; ++k) {
            expNode->cvalue[i++] = s[k];
        }
    }
    expNode->cvalue[i] = 0;
    return expNode->cvalue;
}

int mulIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    return expNode->left->evaluateInteger(symbolTable, expNode->left) * expNode->right->evaluateInteger(symbolTable, expNode->right);
}

ExpNode* MultiplicationExpressionExpAction(ExpNode* exp1, ExpNode* exp2) {
    printf("MultiplicationExpressionExpAction \n");
    return newExpNode(&notTwoStringsMode, &mulIntegers, &mulStringInt, -1, NULL, exp1, exp2);
}

int divIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    int den = expNode->right->evaluateInteger(symbolTable, expNode->right);
    if (den == 0) {
        divByZero(state.errorManager);
        return 0;
    }
    return expNode->left->evaluateInteger(symbolTable, expNode->left) / den;
}

ExpNode* DivisionExpressionExpAction(ExpNode* exp1, ExpNode* exp2) {
    printf("DivisionExpressionExpAction \n");
    return newExpNode(&justIntegerMode, &divIntegers, NULL, -1, NULL, exp1, exp2);
}

/*************************************************************************************
**                       FACTORS
**************************************************************************************/

ExpNode* FactorExpressionExpAction(ExpNode* factor) {
    printf("FactorExpressionExpAction \n");
    return factor;
}

ExpNode* ConstantFactorExpAction(ExpNode* expNode) {
    printf("ConstantFactorExpAction() \n");
    return expNode;
}

ExpNode* VariableFactorExpAction(ExpNode* expNode) {
    printf("VariableFactorGrammarAction() \n");
    return expNode;
}

/*************************************************************************************
**                      VARIABLES
**************************************************************************************/

static void undefinedVariable(SymbolTable* symbolTable, char* varName) {
    state.succeed = FALSE;
    addUndefindedVariable(state.errorManager, varName);
    addSymbolToTable(symbolTable, newSymbol(varName, "{undefined}", STRING));
}

char* returnStringVariable(SymbolTable* symbolTable, ExpNode* expNode) {
    SymbolEntry* entry = getEntryFromTable(symbolTable, expNode->cvalue);
    if (entry == NULL) { // The variable was not defined
        undefinedVariable(symbolTable, expNode->cvalue);
        return returnStringVariable(symbolTable, expNode);
    }
    return entry->value;
}

int returnIntegerVariable(SymbolTable* symbolTable, ExpNode* expNode) {
    return atoi(returnStringVariable(symbolTable, expNode));
}

ExpNode* VariableSubscriptExpAction(char* varName, int index) {
    printf("VariableSubscriptExpAction(%s[%d]) \n", varName, index);
    char* cvalue = malloc(strlen(varName) + 8);
    if(cvalue == NULL){
        outOfMemory(state.errorManager);
        return NULL;
    }
    sprintf(cvalue, "%s[%d]", varName, index);
    printf("%s", cvalue);
    return newExpNode(&varMode, &returnIntegerVariable, &returnStringVariable, index, cvalue, NULL, NULL);
}

ExpNode* VariableExpAction(char* varName) {
    printf("VariableExpAction(%s) \n", varName);
    char* cvalue = malloc(strlen(varName) + 1);
    if(cvalue == NULL){
        outOfMemory(state.errorManager);
        return NULL;
    }
    strcpy(cvalue, varName);
    return newExpNode(&varMode, &returnIntegerVariable, &returnStringVariable, -1, cvalue, NULL, NULL);
}

/*************************************************************************************
**                      CONSTANTS
**************************************************************************************/
int returnIntegerValue(SymbolTable* symbolTable, ExpNode* expNode) {
    return expNode->ivalue;
}

char* returnIntegerAsString(SymbolTable* symbolTable, ExpNode* expNode) {
    if (expNode->cvalue == NULL) {
        int length = snprintf(NULL, 0, "%d", expNode->ivalue);
        expNode->cvalue = malloc(length + 1);
        snprintf(expNode->cvalue, length + 1, "%d", expNode->ivalue);
    }
    return expNode->cvalue;
}

ExpNode* IntegerConstantExpAction(const int value) {
    printf("IntegerConstantGrammarAction(%d) \n", value);
    return newExpNode(&integerMode, &returnIntegerValue, &returnIntegerAsString, value, NULL, NULL, NULL);
}

char* returnString(SymbolTable* symbolTable, struct ExpResultNode* expNode) {
    return expNode->cvalue;
}

ExpResultNode* StringConstantExpAction(char* string) {
    char* cvalue = malloc(strlen(string) + 1);
    if(cvalue == NULL){
        outOfMemory(state.errorManager);
        return NULL;
    }
    strcpy(cvalue, string);
    return newExpResultNode(&returnString, cvalue, NULL);
}

/*************************************************************************************
**                           Free
**************************************************************************************/

void FreeExpResultNode(ExpResultNode* node) {
    if (node == NULL)
        return;
    if (node->cvalue != NULL){
        free(node->cvalue);
    }
    FreeExpNode(node->exp);
    free(node);
}

void FreeExpNode(ExpNode* node) {
    if (node == NULL)
        return;
    if (node->cvalue != NULL)
        free(node->cvalue);
    FreeExpNode(node->right);
    FreeExpNode(node->left);
    free(node);
}