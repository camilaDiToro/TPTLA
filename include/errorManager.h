#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

typedef enum ErrorType {
    UNDEFINED_VAR = 0,
    OUT_OF_MEMORY = 1,
    INVALID_TYPE = 2,
    INVALID_OP = 3
} ErrorType;

typedef struct ErrorNode {
    ErrorType type;
    char* msg;
    struct ErrorNode* next;
} ErrorNode;

typedef struct ErrorManager {
    int errorCount;
    int errorsShown;
    int outOfMemory;
    ErrorNode* first;
    ErrorNode* last;
} ErrorManager;

ErrorManager* newErrorManager();
void addUndefindedVariable(ErrorManager* em, char* varName);
void* outOfMemory(ErrorManager* em);
void showErrors(ErrorManager* em);
void freeErrorManager(ErrorManager* em);
void invalidVariableTypeInForLoop(ErrorManager* em, char * varName);

#endif