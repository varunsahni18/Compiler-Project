#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
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
#include <string.h>
#include <stdbool.h>
#include "symbolTableDef.h"

char *DataTypeMap1[] = {"nulli", "real", "integer", "boolean"};

int numOfSlots = 43; // number of slots in hashTable ; number of slots in hashing table are selected as 43 to minimise collisions (tested for all primes)
int pi = 53;

FunctionSymbolTable *fst;
int offset = 0;
SymbolTable *GlobalSt;

int hashing(char *key) // function to calculate the hashing value of a particular key
{

    int hashValue = 0;
    int power = 1;
    for (int i = 0; i < strlen(key); i++)
    {

        hashValue = (hashValue + (key[i]) * power) % numOfSlots;
        power = (power * pi) % numOfSlots;
    }

    return hashValue;
}

SymbolTable *initSt(int nl, char *moduleName)
{

    SymbolTable *symTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    symTable->numOfSlots = numOfSlots;
    symTable->slot = (SymbolTableEntry **)malloc(numOfSlots * (sizeof(SymbolTableEntry *)));
    
    for (int i = 0; i < numOfSlots; i++)
    {
        // hash_table->slot[i] = (lookUp *)malloc(sizeof(lookUp));
        // hash_table->slot[i]->lexeme = "\0";
        // hash_table->slot[i]->type = 0;
        // hash_table->slot[i]->next = NULL;
        // hash_table->slot[i]->head = hash_table->slot[i];
        // hash_table->slot[i]->tail = hash_table->slot[i]->head;
        symTable->slot[i]= (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
        SymbolTableEntry *newEntry= symTable->slot[i];
        newEntry->lexeme = "\0";
        newEntry->moduleName = moduleName;
        newEntry->scopeLineNoStart = 0;
        newEntry->scopeLineNoEnd = 0;
        newEntry->type = 0;
        newEntry->isArray = 0;
        newEntry->isStatic = 0;
        newEntry->arrayRangeStart = 0;
        newEntry->arrayRangeEnd = 0;
        newEntry->width = 0;
        newEntry->offset = 0;
        newEntry->nestingLevel = nl;
        newEntry->next = NULL;
        newEntry->head = symTable->slot[i];
        newEntry->tail = symTable->slot[i];
        
    }
    symTable->child = (SymbolTable *)malloc(sizeof(SymbolTable));
    symTable->child = NULL;
    symTable->next = (SymbolTable *)malloc(sizeof(SymbolTable));
    symTable->next = NULL;
    symTable->parent = (SymbolTable *)malloc(sizeof(SymbolTable));
    symTable->parent = NULL;
    symTable->nestingLevel = nl;
    symTable->moduleName = moduleName;

    return symTable;
}
FunctionSymbolTable *initFst()
{
    fst = (FunctionSymbolTable *)malloc(sizeof(FunctionSymbolTable));
    fst->numOfSlots = numOfSlots;
    fst->slot = (FunctionSymbolTableEntry **)malloc(numOfSlots * (sizeof(FunctionSymbolTableEntry *)));

    for (int i = 0; i < numOfSlots; i++)
    {
        fst->slot[i] = (FunctionSymbolTableEntry *)malloc(sizeof(FunctionSymbolTableEntry));
        fst->slot[i]->moduleName = "\0";
        fst->slot[i]->head = fst->slot[i];
        fst->slot[i]->tail = fst->slot[i];
        fst->slot[i]->next = NULL;
        fst->slot[i]->symbolTable = NULL;
        fst->slot[i]->inputHead = NULL;
        fst->slot[i]->outputHead = NULL;
    }
    return fst;
}

SymbolTable *addSymbolToSymbolTable(SymbolTable *st, SymbolTableEntry *en)
{
    //printf("INSIDE ADD SYMBOL TO SYMBOLTABLE\n");
    char *lexeme = en->lexeme;
    int val = hashing(lexeme);
    SymbolTableEntry *tail = st->slot[val]->tail;
    tail->next = en;
    tail = tail->next;
    tail->next = NULL;
    return st;
}

FunctionSymbolTable *addFunctionToFunctionSymbolTable(FunctionSymbolTable *fst, FunctionSymbolTableEntry *en)
{
    //printf("INSIDE ADD FUNCTION TO SYMBOLTABLE\n");
    char *lexeme = en->moduleName;
    int val = hashing(lexeme);
   
    FunctionSymbolTableEntry *tail = fst->slot[val]->tail;
    
    tail->next = en;
    tail = tail->next;
    tail->next = NULL;
    //printf("INSIDE ADD FUNCTION TO SYMBOLTABLE\n");
    return fst;
}

SymbolTableEntry *returnInputOutputList(SymbolTableEntry *head, SymbolTableEntry *newEntry)
{
    if (head == NULL)
    {
        head = newEntry;
        head->next = NULL;
        return head;
    }
    SymbolTableEntry *tail = head;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    tail->next = newEntry;
    tail = tail->next;
    tail->next = NULL;
    return head;
}

SymbolTableEntry *populateSymbolTableEntry(char *lexeme, char *moduleName, int scopeLineNoStart, int scopeLineNoEnd,
                                           DataType type, bool isArray, bool isStatic, int arrayRangeStart, int arrayRangeEnd, int width, int offset, int nestingLevel,
                                           SymbolTableEntry *next, SymbolTableEntry *head, SymbolTableEntry *tail)
{
    //printf("C1\n");

    SymbolTableEntry *newEntry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    //printf("C2\n");

    newEntry->lexeme = lexeme;
    newEntry->moduleName = moduleName;
    newEntry->scopeLineNoStart = scopeLineNoStart;
    newEntry->scopeLineNoEnd = scopeLineNoEnd;
    newEntry->type = type;
    newEntry->isArray = isArray;
    newEntry->isStatic = isStatic;
    newEntry->arrayRangeStart = arrayRangeStart;
    newEntry->arrayRangeEnd = arrayRangeEnd;
    newEntry->width = width;
    newEntry->offset = offset;
    newEntry->nestingLevel = nestingLevel;
    newEntry->next = next;
    newEntry->head = head;
    newEntry->tail = tail;

    return newEntry;
}

FunctionSymbolTableEntry *populateFunctionSymbolTableEntry(
    char *moduleName, SymbolTableEntry *inputHead, SymbolTableEntry *outputHead, SymbolTable *symbolTable,
    FunctionSymbolTableEntry *next, FunctionSymbolTableEntry *head, FunctionSymbolTableEntry *tail)
{

    FunctionSymbolTableEntry *newEntry = (FunctionSymbolTableEntry *)malloc(sizeof(FunctionSymbolTableEntry));

    newEntry->moduleName = moduleName;
    newEntry->inputHead = inputHead;
    newEntry->outputHead = outputHead;
    newEntry->symbolTable = symbolTable;
    newEntry->next = next;
    newEntry->head = head;
    newEntry->tail = tail;

    return newEntry;
}

void traversal(SymbolTable *st, AstNode *a)
{
    // declare, for, functionDef,
    if (a->label == AST_DECLARE_STMT)
    {
        Terminal type = a->children->tail->tokenIfLeaf->type;
        DataType dt = nulli;
        int width = 0;
        if (type == INTEGER)
        {
            dt = integer;
            width = 2;
        }
        else if (type == REAL)
        {
            dt = real;
            width = 4;
        }
        else if (type == BOOLEAN)
        {
            dt = boolean;
            width = 1;
        }

        AstNode *idList = a->children->head;
        AstNode *id = idList->children->head;

        // Primitive Data Type
        if (dt != nulli)
        {
            while (id != NULL)
            {
                SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                      dt, false, false, 0, 0, width, offset, st->nestingLevel, NULL, NULL, NULL);
                addSymbolToSymbolTable(st, newEntry);
                offset += width;

                id = id->compression_node;
                if (id != NULL)
                    id = id->children->head;
            }
        }

        // Array
        else if (type == ARRAY)
        {
            AstNode *child = a->children->tail; // pointing to array
            Terminal childType = child->children->tail->tokenIfLeaf->type;
            AstNode *range = child->children->head; // pointing to range
            Token *startingRange = range->children->head->tokenIfLeaf;
            Token *endingRange = range->children->head->nextNode->tokenIfLeaf;
            bool isStatic = 1;

            if (startingRange->type == ID)
            {
                isStatic = 0;
                if (endingRange->type == ID)
                {
                }
                else if (endingRange->type == NUM)
                {
                }
                else
                {
                    // report error
                }
            }
            else
            {
                if (startingRange->type == NUM)
                {
                    if (endingRange->type == ID)
                    {
                        isStatic = 0;
                    }
                    else if (endingRange->type == NUM)
                    {
                    }
                    else
                    {
                        // report error
                    }
                }
                else
                {
                    // report error
                }
            }
            if (isStatic)
            {
                int sr = startingRange->val->num;
                int er = endingRange->val->num;

                // todo type checking of startRange and end Range
                width = 1;
                if (childType == INTEGER)
                {
                    dt = integer;
                    width += 2 * (er - sr + 1);
                }
                else if (childType == REAL)
                {
                    dt = real;
                    width += 4 * (er - sr + 1);
                }
                else if (childType == BOOLEAN)
                {
                    dt = boolean;
                    width += 1 * (er - sr + 1);
                }
                while (id != NULL)
                {

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                          dt, true, true, sr, er, width, offset, st->nestingLevel, NULL, NULL, NULL);
                    addSymbolToSymbolTable(st, newEntry);
                    offset += width;
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                }
            }
            else
            { // is Dynamic
                width = 1;
                while (id != NULL)
                {

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                          dt, true, false, 0, 0, width, offset, st->nestingLevel, NULL, NULL, NULL);
                    addSymbolToSymbolTable(st, newEntry);
                    offset += width;
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                }
            }
        }
        else
        {
            // REPORT ERROR
        }
    }
    else if (a->label == AST_ITERATIVE_FOR)
    {
        AstNode *IDfor = a->children->head;
        char *lexeme = IDfor->tokenIfLeaf->lexeme;
        SymbolTableEntry *newEntry = populateSymbolTableEntry(lexeme, st->moduleName, 0, 0,
                                                              integer, false, false, 0, 0, 2, offset, 1 + st->nestingLevel, NULL, NULL, NULL);
        offset += 2;
        SymbolTable *newSt = initSt(1 + st->nestingLevel, st->moduleName);
        if (st->child == NULL)
        {
            st->child = newSt;
            newSt->parent = st;
            st->child->next = NULL;
        }
        else
        {
            SymbolTable *temp = st->child;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newSt;
            newSt->parent = st;
            temp = temp->next;
            temp->next = NULL;
        }
        addSymbolToSymbolTable(newSt, newEntry);
    }
    else if (a->label == AST_ITERATIVE_WHILE)
    {
        SymbolTable *newSt = initSt(1 + st->nestingLevel, st->moduleName);
        if (st->child == NULL)
        {
            st->child = newSt;
            newSt->parent = st;
            st->child->next = NULL;
        }
        else
        {
            SymbolTable *temp = st->child;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newSt;
            newSt->parent = st;
            temp = temp->next;
            temp->next = NULL;
        }
    }
    else if (a->label == AST_MODULE)
    {
        char *funcName = a->children->head->tokenIfLeaf->lexeme;

        AstNode *inputList = a->children->head->nextNode;
        AstNode *outputList = inputList->nextNode;

        // Starting with traversal of input list
        AstNode *id = inputList->children->head;
        SymbolTableEntry *inputHead = NULL;

        while (id != NULL)
        {
            Terminal type = id->nextNode->tokenIfLeaf->type;
            DataType dt = nulli;
            int width = 0;
            if (type == INTEGER)
            {
                dt = integer;
                width = 2;
            }
            else if (type == REAL)
            {
                dt = real;
                width = 4;
            }
            else if (type == BOOLEAN)
            {
                dt = boolean;
                width = 1;
            }

            // Primitive Data Type
            if (dt != nulli)
            {
                SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, false, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                inputHead = returnInputOutputList(inputHead, newEntry);
                offset += width;
            }

            // Array
            else if (type == ARRAY)
            {
                // AstNode *child = a ->children ->tail;//pointing to array
                // Token *childType= child->children->tail->tokenIfLeaf;
                // AstNode *range= child->children->head;//pointing to range

                AstNode *arr = id->nextNode;
                AstNode *range = arr->children->head;
                Terminal childType = range->nextNode->tokenIfLeaf->type;
                Token *startingRange = range->children->head->tokenIfLeaf;
                Token *endingRange = range->children->head->nextNode->tokenIfLeaf;
                bool isStatic = 1;

                if (startingRange->type == ID)
                {
                    isStatic = 0;
                    if (endingRange->type == ID)
                    {
                    }
                    else if (endingRange->type == NUM)
                    {
                    }
                    else
                    {
                        // report error
                    }
                }
                else
                {
                    if (startingRange->type == NUM)
                    {
                        if (endingRange->type == ID)
                        {
                            isStatic = 0;
                        }
                        else if (endingRange->type == NUM)
                        {
                        }
                        else
                        {
                            // report error
                        }
                    }
                    else
                    {
                        // report error
                    }
                }
                if (isStatic)
                {
                    int sr = startingRange->val->num;
                    int er = endingRange->val->num;

                    // todo type checking of startRange and end Range
                    width = 1;
                    if (childType == INTEGER)
                    {
                        dt = integer;
                        width += 2 * (er - sr + 1);
                    }
                    else if (childType == REAL)
                    {
                        dt = real;
                        width += 4 * (er - sr + 1);
                    }
                    else if (childType == BOOLEAN)
                    {
                        dt = boolean;
                        width += 1 * (er - sr + 1);
                    }

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, true, sr, er, width, offset, 0, NULL, NULL, NULL);
                    inputHead = returnInputOutputList(inputHead, newEntry);

                    offset += width;
                }
                else
                { // is Dynamic
                    width = 1;

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                    inputHead = returnInputOutputList(inputHead, newEntry);

                    offset += width;
                }
            }

            id = id->compression_node;
            if (id != NULL)
                id = id->children->head;
        }

        // Traversing Output List
        id = outputList->children->head;
        SymbolTableEntry *outputHead = NULL;

        while (id != NULL)
        {
            Terminal type = id->nextNode->tokenIfLeaf->type;
            DataType dt = nulli;
            int width = 0;
            if (type == INTEGER)
            {
                dt = integer;
                width = 2;
            }
            else if (type == REAL)
            {
                dt = real;
                width = 4;
            }
            else if (type == BOOLEAN)
            {
                dt = boolean;
                width = 1;
            }

            // Primitive Data Type
            if (dt != nulli)
            {
                SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, false, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                outputHead = returnInputOutputList(outputHead, newEntry);
                offset += width;
            }

            // Array
            else if (type == ARRAY)
            {
                // AstNode *child = a ->children ->tail;//pointing to array
                // Token *childType= child->children->tail->tokenIfLeaf;
                // AstNode *range= child->children->head;//pointing to range

                AstNode *arr = id->nextNode;
                AstNode *range = arr->children->head;
                Terminal childType = range->nextNode->tokenIfLeaf->type;
                Token *startingRange = range->children->head->tokenIfLeaf;
                Token *endingRange = range->children->head->nextNode->tokenIfLeaf;
                bool isStatic = 1;

                if (startingRange->type == ID)
                {
                    isStatic = 0;
                    if (endingRange->type == ID)
                    {
                    }
                    else if (endingRange->type == NUM)
                    {
                    }
                    else
                    {
                        // report error
                    }
                }
                else
                {
                    if (startingRange->type == NUM)
                    {
                        if (endingRange->type == ID)
                        {
                            isStatic = 0;
                        }
                        else if (endingRange->type == NUM)
                        {
                        }
                        else
                        {
                            // report error
                        }
                    }
                    else
                    {
                        // report error
                    }
                }
                if (isStatic)
                {
                    int sr = startingRange->val->num;
                    int er = endingRange->val->num;

                    // todo type checking of startRange and end Range
                    width = 1;
                    if (childType == INTEGER)
                    {
                        dt = integer;
                        width += 2 * (er - sr + 1);
                    }
                    else if (childType == REAL)
                    {
                        dt = real;
                        width += 4 * (er - sr + 1);
                    }
                    else if (childType == BOOLEAN)
                    {
                        dt = boolean;
                        width += 1 * (er - sr + 1);
                    }

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, true, sr, er, width, offset, 0, NULL, NULL, NULL);
                    outputHead = returnInputOutputList(outputHead, newEntry);

                    offset += width;
                }
                else
                { // is Dynamic
                    width = 1;

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                    outputHead = returnInputOutputList(outputHead, newEntry);

                    offset += width;
                }
            }

            id = id->compression_node;
            if (id != NULL)
                id = id->children->head;
        }
        SymbolTable *symTable = initSt(1, funcName);
        FunctionSymbolTableEntry *fstEntry = populateFunctionSymbolTableEntry(
            funcName, inputHead, outputHead, symTable, NULL, NULL, NULL);
        addFunctionToFunctionSymbolTable(fst, fstEntry);
    }
    // else if()
    // todo
}

