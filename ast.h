#ifndef ASTH
#define ASTH

/*GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#include "astDef.h"
#include "parserDef.h"

Ast_tree *createAstfunction(ParseTree *Parsetree);

void abstract_syn_tree_printer(Ast_tree* ast);

void helper_print_ast(AstNode* node_ast);

void ast_nodeprint(AstNode* cur_node);

Ast_tree *ast_init();
// void add_ast_root(AST* tree, ASTNode* root); - not needed remove this comment
AstNode* create_ast_node(AstNodeLabels astlabel, AstNode* parent_node, AstNode* next_node, astChildren* chldren,int is_leaf, Token* token); // modify the parameters

AstNode *create_ast_leaf(Token *token);

void update_current_node_parent(astChildren *current_children, AstNode *current_parent);
void update_ast_node_children(AstNode *node_to_add, astChildren *current_child);
void helper_inorder(TreeNode *cur_node, TreeNode *last_node);
void helper_hundred(TreeNode *cur_node, TreeNode *last_node);

void createAST_helper(TreeNode *node);
void analyze_node(TreeNode *node);

void testfunc(ParseTree* ptree);

#endif

