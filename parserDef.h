/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef OURPARSER
#define OURPARSER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include "lexer.h"
#include "stack.h"

#define MAX_SYMBOL_LENGTH 50

// 0. Data Structures & Definitions
#define NO_OF_RULES 141
#define NO_OF_NON_TERMINALS 72
#define NO_OF_TERMINALS 57

// 0. Data Structures & Definitions

typedef struct FirstAndFollow
{
    int **first_set;
    int **follow_set;
} FirstAndFollow;

typedef struct ParsingTable
{
    Rule *cells[NO_OF_NON_TERMINALS][NO_OF_TERMINALS];
} ParsingTable;

// Rule *parseTable[NO_OF_NON_TERMINALS][NO_OF_TERMINALS];

#endif