SymbolTableEntry *searchVariable(SymbolTable *st, char *key)

{
    int val = hashing(key);

    SymbolTableEntry *en = st->slot[val];
    SymbolTableEntry *temp = en;

    while (temp != NULL)
    {
        int strcmpval = strcmp(temp->lexeme, key);
        if (strcmpval == 0)
        {
            return en;
        }
        temp = temp->next;
    }
    return NULL;
}

FunctionSymbolTableEntry *searchFunction(char *funcName)
{
    int val = hashing(funcName);
    FunctionSymbolTableEntry *en = fst->slot[val];
    FunctionSymbolTableEntry *temp = en;
    int count=0;
    //printf("%d\n",count);
    while (temp != NULL)
    {
        if (strcmp(temp->moduleName, funcName) == 0)
            return en;
        temp = temp->next;
        count++;
        //printf("%d\n",count);
    }
    return NULL;
}

void printOneSymbolTable(SymbolTable *st)
{
    if (st == NULL)
        return;
    for (int i = 0; i < numOfSlots; i++)
    {
        SymbolTableEntry *entry = st->slot[i]->next;
        while (entry!=NULL && entry->lexeme != "\0")
        {

            printf("%s ", (entry->lexeme));
            printf("%s ", (entry->moduleName));
            printf(" [%d %d] ", (entry->scopeLineNoStart), (entry->scopeLineNoEnd));
            printf("%s ", DataTypeMap1[entry->type]);
            printf("%d ", (entry->isArray));
            printf("%d ", (entry->isStatic));
            printf("[%d %d] ", (entry->arrayRangeStart), (entry->arrayRangeEnd));
            printf("%d ", (entry->width));
            printf("%d ", (entry->offset));
            printf("%d \n", (entry->nestingLevel));

            entry = entry->next;
        }
    }
    printOneSymbolTable(st->child);
    printOneSymbolTable(st->next);
}

