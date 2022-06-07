// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "./include/symbolTable.h"
#include "./include/shared.h"

// Pure, simple, symbol manipulation
SymbolEntry* newSymbol(char* name, char* value, SymbolType type) {
    SymbolEntry* entry = malloc(sizeof(SymbolEntry));
    if(entry == NULL){
        outOfMemory(state.errorManager);
        return NULL;
    }
    entry->key = malloc(strlen(name) + 1);
    if(entry->key == NULL){
        free(entry);
        outOfMemory(state.errorManager);
        return NULL;
    }
    strcpy(entry->key, name);
    entry->value = value;
    entry->type = type;
    return entry;
}

void printSymbolEntry(SymbolEntry* entry) {
    if (entry == NULL)
        printf("<%10s, %20s, %15s >\n", "NONE", "NONE", "NONE");
    else
        printf("<%10s, %20s, %15s >\n", entry->key, entry->value, entry->type == INT ? "type:int" : "type:string");
}

void printSymbolEntryList(SymbolEntry* entry) {
    if (entry == NULL)
        return;
    printSymbolEntry(entry);
    printSymbolEntryList(entry->next);
}

// Recurse through the symbol list

SymbolEntry* getSymbolWithKey(SymbolEntry* entry, char* key) {

    if (entry == NULL)
        return NULL;

    if (strcmp(entry->key, key) == 0)
        return entry;

    return getSymbolWithKey(entry->next, key);
}

// Delete symbol list

void deleteSymbolEntryList(SymbolEntry* entry) {

    if (entry == NULL)
        return;

    if (entry->key != NULL)
        free(entry->key);

    deleteSymbolEntryList(entry->next);
    free(entry);
}

// Table creation

SymbolTable* newScope(SymbolTable* previousTable) {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    if(table == NULL){
        outOfMemory(state.errorManager);
        return NULL;
    }
    
    table->top = NULL;
    table->previousTable = previousTable;
    return table;
}

SymbolTable* newEmptySymbolTable() {
    return newScope(NULL);
}

// Table erasure

SymbolTable* deleteScope(SymbolTable* table) {

    if (table == NULL)
        return NULL;

    deleteSymbolEntryList(table->top);
    SymbolTable* aux = table->previousTable;
    free(table);
    return aux;
}

// Table debugging

void printSymbolTable(SymbolTable* table) {

    if (table == NULL) {
        printf("----EOT----\n");
        return;
    }

    printf("----TABLE---\n");
    printSymbolEntryList(table->top);
    printSymbolTable(table->previousTable);
}

// Table manipulation and referencing

void addSymbolToTable(SymbolTable* table, SymbolEntry* entry) {
    entry->next = table->top;
    table->top = entry;
}

SymbolEntry* getEntryFromTable(SymbolTable* table, char* key) {
    if (table == NULL)
        return NULL;

    SymbolEntry* res = getSymbolWithKey(table->top, key);
    return res == NULL ? getEntryFromTable(table->previousTable, key) : res;
}