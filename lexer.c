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
#include <ctype.h>
#include <math.h>

#include "hashTable.h"
// do not run lookuptable.c together

#define SIZE_OF_BUFFER 64
#define BUFFER_SIZE 64

char *current_buffer;
char *previous_buffer;
unsigned int start;
int forward_ptr = 0;
int identifier_limit = 20; // maximum length of identifier allowed
int begin_ptr = 0;
unsigned int cur_line_no = 1; // initially at first line of file
unsigned int ERROR_STATE = 51;
unsigned int flag_file_end = 0;
unsigned int retract_flag = 0;

int numSlots = 43; // number of slots in hashTable
// number of slots in hash table are selected as 59 to minimise collisions (tested for all primes)
// 43 results in atmax 3 collisions in a field which can easily be lineary probed using the linked list struct we created

// we choose a large prime number for effective hashing
// We are using the rolling hash function for hashing
// hash[s] = (s[0] + s[1]*p + s[2]*p^2....s[n-1]*p^(n-1))%m
int p = 53; // used for the rolling hash function

hashTable *hash_table;

// int max(int a,int b)
// {
//     if(a>b)return a;
//     else return b;
// }

int hash(char *key) // function to calculate the hash value of a particular key
{

    int hashValue = 0;
    int power = 1;
    for (int i = 0; i < strlen(key); i++)
    {

        hashValue = (hashValue + (key[i]) * power) % numSlots;
        power = (power * p) % numSlots;
    }

    return hashValue;
}
hashTable *init()
{

    hashTable *hash_table = (hashTable *)malloc(sizeof(hashTable));
    hash_table->numSlots = numSlots;
    hash_table->slot = (lookUp **)malloc(numSlots * (sizeof(lookUp *)));

    for (int i = 0; i < numSlots; i++)
    {
        hash_table->slot[i] = (lookUp *)malloc(sizeof(lookUp));
        hash_table->slot[i]->lexeme = "\0";
        hash_table->slot[i]->type = 0;
        hash_table->slot[i]->next = NULL;
        hash_table->slot[i]->head = hash_table->slot[i];
        hash_table->slot[i]->tail = hash_table->slot[i]->head;
    }

    return hash_table;
}

