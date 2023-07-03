/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/
#ifndef OURSTACKDEF
#define OURSTACKDEF
#include <stdlib.h>
#include "tree.h"

typedef struct StackNode
{
    TreeNode *treeNodePointer;
    struct StackNode *next;
} StackNode;

typedef struct Stack
{
    StackNode *top;
    int size;
} Stack;

#endif