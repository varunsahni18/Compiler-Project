/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef HASHTABLE
#define HASHTABLE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTableDef.h"

// contains function declarations
hashTable *init();
void addEntry(char *lexeme, Terminal type);
hashTable *insert(char *lexeme, Terminal type, hashTable *hash_table);
lookUp *search(hashTable *hash_table, char *lexeme);
hashTable *populateTable();

int hash(char *key);
#endif