hashTable *insert(char *lexeme, Terminal type, hashTable *hash_table)
{

    int hashValue = hash(lexeme);
    lookUp *tail = hash_table->slot[hashValue]->tail;
    tail->next = (lookUp *)malloc(sizeof(lookUp));
    tail = tail->next;
    tail->next = NULL;
    tail->lexeme = lexeme;
    tail->type = type;
    hash_table->slot[hashValue]->tail = tail;
    return hash_table;
}
lookUp *search(hashTable *hash_table, char *lexeme)
{

    int hashValue = hash(lexeme);

    lookUp *temp = hash_table->slot[hashValue]->head;
    int count = 0;
    while (temp)
    {
        count++;

        if (strcmp(lexeme, temp->lexeme) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
hashTable *populateTable()
{
    hashTable *hash_table = init();
    hash_table = insert("integer", INTEGER, hash_table);
    hash_table = insert("real", REAL, hash_table);
    hash_table = insert("boolean", BOOLEAN, hash_table);
    hash_table = insert("of", OF, hash_table);
    hash_table = insert("array", ARRAY, hash_table);
    hash_table = insert("start", START, hash_table);
    hash_table = insert("end", END, hash_table);
    hash_table = insert("declare", DECLARE, hash_table);
    hash_table = insert("module", MODULE, hash_table);
    hash_table = insert("driver", DRIVER, hash_table);
    hash_table = insert("program", PROGRAM, hash_table);
    hash_table = insert("get_value", GET_VALUE, hash_table);
    hash_table = insert("print", PRINT, hash_table);
    hash_table = insert("use", USE, hash_table);
    hash_table = insert("with", WITH, hash_table);
    hash_table = insert("parameters", PARAMETERS, hash_table);
    hash_table = insert("takes", TAKES, hash_table);
    hash_table = insert("input", INPUT, hash_table);
    hash_table = insert("returns", RETURNS, hash_table);
    hash_table = insert("for", FOR, hash_table);
    hash_table = insert("in", IN, hash_table);
    hash_table = insert("switch", SWITCH, hash_table);
    hash_table = insert("case", CASE, hash_table);
    hash_table = insert("break", BREAK, hash_table);
    hash_table = insert("default", DEFAULT, hash_table);
    hash_table = insert("while", WHILE, hash_table);
    hash_table = insert("AND", AND, hash_table);
    hash_table = insert("true", TRUE, hash_table);
    hash_table = insert("false", FALSE, hash_table);
    hash_table = insert("OR", OR, hash_table);
    return hash_table;
}

void swap_pointers(char *ptr1, char *ptr2)
{
    char *buf = ptr1;
    ptr1 = ptr2;
    ptr2 = buf;
}

void inc_func(int *ptr1, int val1, int *ptr2, int val2)
{
    if (ptr1 == NULL || ptr2 == NULL)
        return;
    (*ptr1) += val1;
    (*ptr2) += val2;
}

int is_alphabet(char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return 1;
    if (ch >= 'A' && ch <= 'Z')
        return 1;
    return 0;
}

int is_digit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return 1;
    return 0;
}

char *copy_string(char *dest, char *src)
{
    // return if no memory is allocated to the destination
    if (dest == NULL)
        return NULL;
    char *destLexeme = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';

    return destLexeme;
}

void assign_token_value(Token *tk, Terminal type, char *lexeme, int valType, Value *value)
{
    tk->lexeme = lexeme;
    tk->line_no = cur_line_no;
    tk->type_of_value = valType;
    tk->type = type;
    tk->val = value;
}

void end_encountered_no_error(FILE **fp, int *st, int jmp_to, char *reason, int *lexeme_ptr)
{
    forward_ptr = 0;
    *fp = getStream(*fp);
    if (*fp == NULL)
    {
        // the file has ended. jump to a state and process the current lexeme
        flag_file_end = 1;
        *st = jmp_to; // process this lexeme
        // fprintf(stderr, "Lexical error, line - %d, reason - %s\n", cur_line_no,reason);
        inc_func(&forward_ptr, 1, lexeme_ptr, 0);
    }
}

FILE *getStream(FILE *fp)
{
    swap_pointers(previous_buffer, current_buffer); // the current buffer would become the prevous one
    memset(current_buffer, 0, sizeof(current_buffer));
    for (int i = 0; i < SIZE_OF_BUFFER; i++)
        current_buffer[i] = '\0';

    int val;
    // if (forward_ptr == SIZE_OF_BUFFER)
    // {
    //     forward_ptr = 0;
    //     // read one round -
    // }

    if ((val = fread(current_buffer, sizeof(char), SIZE_OF_BUFFER, fp)) > 0)
    {
        current_buffer[SIZE_OF_BUFFER] = '\0';
        return fp;
    }
    else
    {
        fclose(fp);
        return NULL;
    }
}

FILE *lexerInit(char *input_file)
{
    forward_ptr = 0;
    cur_line_no = 1;
    flag_file_end = 0;
    current_buffer = (char *)malloc(SIZE_OF_BUFFER * sizeof(char));
    previous_buffer = (char *)malloc(SIZE_OF_BUFFER * sizeof(char));
    memset(current_buffer, '\0', sizeof(current_buffer));
    memset(previous_buffer, '\0', sizeof(previous_buffer));
    hash_table = populateTable();

    FILE *fp = fopen(input_file, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error Opening File\n");
        return NULL;
    }

    else
    {
        printf("\nFILE OPENED SUCCESSFULLY.\n\n");
    }

    return fp;
}

Token *getNextToken(FILE **fp)
{ // function reads the lexemes and returns all relevant information for the parser

    Token *token = (Token *)malloc(sizeof(Token));
    if (current_buffer[forward_ptr] == '\0' || current_buffer[forward_ptr] == (-1))
    {
        forward_ptr = 0;
        *fp = getStream(*fp);
        if (*fp == NULL)
        {
            flag_file_end = 1;
            printf("\nRead till end of file\n\n");
            return NULL;
        }
    }

    int state = 0; // the start state is considered as 0 in our DFA
    if (retract_flag == 1)
    {
        retract_flag = 0;
        state = 49;
    }
    char *lexeme = (char *)malloc(100 * sizeof(char));
    int lexeme_ptr = 0, flag_final_state = 0;

    while (1)
    {
        // printf("Entering Code44 %s\n",lexeme);

        if (flag_final_state)
            break;
        char ch = current_buffer[forward_ptr];
        switch (state)
        {

        case 0:
        { // start state

            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                if (*fp == NULL)
                {
                    printf("\nRead till end of file\n\n");
                    return NULL;
                }
            }
            else if (ch == '\r' || ch == '\t' || ch == ' ')
            { // skip whitespace characters
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 13;
            }
            else if (ch == '\n')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 50;
            }
            else if (ch == '+')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 15;
            }
            else if (ch == '-')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 16;
            }
            else if (ch == '*')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 17;
            }

            else if (ch == '/')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 22;
            }
            else if (ch == '=')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 23;
            }
            else if (ch == ':')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 25;
            }
            else if (ch == '!')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 28;
            }
            else if (ch == ';')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 30;
            }
            else if (ch == ',')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 31;
            }
            else if (ch == '[')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 32;
            }
            else if (ch == ']')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 33;
            }
            else if (ch == '<')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 34;
            }
            else if (ch == '>')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 40;
            }
            else if (ch == '(')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 46;
            }
            else if (ch == ')')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 47;
            }
            else if (ch == '.')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 48;
            }
            else if (is_alphabet(ch) || ch == '_')
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 1;
            }
            else if (is_digit(ch))
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 3;
            }
        }
        break;
        case 1:
        {
            if (is_digit(ch) || is_alphabet(ch) || ch == '_')
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 1;
            }
            else if (current_buffer[forward_ptr] == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 1; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 2; // chk
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else
            { // read something else - do not increment
                state = 2;
            }
        }
        break;

        case 2:
        {
            lexeme[lexeme_ptr] = '\0';
            int length = strlen(lexeme);
            if (length > identifier_limit)
            {
                printf("\nLexical error occured, Line %d, - The length of lexeme %s is higher than the maximum length allowed!\n", cur_line_no, lexeme);
                state = ERROR_STATE;
            }
            else
            {
                // lookup table wali bt handle krni
                lookUp *tken = search(hash_table, lexeme);
                flag_final_state = 1;
                if (tken == NULL)
                    token->type = ID;
                else
                    token->type = tken->type;
                assign_token_value(token, token->type, lexeme, 2, NULL);
            }
        }
        break;
        case 3:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 3; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 4; // process this lexeme
                    // fprintf(stderr,"Lexical error, line - %d, two '**' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (is_digit(ch))
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 3; // stay in the same state
            }
            else if (ch == '.')
            {
                // process this dot and look for the next character
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 5;
            }
            else
            {
                // other character - do not increment the forward pointer
                state = 4;
            }
        }
        break;
        case 4:
        {
            flag_final_state = 1;
            lexeme[lexeme_ptr] = '\0';
            int length = strlen(lexeme);
            int number = atoi(lexeme);
            token->val = (Value *)malloc(sizeof(Value));
            token->val->num = number;
            assign_token_value(token, NUM, lexeme, 0, token->val);
        }
        break;
        case 5:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 5; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme
                    fprintf(stderr, "Lexical error, line - %d, '.' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '.')
            {
                // retract once to get to range op one
                state = 4; // need to return the number token so same as state 4.
                inc_func(&forward_ptr, 1, &lexeme_ptr, -1);
                retract_flag = 1;
                lexeme[lexeme_ptr] = '\0'; // remove the . from the lexeme
            }
            else if (is_digit(ch))
            {
                state = 7;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
            }
            else
            {
                state = ERROR_STATE;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                fprintf(stderr, "\nLexical error, line - %d, %s is invalid form of lexeme\n", cur_line_no, lexeme);
                inc_func(&forward_ptr, 0, &lexeme_ptr, 0); // do not move forward - process the file from here itself
            }
        }
        break;
        case 6:
        {
            // same as state 4 - we would not reach this state in any scenario
        }
        break;
        case 7:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 7; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 8; // process this lexeme as a real number - no error
                    // fprintf(stderr,"Lexical error, line - %d, two '**' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (is_digit(ch))
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 7;
            }
            else if (ch == 'E' || ch == 'e')
            {
                state = 9;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
            }
            else
            {
                // return the real number as a token
                state = 8; // do not increment the forward pointer
            }
        }
        break;
        case 8:
        {
            lexeme[lexeme_ptr] = '\0';
            float number = atof(lexeme);
            flag_final_state = 1;
            token->val = (Value *)malloc(sizeof(Value));
            token->val->rnum = number;
            assign_token_value(token, RNUM, lexeme, 1, token->val);
        }
        break;
        case 9:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 9; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme as a real number - no error
                    lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                    fprintf(stderr, "Lexical error, line - %d, %s is not a valid string\n", cur_line_no, lexeme);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0); // do not increment because need to process from here itself
                }
            }
            else if (ch == '+' || ch == '-')
            {
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
                state = 10;
            }
            else if (is_digit(ch))
            {
                state = 11;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
            }
            else
            {
                // error
                state = ERROR_STATE;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                fprintf(stderr, "\nLexical error, line - %d, %s is not a valid string\n", cur_line_no, lexeme);
            }
        }
        break;
        case 10:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 10; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme as a real number - no error
                    lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                    fprintf(stderr, "Lexical error, line - %d, %s is not a valid string at end of file\n", cur_line_no, lexeme);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0); // do not increment because need to process from here itself
                }
            }
            else if (is_digit(ch))
            {
                state = 11;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
            }
            else
            {
                state = ERROR_STATE;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                fprintf(stderr, "\nLexical error, line - %d, %s is not a valid string\n", cur_line_no, lexeme);
            }
        }
        break;
        case 11:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 11; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 12; // process this lexeme as a number - no error
                    // lexeme[lexeme_ptr]=current_buffer[forward_ptr];
                    // fprintf(stderr,"Lexical error, line - %d, %s is not a valid string at end of file\n",cur_line_no,lexeme);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0); // do not increment because need to process from here itself
                }
            }
            else if (is_digit(ch))
            {
                state = 11;
                lexeme[lexeme_ptr] = current_buffer[forward_ptr];
                inc_func(&forward_ptr, 1, &lexeme_ptr, 1);
            }
            else
            {
                state = 12; // do not copy the current character and just process this lexeme
            }
        }
        break;
        case 12:
        {
            lexeme[lexeme_ptr] = '\0';
            float number = atof(lexeme);
            flag_final_state = 1;
            token->val = (Value *)malloc(sizeof(Value));
            token->val->rnum = number;
            assign_token_value(token, RNUM, lexeme, 1, token->val);
        }
        break;
        case 13:
        {
            state = 0; // skip the delimiter character and go to the base state
            // there is no state 14 required
        }
        break;
        case 15:
        {
            copy_string(lexeme, "+");
            assign_token_value(token, PLUS, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 16:
        {
            copy_string(lexeme, "-");
            assign_token_value(token, MINUS, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 17:
        {
            if (current_buffer[forward_ptr] == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 17; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 18; // process this lexeme
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '*')
            {
                // handle comments
                state = 19;
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
            else
            {
                // read some other character - move to state 18 and return the current mult token
                state = 18;
            }
        }
        break;
        case 18:
        {
            copy_string(lexeme, "*");
            assign_token_value(token, MUL, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 19:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 19; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme
                    fprintf(stderr, "Lexical error, line - %d, two '**' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '*')
            {
                state = 20;
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
            else
            {
                if (ch == '\n')
                    cur_line_no++;
                state = 19; // stay in current state
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
        }
        break;
        case 20:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 20; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme
                    fprintf(stderr, "Lexical error, line - %d, two '**..*' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '*')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 21;
            }
            else
            {
                if (ch == '\n')
                    cur_line_no++;
                state = 19; // comment does not end now
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
        }
        break;
        case 21:
        {
            // copy_string(lexeme,"**..**");
            // assign_token_value(token,COMMENTMARK,lexeme,2,NULL);
            // flag_final_state=1;
            state = 0;
        }
        break;
        case 22:
        {
            copy_string(lexeme, "/");
            assign_token_value(token, DIV, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 23:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 23; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme
                    fprintf(stderr, "Lexical error, line - %d, '=' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '=')
            {
                state = 24;
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
            else
            {
                state = ERROR_STATE;
                fprintf(stderr, "Lexical error, line - %d, '=' followed by others \n", cur_line_no);
            }
        }
        break;
        case 24:
        {
            copy_string(lexeme, "==");
            assign_token_value(token, EQ, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 25:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 25; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 27; // process this lexeme
                    // fprintf(stderr,"Lexical error, line - %d, ':' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '=')
            {
                state = 26;
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
            else
            {
                state = 27;
            }
        }
        break;
        case 26:
        {
            copy_string(lexeme, ":=");
            assign_token_value(token, ASSIGNOP, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 27:
        {
            copy_string(lexeme, ":");
            assign_token_value(token, COLON, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 28:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 28; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme
                    fprintf(stderr, "Lexical error, line - %d, '!' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '=')
            {
                state = 29;
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
            }
            else
            {
                // report lexical error
                state = ERROR_STATE;
                fprintf(stderr, "Lexical error, line - %d, '!' alone is invalid\n", cur_line_no);
            }
        }
        break;
        case 29:
        {
            copy_string(lexeme, "!=");
            assign_token_value(token, NE, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 30:
        {
            copy_string(lexeme, ";");
            assign_token_value(token, SEMICOL, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 31:
        {
            copy_string(lexeme, ",");
            assign_token_value(token, COMMA, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 32:
        {
            copy_string(lexeme, "[");
            assign_token_value(token, SQBO, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 33:
        {
            copy_string(lexeme, "]");
            assign_token_value(token, SQBC, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 34:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 34; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 37; // process this lexeme - less than
                    // fprintf(stderr,"Lexical error, line - %d, '<' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '<')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 35;
            }
            else if (ch == '=')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 36;
            }
            else
            {
                state = 37;
                // don't increment the forward pointer as it is pointing at the right spot
            }
        }
        break;
        case 35:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 35; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 39; // process this lexeme - less than
                    // fprintf(stderr,"Lexical error, line - %d, '<' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '<')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 38;
            }
            else
            {
                // don't increment for ptr
                state = 39;
            }
        }
        break;
        case 36:
        {
            copy_string(lexeme, "<=");
            assign_token_value(token, LE, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 37:
        {
            copy_string(lexeme, "<");
            assign_token_value(token, LT, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 38:
        {
            copy_string(lexeme, "<<<");
            assign_token_value(token, DRIVERDEF, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 39:
        {
            copy_string(lexeme, "<<");
            assign_token_value(token, DEF, lexeme, 2, NULL);
            flag_final_state = 1;
        }
        break;
        case 40:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 40; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 43; // process this lexeme - greater than
                    // fprintf(stderr,"Lexical error, line - %d, '<' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '>')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 41;
            }
            else if (ch == '=')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 42;
            }
            else
            {
                // do not increment as already reading other
                state = 43;
            }
        }
        break;
        case 41:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 41; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = 45; // process this lexeme - greater than
                    // fprintf(stderr,"Lexical error, line - %d, '<' at end of file\n",cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '>')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 44;
            }
            else
            {
                // do not increment - already reading others
                state = 45;
            }
        }
        break;
        case 42:
        {
            flag_final_state = 1;
            copy_string(lexeme, ">=");
            assign_token_value(token, GE, lexeme, 2, NULL);
        }
        break;
        case 43:
        {
            flag_final_state = 1;
            copy_string(lexeme, ">");
            assign_token_value(token, GT, lexeme, 2, NULL);
        }
        break;
        case 44:
        {
            flag_final_state = 1;
            copy_string(lexeme, ">>>");
            assign_token_value(token, DRIVERENDDEF, lexeme, 2, NULL);
        }
        break;
        case 45:
        {
            flag_final_state = 1;
            copy_string(lexeme, ">>");
            assign_token_value(token, ENDDEF, lexeme, 2, NULL);
        }
        break;
        case 46:
        {
            flag_final_state = 1;
            copy_string(lexeme, "(");
            assign_token_value(token, BO, lexeme, 2, NULL);
        }
        break;
        case 47:
        {
            flag_final_state = 1;
            copy_string(lexeme, ")");
            assign_token_value(token, BC, lexeme, 2, NULL);
        }
        break;
        case 48:
        {
            if (ch == '\0' || current_buffer[forward_ptr] == (-1))
            {
                forward_ptr = 0;
                *fp = getStream(*fp);
                state = 48; // stay in the same state unless it is end of file
                if (*fp == NULL)
                {
                    // the file has ended. jump to a state and process the current lexeme
                    flag_file_end = 1;
                    state = ERROR_STATE; // process this lexeme - greater than
                    fprintf(stderr, "Lexical error, line - %d, '.' at end of file\n", cur_line_no);
                    inc_func(&forward_ptr, 0, &lexeme_ptr, 0);
                }
            }
            else if (ch == '.')
            {
                inc_func(&forward_ptr, 1, &lexeme_ptr, 0);
                state = 49;
            }
            else
            {
                // lexical error - do not incremetn as you have to process from here
                state = ERROR_STATE;
                fprintf(stderr, "Lexical error, line - %d, '.' alone not allowed\n", cur_line_no);
            }
        }
        break;
        case 49:
        {
            flag_final_state = 1;
            copy_string(lexeme, "..");
            assign_token_value(token, RANGEOP, lexeme, 2, NULL);
        }
        break;
        case 50:
        {
            cur_line_no++;
            state = 0;
        }
        case 51:
        {
            // this is error state - read from the current forward_ptr only - do not increment.
            // flag_final_state=1;
            // lexeme[lexeme_ptr]='\0';
            // assign_token_value(token,ERROR,lexeme,-1,NULL);
            for (int i = 0; i < 100; i++)
                lexeme[i] = '\0';
            lexeme_ptr = 0;
            state = 0;
        }
        break;
        }
    }
    return token;
}
// removing comments
void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE *fptrTestFile = fopen(testcaseFile, "r");
    // FILE *fptrCleanFile = fopen(cleanFile, "w+");
    current_buffer = (char *)malloc(SIZE_OF_BUFFER * sizeof(char));
    previous_buffer = (char *)malloc(SIZE_OF_BUFFER * sizeof(char));
    memset(current_buffer, '\0', sizeof(current_buffer));
    memset(previous_buffer, '\0', sizeof(previous_buffer));
    if (fptrTestFile == NULL)
    {
        fprintf(stderr, "Cannot open the Test Case File\n");
        return;
    }
    // if(cleanFile==NULL)
    // {
    //     fprintf(stderr,"Cannot open the Clean File\n");
    //     return;
    // }
    int line_change = 0;
    fptrTestFile = getStream(fptrTestFile);
    cur_line_no = 1;
    forward_ptr = 0;
    flag_file_end = 0;
    int asteriskCount = 0;
    int isComment = 0; // open=1 represents start of the token
    while (1)
    {
        if (current_buffer[forward_ptr] == '\0' || current_buffer[forward_ptr] == (-1))
        {
            fptrTestFile = getStream(fptrTestFile);
            if (fptrTestFile == NULL)
            {
                break;
            }
            forward_ptr = 0;
        }

        if (asteriskCount == 0)
        {
            if (current_buffer[forward_ptr] != '*')
            {
                // fputc(current_buffer[forward_ptr],fptrCleanFile); // uncommenting this would print the data in clean file
                fputc(current_buffer[forward_ptr], stdout);
            }
            else
            {
                asteriskCount = 1;
            }
            inc_func(&forward_ptr, 1, &begin_ptr, 0);
        }
        else if (asteriskCount == 1)
        {
            if (isComment)
            {
                // comment might close here
                if (current_buffer[forward_ptr] == '*')
                {
                    isComment = 0; // not a comment anymore
                    asteriskCount = 0;
                }
                else
                {
                    // was a part of comment
                    asteriskCount = 2;
                }
                inc_func(&forward_ptr, 1, &begin_ptr, 0);
            }
            else
            {
                if (current_buffer[forward_ptr] == '*') // comment will begin here
                {
                    isComment = 1;
                    asteriskCount = 2;
                }
                else
                { // it wasnt a comment
                    // fputc('*',fptrCleanFile);
                    fputc(current_buffer[forward_ptr], stdout);
                    asteriskCount = 0;
                }
                inc_func(&forward_ptr, 1, &begin_ptr, 0);
            }
        }
        else if (asteriskCount == 2) // comment has begun
        {
            if (current_buffer[forward_ptr] == '\n')
                fputc(current_buffer[forward_ptr], stdout);
            else if (current_buffer[forward_ptr] == '*')
            {
                asteriskCount = 1;
            }
            else
            {
                // simply ignore
            }
            inc_func(&forward_ptr, 1, &begin_ptr, 0);
        }
    }
    // fclose(fptrCleanFile);
}
// for testing the working of lexer
void getAllTokens(char *fp)
{
    current_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    memset(current_buffer, 0, sizeof(current_buffer));
    previous_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    FILE *fptr = fopen(fp, "r");
    memset(previous_buffer, 0, sizeof(previous_buffer));
    hash_table = populateTable();
    if (fptr == NULL)
    {
        fprintf(stderr, "Cannot open the given file contiaining source code\n");
        return;
    }
    // printf("Entering Code*2\n");

    cur_line_no = 1, flag_file_end = 0, forward_ptr = 0;
    printf("\n Given input file succesfully open.\n The tokens are - \n\n");
    Token *token = getNextToken(&fptr);
    // printf("Entering Code*3\n");

    while (token != NULL)
    {
        printf("Line no - %d. Lexeme - %s. Token type - %s\n", token->line_no, token->lexeme, tokenName[token->type]);
        // printf("Entering Code*66\n");
        if (fptr != NULL)
            token = getNextToken(&fptr);
        else
            break;
        // printf("Entering Code*77\n");
    }
    if (fptr != NULL)
        fclose(fptr);
    printf("\n");
    return;
}

// int main(){
//     char* input_file_name="input_code.txt";
//     //printf("\n%s\n",input_file_name);
//     //char* clean_file="clean_file.txt";
//     //removeComments(input_file_name,clean_file);
//     printf("Entering Code!\n");
//     getAllTokens(input_file_name);

//     return 0;
// }
