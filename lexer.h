/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

void swap_pointers(char *ptr1, char *ptr2);
void inc_func(int *ptr1, int val1, int *ptr2, int val2);
int is_alphabet(char ch);
int is_digit(char ch);
char *copy_string(char *dest, char *src);
void assign_token_value(Token *tk, Terminal type, char *lexeme, int valType, Value *value);
void end_encountered_no_error(FILE **fp, int *st, int jmp_to, char *reason, int *lexeme_ptr);
FILE *getStream(FILE *fp);
FILE *lexerInit(char *input_file);
Token *getNextToken(FILE **fp);
void removeComments(char *testcaseFile, char *cleanFile);
void getAllTokens(char *fp);
#endif