void printFunctionSymbolTable()
{
    
    char *funcName="driver program";
   FunctionSymbolTableEntry *sr= searchFunction(funcName);
   
    for (int i = 0; i < numOfSlots; i++)
    {
        FunctionSymbolTableEntry *fstEntry = fst->slot[i]->next;
       
      
        
        while (fstEntry != NULL && fstEntry->moduleName!="\0")
        {

            SymbolTableEntry *entry = fstEntry->inputHead;
            
            while (entry!=NULL && entry->lexeme != "\0")
            {
               
                printf("%s ", (entry->lexeme)); // input parameters, output parameters
                printf("%s ", (entry->moduleName));
                printf("[%d %d] ", (entry->scopeLineNoStart), (entry->scopeLineNoEnd));
                printf("%s ", DataTypeMap1[entry->type]);
                printf("%d ", (entry->isArray));
                printf("%d ", (entry->isStatic));
                printf("[%d %d] ", (entry->arrayRangeStart), (entry->arrayRangeEnd));
                printf("%d ", (entry->width));
                printf("%d ", (entry->offset));
                printf("%d \n", (entry->nestingLevel));
                entry = entry->next;
            }
            entry = fstEntry->outputHead;
            while (entry!=NULL && entry->lexeme != "\0")
            {
                printf("%s ", (entry->lexeme));
                printf("%s ", (entry->moduleName));
                printf("[%d %d] ", (entry->scopeLineNoStart), (entry->scopeLineNoEnd));
                printf("%s ", DataTypeMap1[entry->type]);
                printf("%d ", (entry->isArray));
                printf("%d ", (entry->isStatic));
                printf("[%d %d] ", (entry->arrayRangeStart), (entry->arrayRangeEnd));
                printf("%d ", (entry->width));
                printf("%d ", (entry->offset));
                printf("%d \n", (entry->nestingLevel));
                entry = entry->next;
            }
           
            printOneSymbolTable(fstEntry->symbolTable);
            if(fstEntry->next == NULL)
                break;
            fstEntry = fstEntry->next;
        }
    }
}

