/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef LOOKUP_TABLE_DEF
#define LOOKUP_TABLE_DEF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

char *tokenName[] = {
    "EPSILON",
    "AND",
    "ARRAY",
    "ASSIGNOP",
    "BC",
    "BO",
    "BOOLEAN",
    "BREAK",
    "CASE",
    "COLON",
    "COMMA",
    "DECLARE",
    "DEF",
    "DEFAULT",
    "DIV",
    "DRIVER",
    "DRIVERDEF",
    "DRIVERENDDEF",
    "END",
    "ENDDEF",
    "EQ",
    "FALSE",
    "FOR",
    "GE",
    "GET_VALUE",
    "GT",
    "ID",
    "IN",
    "INPUT",
    "INTEGER",
    "LE",
    "LT",
    "MINUS",
    "MODULE",
    "MUL",
    "NE",
    "NUM",
    "OF",
    "OR",
    "PARAMETERS",
    "PLUS",
    "PRINT",
    "PROGRAM",
    "RANGEOP",
    "REAL",
    "RETURNS",
    "RNUM",
    "SEMICOL",
    "SQBC",
    "SQBO",
    "START",
    "SWITCH",
    "TAKES",
    "TRUE",
    "USE",
    "WHILE",
    "WITH"};
typedef struct Node
{
    struct Node *next;
    struct Node *head;
    struct Node *tail;
    char *lexeme;
    Terminal type;

} lookUp;

typedef struct hashTable
{
    lookUp **slot;
    int numSlots;
} hashTable;

#endif