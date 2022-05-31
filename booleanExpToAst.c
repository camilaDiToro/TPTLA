#include "include/expToAst.h"

extern ExpNode* newExpNode(getModeFunction gm, evaluateIntegerFunction ei, evaluateStringFunction es, int ivalue, char* cvalue, ExpNode* left, ExpNode* right);
extern SymbolType orMode(SymbolTable* symbolTable, struct ExpNode* expNode);
extern SymbolType justIntegerMode(SymbolTable* symbolTable, struct ExpNode* expNode);

static int equality(int elem1, int elem2);
static int nequality(int elem1, int elem2);
static int less(int elem1, int elem2);
static int lessOrEqual(int elem1, int elem2);
static int greater(int elem1, int elem2);
static int greaterOrEqual(int elem1, int elem2);
static int addOp(int elem1, int elem2);
static int orOp(int elem1, int elem2);

/* In boolean expressions, the ivalue is used to store the type of the boolean expression. */

typedef int (*booleanFunction)(int elem1, int elem2);

static booleanFunction booleanFunctions[] = {
    equality, nequality, less, lessOrEqual, greater, greaterOrEqual, addOp, orOp
};

char* compareStrings(SymbolTable* symbolTable, ExpNode* expNode) {
    char* left = expNode->left->evaluateString(symbolTable, expNode->left);
    char* right = expNode->right->evaluateString(symbolTable, expNode->right);
    expNode->cvalue = realloc(expNode->cvalue, 2);
    expNode->cvalue[1] = 0;
    booleanFunction fun = booleanFunctions[expNode->ivalue];
    if (fun(strcmp(left, right), 0)) {
        expNode->cvalue[0] = '1';
    } else {
        expNode->cvalue[0] = '0';
    }
    return expNode->cvalue;
}

int compareIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    booleanFunction fun = booleanFunctions[expNode->ivalue];
    return fun(expNode->left->evaluateInteger(symbolTable, expNode->left), expNode->right->evaluateInteger(symbolTable, expNode->right));
}

ExpNode* BooleanExpressionExpAction(ExpNode* exp1, ExpNode* exp2, BooleanExpType type) {
    if(type < BOOLEAN_AND){
        return newExpNode(&orMode, &compareIntegers, &compareStrings, type, NULL, exp1, exp2);
    }
    return newExpNode(&justIntegerMode, &compareIntegers, NULL, type, NULL, exp1, exp2);
}

int notIntegers(SymbolTable* symbolTable, ExpNode* expNode) {
    return !expNode->left->evaluateInteger(symbolTable, expNode->left);
}

ExpNode* NotExpressionExpAction(ExpNode* exp1){
    return newExpNode(&justIntegerMode, &notIntegers, NULL, BOOLEAN_NOT, NULL, exp1, NULL);
}

static int equality(int elem1, int elem2) {
    return elem1 == elem2;
}
static int nequality(int elem1, int elem2) {
    return elem1 != elem2;
}
static int less(int elem1, int elem2) {
    return elem1 < elem2;
}
static int lessOrEqual(int elem1, int elem2) {
    return elem1 <= elem2;
}
static int greater(int elem1, int elem2) {
    return elem1 > elem2;
}
static int greaterOrEqual(int elem1, int elem2) {
    return elem1 >= elem2;
}
static int addOp(int elem1, int elem2) {
    return elem1 && elem2;
}
static int orOp(int elem1, int elem2) {
    return elem1 || elem2;
}