void processDeclareStmt(AstNode *a, SymbolTable *st)
{
    if (a->label == AST_DECLARE_STMT)
    {
        DataType dt = nulli;
        int width = 0;
        AstNode *idList = a->children->head;
        AstNode *id = idList->children->head;

        bool isArray = false;
        if(a->children->tail->label == AST_ARRAY)
            isArray = true;
        else{
            Terminal type = a->children->tail->tokenIfLeaf->type;
            if (type == INTEGER)
            {
                dt = integer;
                width = 2;
            }
            else if (type == REAL)
            {
                dt = real;
                width = 4;
            }
            else if (type == BOOLEAN)
            {
                dt = boolean;
                width = 1;
            }

            // Primitive Data Type
            if (dt != nulli)
            {
                while (id != NULL)
                {
                    //printf("Inside while loop\n");
                    // if (st == NULL)
                    //     //printf("NULL1\n");
                    // else
                    //     printf("NOT NULL\n");

                    //printf("%s %s\n", id->tokenIfLeaf->lexeme, st->moduleName);
                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                          dt, false, false, 0, 0, width, offset, st->nestingLevel, NULL, NULL, NULL);
                    //printf("Symbol table Entry populated\n");
                    //printf("%s\n", newEntry->lexeme);
                    if (searchVariable(st, newEntry->lexeme) != NULL)
                    {
                        // report error for redeclation in same scope
                        printf("ERROR in line %d: Redeclaration of identifier %s in same scope\n", id->tokenIfLeaf->line_no, id->tokenIfLeaf->lexeme);
                        break;
                    }
                    //printf("NULL3\n");
                    addSymbolToSymbolTable(st, newEntry);
                    //printf("NULL4\n");
                    offset += width;
                    //printf("Doing a lot of work\n");
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                }
            }
        }


        // Array
        if(isArray)
        {
            AstNode *child = a->children->tail; // pointing to array
            Terminal childType = child->children->tail->tokenIfLeaf->type;
            AstNode *range = child->children->head; // pointing to range
            Token *startingRange = range->children->head->children->head->tokenIfLeaf;
            Token *endingRange = range->children->tail->children->head->tokenIfLeaf;
            bool isStatic = 1;

            if(startingRange->type == ID || endingRange->type == ID){
                isStatic = 0;
            }
            else if(startingRange->type == NUM && endingRange->type == NUM)
                isStatic = 1;
            else{
                int line=9;
                printf("ERROR in Line No. %d : Array Range Index Bound not in proper format\n",line);
                return;
            }
            if (isStatic)
            {
                int sr = startingRange->val->num;
                int er = endingRange->val->num;

                int width = 1;
                DataType dt;
                if (childType == INTEGER)
                {
                    dt = integer;
                    width += 2 * (er - sr + 1);
                }
                else if (childType == REAL)
                {
                    dt = real;
                    width += 4 * (er - sr + 1);
                }
                else if (childType == BOOLEAN)
                {
                    dt = boolean;
                    width += 1 * (er - sr + 1);
                }
                while (id != NULL)
                {

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                          dt, true, true, sr, er, width, offset, st->nestingLevel, NULL, NULL, NULL);
                    if (searchVariable(st, id->tokenIfLeaf->lexeme) != NULL)
                    {
                        // report error for redeclation in same scope
                        printf("ERROR in line %d: Redeclaration of identifier %s in same scope\n", id->tokenIfLeaf->line_no, id->tokenIfLeaf->lexeme);
                        break;
                    }
                    addSymbolToSymbolTable(st, newEntry);
                    offset += width;
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                }
            }
            else
            { // is Dynamic
                width = 1;
                while (id != NULL)
                {

                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, st->moduleName, 0, 0,
                                                                          dt, true, false, 0, 0, width, offset, st->nestingLevel, NULL, NULL, NULL);
                    if (searchVariable(st, id->tokenIfLeaf->lexeme) != NULL)
                    {
                        // report error for redeclation in same scope
                        printf("ERROR in line %d: Redeclaration of identifier %s in same scope\n", id->tokenIfLeaf->line_no, id->tokenIfLeaf->lexeme);
                        break;
                    }
                    addSymbolToSymbolTable(st, newEntry);
                    offset += width;
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                }
            }
        }
    }
}

