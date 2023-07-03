/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/
#ifndef OURTREE
#define OURTREE

#include "treeDef.h"
#include <stdio.h>

TreeNode *createTreeNode(int enumval, bool isterminal, TreeNode *parent);
void addChildToTree(TreeNode *treeNode, Rule *rule);

#endif