/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef LEXER_DEF
#define LEXER_DEF

#include <stdio.h>
#define BUFFER_SIZE 64

typedef enum terminal
{
    EPSILON,
    AND,
    ARRAY,
    ASSIGNOP,
    BC,
    BO,
    BOOLEAN,
    BREAK,
    CASE,
    COLON,
    COMMA,
    DECLARE,
    DEF,
    DEFAULT,
    DIV,
    DRIVER,
    DRIVERDEF,
    DRIVERENDDEF,
    END,
    ENDDEF,
    EQ,
    FALSE,
    FOR,
    GE,
    GET_VALUE,
    GT,
    ID,
    IN,
    INPUT,
    INTEGER,
    LE,
    LT,
    MINUS,
    MODULE,
    MUL,
    NE,
    NUM,
    OF,
    OR,
    PARAMETERS,
    PLUS,
    PRINT,
    PROGRAM,
    RANGEOP,
    REAL,
    RETURNS,
    RNUM,
    SEMICOL,
    SQBC,
    SQBO,
    START,
    SWITCH,
    TAKES,
    TRUE,
    USE,
    WHILE,
    WITH
} Terminal;

typedef union
{
    int num;
    float rnum;
} Value;

typedef struct token
{
    Terminal type;        // stores the types of the tokens
    unsigned int line_no; // stores the line number of the token
    char *lexeme;         // stores the actual lexeme
    Value *val;           // to store integers and floating point numbers
    int type_of_value;    // assign ints - 0, assign reals - 1, assign others -2 , assign -1 to errors
} Token;

#endif