void processModuleDeclaration(AstNode *a)
{
    if (a->label == AST_MODULE)
    {
        char *funcName = a->children->head->tokenIfLeaf->lexeme;

        AstNode *inputList = a->children->head->nextNode;
        AstNode *outputList = inputList->nextNode;

        // Starting with traversal of input list
        AstNode *id = inputList->children->head;
        SymbolTableEntry *inputHead = NULL;

        while (id != NULL)
        {
            DataType dt = nulli;
            int width = 0;
            bool isArray = false;
            if(id->nextNode->label == AST_ARRAY)
                isArray = true;
            if(!isArray){
                Terminal type = id->nextNode->tokenIfLeaf->type;

                if (type == INTEGER)
                {
                    dt = integer;
                    width = 2;
                }
                else if (type == REAL)
                {
                    dt = real;
                    width = 4;
                }
                else if (type == BOOLEAN)
                {
                    dt = boolean;
                    width = 1;
                }

                // Primitive Data Type
                if (dt != nulli)
                {
                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, false, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                    inputHead = returnInputOutputList(inputHead, newEntry);
                    //printf("Success\n");
                    offset += width;
                }
            }
            else
            {
                    AstNode *range = id->nextNode->children->head; // pointing to range
                    Token *startingRange = range->children->head->children->head->tokenIfLeaf;
                    Token *endingRange = range->children->tail->children->head->tokenIfLeaf;
                    Terminal childType = id->nextNode->children->tail->tokenIfLeaf->type;
                    bool isStatic = 1;

                    if(startingRange->type == ID || endingRange->type == ID){
                        isStatic = 0;
                    }
                    else if(startingRange->type == NUM && endingRange->type == NUM)
                        isStatic = 1;
                    else{
                        int line = 11;
                        printf("ERROR in Line No. %d : Array Range Index Bound not in proper format\n",line);
                        id = id->compression_node;
                        if (id != NULL)
                            id = id->children->head;
                        continue;
                    }
                    if (isStatic)
                    {
                        int sr = startingRange->val->num;
                        int er = endingRange->val->num;

                        int width = 1;
                        DataType dt;
                        if (childType == INTEGER)
                        {
                            dt = integer;
                            width += 2 * (er - sr + 1);
                        }
                        else if (childType == REAL)
                        {
                            dt = real;
                            width += 4 * (er - sr + 1);
                        }
                        else if (childType == BOOLEAN)
                        {
                            dt = boolean;
                            width += 1 * (er - sr + 1);
                        }
                        SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, true, sr, er, width, offset, 0, NULL, NULL, NULL);
                        inputHead = returnInputOutputList(inputHead, newEntry);

                        offset+=width;
                    }
                    else { // is Dynamic
                        width = 1;
                        SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0,
                                                                              dt, true, false, 0, 0, width, offset, 0,
                                                                              NULL, NULL, NULL);
                        inputHead = returnInputOutputList(inputHead, newEntry);

                        offset += width;

                    }
            }

            id = id->compression_node;
            if (id != NULL)
                id = id->children->head;
        }



        // Traversing Output List
        id = outputList->children->head;
        SymbolTableEntry *outputHead = NULL;

        //Old Code
        // while (id != NULL)
