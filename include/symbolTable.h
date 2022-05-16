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

typedef struct SymbolEntry * SymbolEntryP;
typedef struct SymbolTable * SymbolTableP;


SymbolEntry * newSymbol(char * name, char * value, SymbolType type); 
void printSymbolEntry(SymbolEntry * entry); 
void printSymbolEntryList(SymbolEntry * entry); 
SymbolEntry * getSymbolWithKey(SymbolEntry * entry, char * key); 

SymbolTable * newScope(SymbolTable * previousTable); 
SymbolTable * newEmptySymbolTable(); 
void printSymbolTable(SymbolTable * table);
void addSymbolToTable(SymbolTable * table, SymbolEntry * entry); 
SymbolEntry * getEntryFromTable(SymbolTable * table, char * key); 

#endif