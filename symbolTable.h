#ifndef SYMBOL_TABLEH
#define SYMBOL_TABLEH
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
#include "symbolTableDef.h"

void processSwitchCaseStmt(AstNode *a, SymbolTable *st);
void AstTraversal(AstNode *node);
void processDriverProgram(AstNode *a);
void processIterativeWhile(AstNode *a, SymbolTable *st);
void processIterativeFor(AstNode *a, SymbolTable *st);
void processModuleReuseStatement(AstNode *a);
void processModuleReuseStatementTraverse(AstNode *a);
void processGetValueStatement(AstNode *a);
void processAssignmentStatement(AstNode *a);
bool searchIOlist(FunctionSymbolTableEntry *fstentry, char *key);
void processModuleDeclaration(AstNode *a);
void processDeclareStmt(AstNode *a, SymbolTable *st);
void printFunctionSymbolTable();
void printOneSymbolTable(SymbolTable *st);
FunctionSymbolTableEntry *searchFunction(char *funcName);
SymbolTableEntry *searchVariable(SymbolTable *st, char *key);
void traversal(SymbolTable *st, AstNode *a);
FunctionSymbolTableEntry *populateFunctionSymbolTableEntry(
    char *moduleName, SymbolTableEntry *inputHead, SymbolTableEntry *outputHead, SymbolTable *symbolTable,
    FunctionSymbolTableEntry *next, FunctionSymbolTableEntry *head, FunctionSymbolTableEntry *tail);
SymbolTableEntry *populateSymbolTableEntry(char *lexeme, char *moduleName, int scopeLineNoStart, int scopeLineNoEnd,
                                           DataType type, bool isArray, bool isStatic, int arrayRangeStart, int arrayRangeEnd, int width, int offset, int nestingLevel,
                                           SymbolTableEntry *next, SymbolTableEntry *head, SymbolTableEntry *tail);

SymbolTableEntry *returnInputOutputList(SymbolTableEntry *head, SymbolTableEntry *newEntry);
FunctionSymbolTable *addFunctionToFunctionSymbolTable(FunctionSymbolTable *fst, FunctionSymbolTableEntry *en);
SymbolTable *addSymbolToSymbolTable(SymbolTable *st, SymbolTableEntry *en);
FunctionSymbolTable *initFst();
SymbolTable *initSt(int nl, char *moduleName);
int hashing(char *key);

#endif