//        {
//            Terminal type = id->nextNode->tokenIfLeaf->type;
//            DataType dt;
//            int width = 0;
//            if (type == INTEGER)
//            {
//                dt = integer;
//                width = 2;
//            }
//            else if (type == REAL)
//            {
//                dt = real;
//                width = 4;
//            }
//            else if (type == BOOLEAN)
//            {
//                dt = boolean;
//                width = 1;
//            }
//
//            // Primitive Data Type
//            if (dt != nulli)
//            {
//                SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, false, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
//                outputHead = returnInputOutputList(outputHead, newEntry);
//                offset += width;
//            }
//
//            // Array
//            else if (type == ARRAY)
//            {
//                AstNode *arr = id->nextNode;
//                AstNode *range = arr->children->head;
//                Terminal childType = range->nextNode->tokenIfLeaf->type;
//                Token *startingRange = range->children->head->tokenIfLeaf;
//                Token *endingRange = range->children->head->nextNode->tokenIfLeaf;
//                bool isStatic = 1;
//
//                if(startingRange->type == ID || endingRange->type==ID)
//                    isStatic = 0;
//                else if(startingRange->type == NUM && endingRange->type == NUM)
//                    isStatic = 1;
//                else{
//                    printf("ERROR in Line No. %d : Array Range Index Bound not in proper format\n");
//                }
//                if (isStatic)
//                {
//                    int sr = startingRange->val->num;
//                    int er = endingRange->val->num;
//
//                    // todo type checking of startRange and end Range
//                    width = 1;
//                    if (childType == INTEGER)
//                    {
//                        dt = integer;
//                        width += 2 * (er - sr + 1);
//                    }
//                    else if (childType == REAL)
//                    {
//                        dt = real;
//                        width += 4 * (er - sr + 1);
//                    }
//                    else if (childType == BOOLEAN)
//                    {
//                        dt = boolean;
//                        width += 1 * (er - sr + 1);
//                    }
//
//                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, true, sr, er, width, offset, 0, NULL, NULL, NULL);
//                    outputHead = returnInputOutputList(outputHead, newEntry);
//
//                    offset += width;
//                }
//                else
//                { // is Dynamic
//                    width = 1;
//
//                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
//                    outputHead = returnInputOutputList(outputHead, newEntry);
//
//                    offset += width;
//                }
//            }
//
//            id = id->compression_node;
//            if (id != NULL)
//                id = id->children->head;
//        }

        while (id != NULL)
        {
            DataType dt = nulli;
            int width = 0;
            bool isArray = false;
            if(id->nextNode->label == AST_ARRAY)
                isArray = true;
            if(!isArray){
                Terminal type = id->nextNode->tokenIfLeaf->type;

                if (type == INTEGER)
                {
                    dt = integer;
                    width = 2;
                }
                else if (type == REAL)
                {
                    dt = real;
                    width = 4;
                }
                else if (type == BOOLEAN)
                {
                    dt = boolean;
                    width = 1;
                }

                // Primitive Data Type
                if (dt != nulli)
                {
                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, false, false, 0, 0, width, offset, 0, NULL, NULL, NULL);
                    outputHead = returnInputOutputList(outputHead, newEntry);
                    offset += width;
                }
            }
            else
            {
                AstNode *child = a->children->tail; // pointing to array
                Terminal childType = child->children->tail->tokenIfLeaf->type;
                AstNode *range = child->children->head; // pointing to range
                Token *startingRange = range->children->head->children->head->tokenIfLeaf;
                Token *endingRange = range->children->tail->children->head->tokenIfLeaf;
                bool isStatic = 1;

                if(startingRange->type == ID || endingRange->type == ID){
                    isStatic = 0;
                }
                else if(startingRange->type == NUM && endingRange->type == NUM)
                    isStatic = 1;
                else{
                    int line=11;
                    printf("ERROR in Line No. %d : Array Range Index Bound not in proper format\n",line);
                    id = id->compression_node;
                    if (id != NULL)
                        id = id->children->head;
                    continue;
                }
                if (isStatic)
                {
                    int sr = startingRange->val->num;
                    int er = endingRange->val->num;

                    int width = 1;
                    DataType dt;
                    if (childType == INTEGER)
                    {
                        dt = integer;
                        width += 2 * (er - sr + 1);
                    }
                    else if (childType == REAL)
                    {
                        dt = real;
                        width += 4 * (er - sr + 1);
                    }
                    else if (childType == BOOLEAN)
                    {
                        dt = boolean;
                        width += 1 * (er - sr + 1);
                    }
                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0, dt, true, true, sr, er, width, offset, 0, NULL, NULL, NULL);
                    outputHead = returnInputOutputList(outputHead, newEntry);

                    offset+=width;
                }
                else { // is Dynamic
                    width = 1;
                    SymbolTableEntry *newEntry = populateSymbolTableEntry(id->tokenIfLeaf->lexeme, funcName, 0, 0,
                                                                          dt, true, false, 0, 0, width, offset, 0,
                                                                          NULL, NULL, NULL);
                    outputHead = returnInputOutputList(outputHead, newEntry);

                    offset += width;

                }
            }

            id = id->compression_node;
            if (id != NULL)
                id = id->children->head;
        }

        SymbolTable *symTable = initSt(1, funcName);
        FunctionSymbolTableEntry *fstEntry = populateFunctionSymbolTableEntry(
            funcName, inputHead, outputHead, symTable, NULL, NULL, NULL);
        addFunctionToFunctionSymbolTable(fst, fstEntry);
        GlobalSt = symTable;
    }
}

