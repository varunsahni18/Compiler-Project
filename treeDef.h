/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/
#ifndef OURTREEDEF
#define OURTREEDEF

#include <stdio.h>
#include <stdlib.h>
#include "grammar_symbol.h"
#include "astDef.h"

typedef struct TreeNode
{
    Symbol *symbol;
    Token *token; // if leaf node
    struct TreeNode *next;
    struct TreeNode *parent;
    struct TreeNode *childHead;
    int noOfChildren;

    // ast requirements
    AstNode* addr;
    AstNode* inh_addr;
    AstNode* syn_addr;
    int current_rule_number;

    int child_rule_number; // kabhi
    int parent_rule_number; // kabhi
} TreeNode;

typedef struct ParseTree
{
    TreeNode *root;
    int parse_tree_node_cnt;
} ParseTree;

#endif