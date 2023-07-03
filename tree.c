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

#include "tree.h"

TreeNode *createTreeNode(int enumval, bool isterminal, TreeNode *parent)
{

    TreeNode *treeNode = (TreeNode *)malloc(sizeof(TreeNode));
    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    treeNode->current_rule_number = 0;
    sym->is_terminal = isterminal;
    if (isterminal)
        sym->t = enumval;
    else
        sym->nt = enumval;

    treeNode->inh_addr = NULL;
    treeNode->symbol = sym;
    treeNode->token = NULL;
    treeNode->next = NULL;
    treeNode->addr = NULL;
    treeNode->parent = parent;
    treeNode->childHead = NULL;
    treeNode->noOfChildren = 0;

    if (parent->childHead == NULL)
        parent->childHead = treeNode;

    parent->noOfChildren++;

    return treeNode;
}

void addChildToTree(TreeNode *treeNode, Rule *rule)
{

    SymbolNode *symbolNode = rule->head;
    TreeNode *temp = treeNode->childHead;
    treeNode->current_rule_number = rule->ruleNo + 1;
    for (int i = 0; i < rule->length; i++)
    {
        int enumval;
        if (symbolNode->symbol->is_terminal){
            enumval = symbolNode->symbol->t;

        }
        else
            enumval = symbolNode->symbol->nt;

        TreeNode *childNode = createTreeNode(enumval, symbolNode->symbol->is_terminal, treeNode);
        //childNode->current_rule_number = rule->ruleNo + 1;

        if (temp == NULL)
        {
            temp = childNode;
        }
        else
        {
            temp->next = childNode;
            temp = childNode;
        }

        if (symbolNode->next != NULL)
            symbolNode = symbolNode->next;
    }
}