void processIterativeFor(AstNode *a, SymbolTable *st)
{
    if (a->label == AST_ITERATIVE_FOR)
    {
        AstNode *IDfor = a->children->head;
        char *lexeme = IDfor->tokenIfLeaf->lexeme;
        SymbolTableEntry *newEntry = populateSymbolTableEntry(lexeme, st->moduleName, 0, 0,
                                                              integer, false, false, 0, 0, 2, offset, 1 + st->nestingLevel, NULL, NULL, NULL);
        offset += 2;
        SymbolTable *newSt = initSt(1 + st->nestingLevel, st->moduleName);
        if (st->child == NULL)
        {
            st->child = newSt;
            newSt->parent = st;
            st->child->next = NULL;
        }
        else
        {
            SymbolTable *temp = st->child;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newSt;
            newSt->parent = st;
            temp = temp->next;
            temp->next = NULL;
        }
        addSymbolToSymbolTable(newSt, newEntry);
        GlobalSt = newSt;
    }
}

void processIterativeWhile(AstNode *a, SymbolTable *st)
{
    if (a->label == AST_ITERATIVE_WHILE)
    {
        SymbolTable *newSt = initSt(1 + st->nestingLevel, st->moduleName);
        if (st->child == NULL)
        {
            st->child = newSt;
            newSt->parent = st;
            st->child->next = NULL;
        }
        else
        {
            SymbolTable *temp = st->child;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newSt;
            newSt->parent = st;
            temp = temp->next;
            temp->next = NULL;
        }
        GlobalSt = newSt;
    }
}

