#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum SymbolType {
    INT = 0,
    STRING = 1
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

// Symbol creation. A symbol cannot be deleted individually, it is destroyed in scope deletion. 
SymbolEntry * newSymbol(char * name, char * value, SymbolType type); 
SymbolEntry * getSymbolWithKey(SymbolEntry * entry, char * key); 

// Scope creation and deletion 
SymbolTable * newScope(SymbolTable * previousTable); 
SymbolTable * newEmptySymbolTable();
SymbolTable * deleteScope(SymbolTable * table);

// Symbol manipulation into tables
void addSymbolToTable(SymbolTable * table, SymbolEntry * entry); 
SymbolEntry * getEntryFromTable(SymbolTable * table, char * key); 

// Debugging
void printSymbolEntry(SymbolEntry * entry); 
void printSymbolEntryList(SymbolEntry * entry); 
void printSymbolTable(SymbolTable * table);

#endif