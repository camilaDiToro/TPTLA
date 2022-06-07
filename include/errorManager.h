#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

typedef enum ErrorType {
    INVALID_TYPE_FOR_LOOP = 0,
    DIV_BY_ZERO = 1,
    INVALID_TYPE_TWO_STRINGS = 2,
    INVALID_TYPE_JUST_INTEGER = 3,
    INVALID_OP = 4,
    UNDEFINED_VAR = 5,
    OUT_OF_MEMORY = 6
} ErrorType;

typedef struct ErrorNode {
    ErrorType type;
    char* msg;
    struct ErrorNode* next;
    void (*printError)(int errorNumber, struct ErrorNode* aux);
} ErrorNode;

typedef struct ErrorManager {
    int errorCount;
    int errorsShown;
    int outOfMemory;
    ErrorNode* first;
    ErrorNode* last;
} ErrorManager;

typedef void (*printErrorFunction)(int errorNumber, ErrorNode* aux);

ErrorManager* newErrorManager();

void addUndefindedVariable(ErrorManager* em, char* varName);
void* outOfMemory(ErrorManager* em);
void showErrors(ErrorManager* em);
void freeErrorManager(ErrorManager* em);
void invalidVariableTypeInForLoop(ErrorManager* em, char * varName);
void invalidSubOperation(ErrorManager* em);
void invalidMulOperation(ErrorManager* em);
void divByZero(ErrorManager* em);

#endif