void processDriverProgram(AstNode *a)
{
    char *funcName = "driver program";
    SymbolTable *symTable = initSt(1, funcName);
    FunctionSymbolTableEntry *fstEntry = populateFunctionSymbolTableEntry(
        funcName, NULL, NULL, symTable, NULL, NULL, NULL);
    addFunctionToFunctionSymbolTable(fst, fstEntry);
    GlobalSt = symTable;
}

void AstTraversal(AstNode *node)
{
    if (node == NULL)
        return;
    if (node->label == AST_DECLARE_STMT)
    {
        //printf("Processing Declare Stmt\n\n");
        processDeclareStmt(node, GlobalSt);
        //printf("Processing of Declare Stmt Complete\n\n");
        return;
    }
    else if (node->label == AST_ITERATIVE_FOR)
    {
        processIterativeFor(node, GlobalSt);
        AstTraversal(node->children->tail);
    }
    else if (node->label == AST_ITERATIVE_WHILE)
    {
        processIterativeWhile(node, GlobalSt);
        AstTraversal(node->children->tail);
    }
    else if (node->label == AST_MODULE)
    {
        //printf("processing Module Declaration Stmt\n\n");
        processModuleDeclaration(node);
        // printf("processing of Module declaration completed\n\n");
        AstTraversal(node->children->tail);
        AstTraversal(node->compression_node);
    }
    else if (node->label == AST_STMT_EXP && node->parent->label == AST_PROGRAM) //inside driver program
    {
        // printf("Processing Driver Program\n\n");
        processDriverProgram(node);
        // printf("Processing of Driver Program Complete\n\n");
        AstTraversal(node->children->head);
        if(node->children->sibling_count > 1)
            AstTraversal(node->children->tail);
    }
    else if(node->label == AST_ASSIGNMENT){
        // printf("Processing Assignment Stmt\n\n");
        // printf("Returning from assignment statement\n\n");
        return;
    }
    else if(node->label == AST_STMT_EXP){
        // printf("going into first stmt\n\n");

        AstTraversal(node->children->head);

        // printf("returning from first stmt and going to next statement\n\n");
        
        if(node->children->sibling_count > 1)
            AstTraversal(node->children->tail);

    }
    else if(node->label == AST_GET_VALUE){
        return;
    }
    else
    {
        //printf("\n\n%s\n\n",node->label);
        if(node->children == NULL || node->children->head==NULL || node->children->sibling_count==0)
            return;
        AstNode *childHead = node->children->head;
        while (childHead != NULL)
        {
            AstTraversal(childHead);
            childHead = childHead->nextNode;
        }
    }
    
}

// int main()
// {
//     printf("Hi");
// }

#endif