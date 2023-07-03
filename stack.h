/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/
#ifndef OURSTACK
#define OURSTACK
#include "stackDef.h"

StackNode *createStackNode(TreeNode *treeNodePointer);

void pushToStack(Stack *s, TreeNode *treeNodePointer);

StackNode *popFromStack(Stack *s);

void addToStack(Stack *s, TreeNode *treeNode);

#endif