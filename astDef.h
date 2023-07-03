/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef ASTHEADH
#define ASTHEADH

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexerDef.h"

typedef enum
{
    AST_PROGRAM,
    AST_MODULE_DECLARATIONS,
    AST_OTHER_MODULE,
    AST_DRIVER_MODULE,
    AST_MODULE_ID,
    AST_OTHER_MODULE_LIST,
    AST_PARAMETER_LIST,
    AST_MODULE,
    AST_ARRAY,
    AST_ARRAY_RANGE,
    AST_GET_VALUE,
    AST_PRINT_VAR,
    AST_PRINT_INDEX,
    AST_ASSIGNMENT,
    AST_ASSIGN_ARR_STMT,
    AST_INDEX_ARR,
    AST_MODULE_REUSE,
    AST_PARAMETER_LIST_DASH,
    AST_K_RNUM,
    AST_K_BOOL_CONST,
    AST_K_N11,
    AST_U_73,
    AST_LOGICAL_OPERATOR,
    AST_RELATOINAL_OPERATOR,
    AST_PLUS_OR_MINUS,
    AST_MULT_OR_DIV,
    AST_FACTOR,
    AST_UNARY_PLUS_OR_MINUS,
    AST_DECLARE_STMT,
    AST_CONDITIONAL_SWITCH,
    AST_CASE_STMTS,
    AST_N9,
    AST_DEFAULT,
    AST_ITERATIVE_FOR,
    AST_ITERATIVE_WHILE,
    AST_RANGE_FOR_LOOP,
    AST_INDEX_FOR_LOOP,
    AST_N3,
    AST_N3_LIST,
    AST_INPUT_PLIST,
    AST_N1,
    AST_STMT_EXP
} AstNodeLabels;

struct abstractSynTreeChildren
{
    struct AbstractSynTree *head;
    struct AbstractSynTree *tail;
    int sibling_count;
};
typedef struct abstractSynTreeChildren astChildren;

struct AbstractSynTree
{
    AstNodeLabels label;
    int order_of_traversal_of_node; // kabhi nhi
    bool is_leaf;
    char *str;                           // can be used in further parts
    astChildren *new_children_generated; // kabhi nhi
    struct AbstractSynTree *parent;
    struct abstractSynTreeChildren *children;
    int *val; // can be used in further parts
    struct AbstractSynTree *nextNode;
    Token *tokenIfLeaf;
    struct AbstractSynTree *compression_node; // mostly stores the rules of the form of lists - input plist ya module list ya other form of
    int line;
    int scopeLineNumberStart;
    int scopeLineNumberEnd;
};
typedef struct AbstractSynTree AstNode;
struct ast_tree
{
    int order_of_traversal; // kabhi nhi
    AstNode *ast_tree_root_node;
    int number_of_nodes; // kabhi nhi
};
typedef struct ast_tree Ast_tree;

// RULE NUMBER - 60 PARAMETER LIST SHREYAS WAALI SAME HAI KYA JO 9 MEI LIKHI HAI
// RULE NO 101 - PLUS OR MINUS SAME
// abhi typelist lite lia hai, name, typle , dimensionn1 and 2, string length, parent function for recursion, tokentype, return parameters and current scope, symboltreenode

#endif