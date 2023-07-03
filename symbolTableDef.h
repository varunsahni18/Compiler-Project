#ifndef SYMBOL_TABLEDEF
#define SYMBOL_TABLEDEF
/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/
#include <stdio.h>
#include <stdlib.h>
#include "astDef.h"
#include <stdbool.h>

// char *DataTypeMap[] = {"nulli", "integer", "real", "boolean"};
typedef enum DataType
{
    nulli,
    real,
    integer,
    boolean
} DataType;

typedef struct SymbolTableEntry
{

    // Data Present for each entry in Symbol Table
    char *lexeme;
    char *moduleName;
    int scopeLineNoStart;
    int scopeLineNoEnd;
    DataType type;
    bool isArray;
    bool isStatic; // if array
    int arrayRangeStart;
    int arrayRangeEnd; // if array
    int width;
    int offset;
    int nestingLevel;

    // Traversal Pointers for Each SymbolTableEntry
    struct SymbolTableEntry *next;
    struct SymbolTableEntry *head;
    struct SymbolTableEntry *tail;

} SymbolTableEntry;

typedef struct SymbolTable
{

    // data
    SymbolTableEntry **slot;
    int numOfSlots;
    char *moduleName;
    int nestingLevel;

    // traversal pointer links
    struct SymbolTable *child;
    struct SymbolTable *parent;
    struct SymbolTable *next;

} SymbolTable;

typedef struct FunctionSymbolTableEntry
{

    // Data Present for each function entry in Symbol Table
    char *moduleName;
    SymbolTableEntry *inputHead;
    SymbolTableEntry *outputHead;
    SymbolTable *symbolTable;

    // Traversal Pointers for Each FunctionSymbolTableEntry
    struct FunctionSymbolTableEntry *next;
    struct FunctionSymbolTableEntry *head;
    struct FunctionSymbolTableEntry *tail;

} FunctionSymbolTableEntry;

typedef struct FunctionSymbolTable
{

    FunctionSymbolTableEntry **slot;
    int numOfSlots;

} FunctionSymbolTable;

#endif