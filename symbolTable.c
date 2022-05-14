#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "./include/symbolTable.h"

typedef enum SymbolType {
    STRING, 
    INT
} SymbolType; 

typedef struct SymbolEntry {
    char * key; 
    char * value;
    SymbolType type; 
    struct SymbolEntry * next; 
} SymbolEntry; 

typedef struct SymbolTable {
    SymbolEntry * top; 
    struct SymbolTable * previousTable; 
} SymbolTable; 

SymbolEntry * newSymbol(char * name, char * value, SymbolType type); 
void printSymbolEntry(SymbolEntry * entry); 
void printSymbolEntryList(SymbolEntry * entry); 
SymbolEntry * getSymbolWithKey(SymbolEntry * entry, char * key); 

SymbolTable * newScope(SymbolTable * previousTable); 
SymbolTable * newEmptySymbolTable(); 
void printSymbolTable(SymbolTable * table);
void addSymbolToTable(SymbolTable * table, SymbolEntry * entry); 
SymbolEntry * getEntryFromTable(SymbolTable * table, char * key); 

// Pure, simple, symbol manipulation

SymbolEntry * newSymbol(char * name, char * value, SymbolType type) {
    SymbolEntry * entry = malloc(sizeof(SymbolEntry)); 
    entry->key = name; 
    entry->value = value; 
    entry->type = type; 
    return entry; 
}

void printSymbolEntry(SymbolEntry * entry) {
    if (entry == NULL)
        printf("<%10s, %20s, %15s >\n", "NONE", "NONE", "NONE"); 
    else 
        printf("<%10s, %20s, %15s >\n", entry->key, entry->value, entry->type == INT ? "type:int" : "type:string"); 
}

void printSymbolEntryList(SymbolEntry * entry) {
    if (entry == NULL)
        return; 
    printSymbolEntry(entry); 
    printSymbolEntryList(entry->next); 
}

// Recurse through the symbol list 

SymbolEntry * getSymbolWithKey(SymbolEntry * entry, char * key) {

    if (entry == NULL)
        return NULL; 
    
    if (strcmp(entry->key, key) == 0)
        return entry; 

    return getSymbolWithKey(entry->next, key); 
}

// Table creation 

SymbolTable * newScope(SymbolTable * previousTable) {
    SymbolTable * table =  malloc(sizeof(SymbolTable)); 
    table->top = NULL; 
    table->previousTable = previousTable; 
    return table; 
}

SymbolTable * newEmptySymbolTable() {
    return newScope(NULL); 
}

// Table debugging 

void printSymbolTable(SymbolTable * table) {

    if (table == NULL) {
        printf("----EOT----\n");
        return; 
    }

    printf("----TABLE---\n"); 
    printSymbolEntryList(table->top); 
    printSymbolTable(table->previousTable); 
}

// Table manipulation and referencing

void addSymbolToTable(SymbolTable * table, SymbolEntry * entry) {
    entry->next = table->top; 
    table->top = entry; 
}

SymbolEntry * getEntryFromTable(SymbolTable * table, char * key) {
    if (table == NULL)
        return NULL; 

    SymbolEntry * res = getSymbolWithKey(table->top, key); 
    return res == NULL ? getEntryFromTable(table->previousTable, key) : res;   
} 