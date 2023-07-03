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

#include "stack.h"

StackNode *createStackNode(TreeNode *treeNodePointer)
{

    StackNode *stackNode = (StackNode *)malloc(sizeof(StackNode)); // allocating memory to stack
    stackNode->next = NULL;
    stackNode->treeNodePointer = treeNodePointer;
    return stackNode;
}

void pushToStack(Stack *s, TreeNode *treeNodePointer)
{

    StackNode *newStackNode = createStackNode(treeNodePointer);
    s->size++; // increase the size of the stack
    newStackNode->next = s->top;
    s->top = newStackNode; // changing the top of the stack

    return;
}

StackNode *popFromStack(Stack *s)
{

    if (s->size != 0)
    {
        s->size--;                // reduce the size of the stack
        StackNode *temp = s->top; // changing the top of the stack
        s->top = s->top->next;

        return temp;
    }

    else
    {

        return NULL;
    }
}

void addToStack(Stack *s, TreeNode *treeNode)
{

    TreeNode *child = treeNode->childHead;

    // printf("\n\n");
    //             // printf("%d\n", rule->LHS);
    //             TreeNode* cur = treeNode->childHead;
    //             while (cur != NULL) {
    //                 if (cur->symbol->is_terminal) {
    //                     printf("T %d   ", cur->symbol->t);
    //                 }
    //                 else {
    //                     printf("NT %d   ", cur->symbol->nt);
    //                 }
    //                 cur = cur->next;
    //             }
    //             printf("\n\n");

    // // If the rule is NT--> epsilon, we dont need to push anything on the stack
    if (child->symbol->is_terminal && child->symbol->t == EPSILON)
        return;

    // We need to reverse the child before inserting in s
    // Reversal of treeNodes can be done via another stack s2
    Stack *s2 = (Stack *)malloc(sizeof(Stack));
    s2->top = NULL;
    s2->size = 0;

    while (child != NULL)
    {
        pushToStack(s2, child);
        child = child->next;
    }

    // printf("Size of s2 - %d", s2->size);
    // print_stack(s2);

    while (s2->size != 0)
    {
        StackNode *stackNode = popFromStack(s2);
        TreeNode *newNode = stackNode->treeNodePointer;
        pushToStack(s, newNode);
    }
    return;
}
