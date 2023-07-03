/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#include <stdio.h>
#include "ast.h"

Ast_tree *ast_global;
int ast_node_cnt=0;

char *T1_HashMap[] = {
    "EPSILON",
    "AND",
    "ARRAY",
    "ASSIGNOP",
    "BC",
    "BO",
    "BOOLEAN",
    "BREAK",
    "CASE",
    "COLON",
    "COMMA",
    "DECLARE",
    "DEF",
    "DEFAULT",
    "DIV",
    "DRIVER",
    "DRIVERDEF",
    "DRIVERENDDEF",
    "END",
    "ENDDEF",
    "EQ",
    "FALSE",
    "FOR",
    "GE",
    "GET_VALUE",
    "GT",
    "ID",
    "IN",
    "INPUT",
    "INTEGER",
    "LE",
    "LT",
    "MINUS",
    "MODULE",
    "MUL",
    "NE",
    "NUM",
    "OF",
    "OR",
    "PARAMETERS",
    "PLUS",
    "PRINT",
    "PROGRAM",
    "RANGEOP",
    "REAL",
    "RETURNS",
    "RNUM",
    "SEMICOL",
    "SQBC",
    "SQBO",
    "START",
    "SWITCH",
    "TAKES",
    "TRUE",
    "USE",
    "WHILE",
    "WITH"};

char *NT1_HashMap[] = {"AnyTerm",
                      "K",
                      "N1",
                      "N3",
                      "N4",
                      "N5",
                      "N7",
                      "N8",
                      "N9",
                      "N_10",
                      "N_11",
                      "P1",
                      "U",
                      "actual_para_list",
                      "actual_para_list_dash",
                      "arithmeticExpr",
                      "arithmeticOrBooleanExpr",
                      "arrExpr",
                      "arrFactor",
                      "arrTerm",
                      "arr_N4",
                      "arr_N5",
                      "assignmentStmt",
                      "boolConstt",
                      "caseStmts",
                      "condionalStmt",
                      "dataType",
                      "deFault",
                      "declareStmt",
                      "driverModule",
                      "element_index_with_expressions",
                      "expression",
                      "factor",
                      "idListComma",
                      "id_num_rnum",
                      "index_arr",
                      "index_for_loop",
                      "input_plist",
                      "ioStmt",
                      "iterativeStmt",
                      "logicalOp",
                      "lvalueARRStmt",
                      "lvalueIDStmt",
                      "module",
                      "moduleDeclaration",
                      "moduleDeclarations",
                      "moduleDef",
                      "moduleReuseStmt",
                      "new_NT",
                      "new_index",
                      "new_index_for_loop",
                      "op1",
                      "op2",
                      "optional",
                      "otherModules",
                      "program",
                      "range_arrays",
                      "range_for_loop",
                      "relationalOp",
                      "ret",
                      "sign",
                      "sign_for_loop",
                      "simpleStmt",
                      "statement",
                      "statements",
                      "term",
                      "type",
                      "unary_op",
                      "value",
                      "var_id_num",
                      "var_print",
                      "whichStmt"};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* L_HashMap[]={
    "AST_PROGRAM",
    "AST_MODULE_DECLARATIONS",
    "AST_OTHER_MODULE",
    "AST_DRIVER_MODULE",
    "AST_MODULE_ID",
    "AST_OTHER_MODULE_LIST",
    "AST_PARAMETER_LIST",
    "AST_MODULE",
    "AST_ARRAY",
    "AST_ARRAY_RANGE",
    "AST_GET_VALUE",
    "AST_PRINT_VAR",
    "AST_PRINT_INDEX",
    "AST_ASSIGNMENT",
    "AST_ASSIGN_ARR_STMT",
    "AST_INDEX_ARR",
    "AST_MODULE_REUSE",
    "AST_PARAMETER_LIST_DASH",
    "AST_K_RNUM",
    "AST_K_BOOL_CONST",
    "AST_K_N11",
    "AST_U_73",
    "AST_LOGICAL_OPERATOR",
    "AST_RELATOINAL_OPERATOR",
    "AST_PLUS_OR_MINUS",
    "AST_MULT_OR_DIV",
    "AST_FACTOR",
    "AST_UNARY_PLUS_OR_MINUS",
    "AST_DECLARE_STMT",
    "AST_CONDITIONAL_SWITCH",
    "AST_CASE_STMTS",
    "AST_N9",
    "AST_DEFAULT",
    "AST_ITERATIVE_FOR",
    "AST_ITERATIVE_WHILE",
    "AST_RANGE_FOR_LOOP",
    "AST_INDEX_FOR_LOOP",
    "AST_N3",
    "AST_N3_LIST",
    "AST_INPUT_PLIST",
    "AST_N1",
    "AST_STMT_EXP"
};

void abstract_syn_tree_printer(Ast_tree* ast){
    Ast_tree* temptree = ast;
	printf("%-25s %-25s %s\n\n\n", "AST_NODE","NODE_PARENT","LEAF");
    
    if(temptree == NULL)
        return;

    AstNode* root_node = ast->ast_tree_root_node;
	helper_print_ast(root_node);
    printf("\n\n\n\n\n\n");
}

void helper_print_ast(AstNode* node_ast){

    AstNode* tempnode = node_ast;
	if(tempnode==NULL)
		return;

    astChildren* t_chld = tempnode->children;

	ast_nodeprint(node_ast); // first print the current node - preorder 

	astChildren* chld = node_ast->children;

	AstNode* temp = tempnode;
    
	if(chld!=NULL){

		tempnode = chld->head;

		while(tempnode!=NULL){ // now traverse all the children call print on them
			helper_print_ast(tempnode);
			tempnode = tempnode->nextNode;
		}
	}

	
	tempnode = node_ast->compression_node; // print the compressed part of current node at end
    if(tempnode == NULL)
        return;

	helper_print_ast(tempnode);

}

void ast_nodeprint(AstNode* cur_node){
    if(cur_node == NULL)
        return;
	char* node_par;
    AstNode* tempnode;
    
	if(cur_node->parent==NULL){
		node_par = "NULL";
        if(cur_node!= NULL)
            tempnode = cur_node;
	}
	else{
		node_par = L_HashMap[cur_node->parent->label];
	}

    if(cur_node != NULL)
            tempnode = cur_node;

    AstNode* temp_ast_node = cur_node;
    AstNode* temp_parent;
    if(temp_ast_node == NULL)
        return;
    
	if(cur_node->is_leaf==1){
		printf("%-30s %-30s %s\n",T1_HashMap[cur_node->tokenIfLeaf->type],node_par,"yes");
        if(cur_node != NULL)	
            temp_parent = cur_node;	
	}
	else{
        if(cur_node != NULL)
            cur_node = temp_ast_node;	
		printf("%-30s %-30s %s\n",L_HashMap[cur_node->label],node_par,"no");		

	}
		
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ast_tree *createAstfunction(ParseTree *parseTree)
{
    //printf("In createast func\n");
    ast_node_cnt = 0;
    ast_global = (Ast_tree *)malloc(sizeof(Ast_tree));
    if (parseTree->root == NULL)
        return NULL;

    TreeNode *root = parseTree->root;
    createAST_helper(root);

    Ast_tree *abstract_tree = (Ast_tree *)malloc(sizeof(Ast_tree));
    abstract_tree->ast_tree_root_node = NULL;
    abstract_tree->order_of_traversal = 1;
    abstract_tree->ast_tree_root_node = parseTree->root->addr; // root ka address assign kia hai .
    abstract_tree->number_of_nodes = ast_node_cnt;

    return abstract_tree;
}

void createAST_helper(TreeNode *root)
{

    if (root == NULL)
        return;

    TreeNode *chld = root->childHead;

    // printf("In createasthelper func\n");
    
    // if(root->symbol->is_terminal)
    //     printf("%s\n",T1_HashMap[root->symbol->t]);
    // else 
    //     printf("%s\n",NT1_HashMap[root->symbol->nt]);
    // printf("Rule no %d\n",root->current_rule_number);

    if (chld != NULL)
    {
        TreeNode *tnode = chld;
        while (tnode != NULL)
        {
            createAST_helper(tnode);
            tnode = tnode->next;
        }
    }

    if(root->current_rule_number == 0)
        return;
    // reaching the very last leaf node here first - and then processing it in the process node functoin
    analyze_node(root);
}

AstNode *create_ast_leaf(Token *token)
{
    
    AstNode *nullnode = (AstNode *)NULL;
    astChildren* nullchild = (astChildren*)NULL;
    AstNode *retnode = create_ast_node(-1, nullnode, nullnode, nullchild, 1, token);
    return retnode;
}

AstNode *create_ast_node(AstNodeLabels astlabel, AstNode *parent_node, AstNode *next_node, astChildren *chldren, int is_leaf, Token *token)
{
    ast_node_cnt++;
    AstNode *astnode = (AstNode *)malloc(sizeof(AstNode));
    astnode->children = chldren;
    astnode->val = NULL;
    astnode->label = astlabel;
    astnode->parent = parent_node;
    if (is_leaf == 1)
    {
        astnode->tokenIfLeaf = token;
        int cur_line_no = token->line_no;
        astnode->line = cur_line_no;
    }
    else
        astnode->tokenIfLeaf = NULL;
    astnode->nextNode = next_node;
    astnode->str = NULL;

    astnode->is_leaf = is_leaf;
    astnode->compression_node = NULL;
    // current scope
    astnode->order_of_traversal_of_node = 0;
    // node type
    // return parameters , parent function , name , tuple
    astnode->new_children_generated = 0;
}

void update_current_node_parent(astChildren *current_children, AstNode *current_parent)
{
    AstNode *t;
    AstNode *t2;
    AstNode *t3;
    AstNode *t4;

    t3 = current_children->tail; // kabhi
    t4 = current_children->head; // kabhi

    t = current_children->head;
    while (t != NULL)
    {
        t3 = current_children->head; // kabhi alternate

        t->parent = current_parent;
        t2 = t->compression_node;

        t4 = t->nextNode; // kabhi alternate

        while (t2 != NULL)
        {
            t2->parent = current_parent;
            t2 = t2->compression_node;
        }

        t3 = current_children->tail; // kabhi
        t4 = current_children->head; // kabhi

        t = t->nextNode;
    }

    t3 = current_children->tail; // kabhi
    t4 = current_children->head; // kabhi

    current_parent->line = current_children->head->line;
}

void update_ast_node_children(AstNode *node_to_add, astChildren *current_child)
{
    if(node_to_add == NULL)
        return;
    if (current_child->head != NULL)
    {
        current_child->tail->nextNode = node_to_add;
        current_child->tail = current_child->tail->nextNode;
        current_child->sibling_count = current_child->sibling_count + 1;
    }
    else
    {
        current_child->sibling_count = 1;
        current_child->head = current_child->tail = node_to_add;
    }
    return;
}

void child_concatenate_node(AstNode *parent_node, astChildren *chld)
{
}


void helper_inorder(TreeNode *cur_node, TreeNode *last_node)
{
    TreeNode *tempnode;

    TreeNode *tempnode3 = last_node;
    TreeNode *tempnode4 = NULL;

    TreeNode *last_temp_node;

    if (cur_node == NULL)
        return;
    int cur_rule_no = cur_node->current_rule_number;

    TreeNode *tempnode2 = cur_node;

    TreeNode *cur_tree_node_children;
    int tempruleno; // kabhi

    switch (cur_rule_no)
    {

    case 83:
    case 89:
    case 92:
    {
        if (cur_node->parent->symbol->nt == arithmeticExpr || cur_node->parent->symbol->nt == term || cur_node->parent->symbol->nt == arithmeticOrBooleanExpr || cur_node->parent->symbol->nt == AnyTerm)
        {
            
            cur_node->inh_addr = last_node->addr;
            tempnode2 = last_node; // kabhi
        }
        else
        {
            astChildren *cur_node_children = (astChildren *)malloc(sizeof(astChildren));
            cur_node_children->head = cur_node_children->tail = NULL;
            cur_node_children->sibling_count = 0;

            update_ast_node_children(cur_node->parent->childHead->addr, cur_node_children);
            update_ast_node_children(cur_node->parent->inh_addr, cur_node_children);
            update_ast_node_children(last_node->addr, cur_node_children);

            if (cur_node->parent->childHead->addr->is_leaf == 0)
            {
                printf("Error in case 92 - handle tree node leaf\n");
            }
            else
            {
                if (cur_node->parent->childHead->addr->label == AST_MULT_OR_DIV)
                    cur_node->inh_addr = create_ast_node(AST_MULT_OR_DIV, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_PLUS_OR_MINUS)
                    cur_node->inh_addr = create_ast_node(AST_PLUS_OR_MINUS, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_LOGICAL_OPERATOR)
                    cur_node->inh_addr = create_ast_node(AST_LOGICAL_OPERATOR, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_RELATOINAL_OPERATOR)
                    cur_node->inh_addr = create_ast_node(AST_RELATOINAL_OPERATOR, NULL, NULL, cur_node_children, 0, NULL);

                update_current_node_parent(cur_node_children, cur_node->inh_addr);
            }

            tempnode2 = cur_node; // kabhi
            tempnode3 = last_node; // kabhi
        }
        cur_node->addr = cur_node->inh_addr;
    }
    break;

        tempnode2 = NULL;     // kabhi
        tempnode3 = NULL;     // kabhi
        tempnode4 = cur_node; // kabhi
    case 85:
    {
        if (cur_node->parent->symbol->nt == AnyTerm)
        {
            astChildren *cur_node_children = (astChildren *)malloc(sizeof(astChildren));
            cur_node_children->head = cur_node_children->tail = NULL;
            cur_node_children->sibling_count = 0;

            update_ast_node_children(cur_node->childHead->addr, cur_node_children);
            update_ast_node_children(cur_node->childHead->next->addr, cur_node_children);
            update_ast_node_children(last_node->addr, cur_node_children);
            cur_node->addr = create_ast_node(AST_RELATOINAL_OPERATOR, NULL, NULL, cur_node_children, 0, NULL);
            update_current_node_parent(cur_node_children, cur_node->addr);
        }
        else
        {
            printf("Error in case 85");
        }
    }
    break;
    case 86:
    {
        cur_node->addr = last_node->addr;
    }
    break;

    case 82:
    case 88:
    case 91:
    {
        if (cur_node->parent->symbol->nt == arithmeticExpr || cur_node->parent->symbol->nt == term || cur_node->parent->symbol->nt == arithmeticOrBooleanExpr || cur_node->parent->symbol->nt == AnyTerm)
        {
            cur_node->inh_addr = last_node->addr;
            tempnode2 = last_node; // kabhi
        }
        else
        {
            astChildren *cur_node_children = (astChildren *)malloc(sizeof(astChildren));
            cur_node_children->head = cur_node_children->tail = NULL;
            cur_node_children->sibling_count = 0;

            update_ast_node_children(cur_node->parent->childHead->addr, cur_node_children);
            update_ast_node_children(cur_node->parent->inh_addr, cur_node_children);
            update_ast_node_children(last_node->addr, cur_node_children);

            if (cur_node->parent->childHead->addr->is_leaf == 0)
            {
                printf("Error in case 91 - handle tree node leaf\n");
            }
            else
            {
                if (cur_node->parent->childHead->addr->label == AST_MULT_OR_DIV)
                    cur_node->inh_addr = create_ast_node(AST_MULT_OR_DIV, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_PLUS_OR_MINUS)
                    cur_node->inh_addr = create_ast_node(AST_PLUS_OR_MINUS, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_LOGICAL_OPERATOR)
                    cur_node->inh_addr = create_ast_node(AST_LOGICAL_OPERATOR, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_RELATOINAL_OPERATOR)
                    cur_node->inh_addr = create_ast_node(AST_RELATOINAL_OPERATOR, NULL, NULL, cur_node_children, 0, NULL);

                update_current_node_parent(cur_node_children, cur_node->inh_addr);
            }
        }
        cur_tree_node_children = cur_node->childHead;
        tempnode2 = cur_tree_node_children; // kabhi
        tempnode = cur_tree_node_children;
        tempnode3 = cur_tree_node_children; // kabhi
        last_temp_node = NULL;

        while (tempnode != NULL)
        {
            tempnode2 = tempnode; // kabhi

            helper_inorder(tempnode, last_temp_node);
            tempnode3 = cur_tree_node_children; // kabhi
            last_temp_node = tempnode;
            tempnode2 = NULL; // kabhi
            tempnode3 = NULL; // kabhi
            tempnode = tempnode->next;
        }
        tempnode2 = NULL; // kabhi
        cur_node->addr = cur_node->childHead->next->next->addr;
        tempnode3 = NULL;
    }
    break;

    case 81:
    case 84:
    case 87:
    case 90:
    {

        tempruleno = cur_node->child_rule_number; // kabhi

        last_temp_node = NULL;
        cur_tree_node_children = cur_node->childHead;

        tempruleno = cur_node->parent_rule_number; // kabhi

        tempnode = cur_tree_node_children;

        tempruleno = cur_node->child_rule_number; // kabhi

        while (tempnode != NULL)
        {
            tempruleno = cur_node->child_rule_number; // kabhi
            helper_inorder(tempnode, last_temp_node);

            tempruleno = cur_node->parent_rule_number; // kabhi
            tempnode3 = cur_node;                      // kabhi
            last_temp_node = tempnode;
            tempnode2 = NULL; // kabhi
            tempnode = tempnode->next;
        }

        cur_node->addr = cur_node->childHead->next->addr;
    }
    break;

    default:
    {
    }
    break;
    }

    return;
}

void helper_hundred(TreeNode *cur_node, TreeNode *last_node)
{

    TreeNode *tempnode;

    TreeNode *tempnode3 = last_node; // kabhi
    TreeNode *tempnode4 = NULL;      // kabhi

    TreeNode *last_temp_node;

    if (cur_node == NULL)
        return;
    int cur_rule_no = cur_node->current_rule_number;

    TreeNode *tempnode2 = cur_node; // kabhi

    TreeNode *cur_tree_node_children;
    int tempruleno; // kabhi

    switch (cur_rule_no)
    {
    case 100:
    case 103:
    {
        tempruleno = cur_node->child_rule_number; // kabhi

        last_temp_node = NULL;
        cur_tree_node_children = cur_node->childHead;

        tempruleno = cur_node->parent_rule_number; // kabhi

        tempnode = cur_tree_node_children;

        tempruleno = cur_node->child_rule_number; // kabhi

        while (tempnode != NULL)
        {
            tempruleno = cur_node->child_rule_number; // kabhi
            helper_inorder(tempnode, last_temp_node);

            tempruleno = cur_node->parent_rule_number; // kabhi
            tempnode3 = cur_node;                      // kabhi
            last_temp_node = tempnode;
            tempnode2 = NULL; // kabhi
            tempnode = tempnode->next;
        }

        cur_node->addr = cur_node->childHead->next->addr;
    }
    break;

    case 101:
    case 104:
    {
        if (cur_node->parent->symbol->nt == arrExpr || cur_node->parent->symbol->nt == arrTerm)
        {
            cur_node->inh_addr = last_node->addr;
            tempnode2 = last_node; // kabhi
        }
        else
        {
            astChildren *cur_node_children = (astChildren *)malloc(sizeof(astChildren));
            cur_node_children->head = cur_node_children->tail = NULL;
            cur_node_children->sibling_count = 0;

            update_ast_node_children(cur_node->parent->childHead->addr, cur_node_children);
            update_ast_node_children(cur_node->parent->inh_addr, cur_node_children);
            update_ast_node_children(last_node->addr, cur_node_children);

            if (cur_node->parent->childHead->addr->is_leaf == 0)
            {
                printf("Error in case 104 - handle tree node leaf\n");
            }
            else
            {
                if (cur_node->parent->childHead->addr->label == AST_MULT_OR_DIV)
                    cur_node->inh_addr = create_ast_node(AST_MULT_OR_DIV, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_PLUS_OR_MINUS)
                    cur_node->inh_addr = create_ast_node(AST_PLUS_OR_MINUS, NULL, NULL, cur_node_children, 0, NULL);

                update_current_node_parent(cur_node_children, cur_node->inh_addr);
            }
        }
        cur_tree_node_children = cur_node->childHead;
        tempnode2 = cur_tree_node_children; // kabhi
        tempnode = cur_tree_node_children;
        tempnode3 = cur_tree_node_children; // kabhi
        last_temp_node = NULL;

        while (tempnode != NULL)
        {
            tempnode2 = tempnode; // kabhi

            helper_inorder(tempnode, last_temp_node);
            tempnode3 = cur_tree_node_children; // kabhi
            last_temp_node = tempnode;
            tempnode2 = NULL; // kabhi
            tempnode3 = NULL; // kabhi
            tempnode = tempnode->next;
        }
        tempnode2 = NULL; // kabhi
        cur_node->addr = cur_node->childHead->next->next->addr;
        tempnode3 = NULL;
    }
    break;

    case 102:
    case 105:
    {
        if (cur_node->parent->symbol->nt == arrExpr || cur_node->parent->symbol->nt == arrTerm)
        {
            cur_node->inh_addr = last_node->addr;
            tempnode2 = last_node; // kabhi
        }
        else
        {
            astChildren *cur_node_children = (astChildren *)malloc(sizeof(astChildren));
            cur_node_children->head = cur_node_children->tail = NULL;
            cur_node_children->sibling_count = 0;

            update_ast_node_children(cur_node->parent->childHead->addr, cur_node_children);
            update_ast_node_children(cur_node->parent->inh_addr, cur_node_children);
            update_ast_node_children(last_node->addr, cur_node_children);

            if (cur_node->parent->childHead->addr->is_leaf == 0)
            {
                printf("Error in case 105 - handle tree node leaf\n");
            }
            else
            {
                if (cur_node->parent->childHead->addr->label == AST_MULT_OR_DIV)
                    cur_node->inh_addr = create_ast_node(AST_MULT_OR_DIV, NULL, NULL, cur_node_children, 0, NULL);
                else if (cur_node->parent->childHead->addr->label == AST_PLUS_OR_MINUS)
                    cur_node->inh_addr = create_ast_node(AST_PLUS_OR_MINUS, NULL, NULL, cur_node_children, 0, NULL);

                update_current_node_parent(cur_node_children, cur_node->inh_addr);
            }

            tempnode2 = cur_node; // kabhi
            tempnode3 = last_node; // kabhi
        }
        cur_node->addr = cur_node->inh_addr;
    }
    break;

    default:
    {
    }
    break;
    }
    return;
}

void analyze_node(TreeNode *root)
{
    // printf("Analysing -> Rule number - %d\n",root->current_rule_number);
    // if(root->symbol->is_terminal)
    //     printf("%s\n",T1_HashMap[root->symbol->t]);
    // else 
    //     printf("%s\n",NT1_HashMap[root->symbol->nt]);

    switch ((root->current_rule_number))
    {
    case 1:
    {
        // create a child for the current node
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        // add the respective needed children
        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr,current_node_children);
        update_ast_node_children(root->childHead->next->next->addr,current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr,current_node_children);

        root->addr = create_ast_node(AST_PROGRAM, NULL, NULL, current_node_children, 0, NULL);
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    // handle ID s insingle statement
    case 4:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->token);
        update_ast_node_children(leaf, current_node_children);

        root->addr = create_ast_node(AST_MODULE_ID, NULL, NULL, current_node_children, 0, NULL);
        ast_global->number_of_nodes += 1; // kabhi
        update_current_node_parent(current_node_children, root->addr);
    }
    break;
    case 33:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->token);
        update_ast_node_children(leaf, current_node_children);

        root->addr = create_ast_node(AST_GET_VALUE, NULL, NULL, current_node_children, 0, NULL);
        ast_global->number_of_nodes += 1; // kabhi
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    // handle terminals on right and only one non terminal - 12 rules
    case 7:
    {
        root->addr = root->childHead->next->next->next->next->addr;
    }
    break;
    case 9:
    {
        root->addr = root->childHead->next->next->addr;
    }
    break;
    case 25:
    {
        root->addr = root->childHead->next->addr;
        if(root->addr!=NULL){
        root->addr->scopeLineNumberStart = root->childHead->token->line_no;
        root->addr->scopeLineNumberEnd = root->childHead->next->next->token->line_no;
        }
    }
    break;
    case 34:
    {
        root->addr = root->childHead->next->next->addr;
    }
    break;
    case 44:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 51:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 67:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 69:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 74:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 93:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 109:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 111:
    {
        root->addr = root->childHead->next->addr;
    }
    break;
    case 132:
    {
        root->addr = root->childHead->next->next->addr;
    }
    break;

    // handling list type grammar rules NT-> ID/NT   NT
    case 2:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        root->addr = create_ast_node(AST_MODULE_DECLARATIONS, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->addr;
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 5:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        root->addr = create_ast_node(AST_OTHER_MODULE_LIST, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->addr;
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 8:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->next->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->next->next->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_MODULE, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 11:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_INPUT_PLIST, NULL, NULL, current_node_children, 0, NULL);

        root->addr->children->head->compression_node = root->childHead->next->next->next->addr;

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 12:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_N1, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->next->next->next->addr;

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 14:
    { // created a new astlabel - n3
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);

        root->addr = create_ast_node(AST_N3, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->addr;

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 15:
    { // created a new astlabel - n3list
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->token);
        update_ast_node_children(leaf, current_node_children);

        root->addr = create_ast_node(AST_N3_LIST, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->next->addr;

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    // create array of type
    case 20:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_ARRAY, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 21:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_ARRAY_RANGE, NULL, NULL, current_node_children, 0, NULL);
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 26:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_STMT_EXP, NULL, NULL, current_node_children, 0, NULL);
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 41:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);

        root->addr = create_ast_node(AST_PRINT_INDEX, NULL, NULL, current_node_children, 0, NULL);
        root->addr->children->head->compression_node = root->childHead->next->addr;

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 48:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);
        root->addr = create_ast_node(AST_ASSIGNMENT, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 52:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->addr, current_node_children);
        root->addr = create_ast_node(AST_ASSIGN_ARR_STMT, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 53:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);
        root->addr = create_ast_node(AST_INDEX_ARR, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 59:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->next->token);
        update_ast_node_children(leaf, current_node_children);

        update_ast_node_children(root->childHead->next->next->next->next->next->next->addr, current_node_children);
        root->addr = create_ast_node(AST_MODULE_REUSE, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 60:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_PARAMETER_LIST, NULL, NULL, current_node_children, 0, NULL);
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 61:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_PARAMETER_LIST_DASH, NULL, NULL, current_node_children, 0, NULL);
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 66:
    { // created a new astlabel - n3
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_K_N11, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 73:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_U_73, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 96:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_FACTOR, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 98:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_UNARY_PLUS_OR_MINUS, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 124:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_DECLARE_STMT, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 125:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_CONDITIONAL_SWITCH, NULL, NULL, current_node_children, 0, NULL);

        if(root->addr!=NULL){
        root->addr->scopeLineNumberStart = root->childHead->next->next->next->next->token->line_no;
        root->addr->scopeLineNumberEnd = root->childHead->next->next->next->next->next->next->next->token->line_no;
        }
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 126:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->addr, current_node_children);
        root->addr = create_ast_node(AST_CASE_STMTS, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 127:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->addr, current_node_children);
        root->addr = create_ast_node(AST_N9, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 134:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        AstNode *leaf = create_ast_leaf(root->childHead->next->next->token);
        update_ast_node_children(leaf, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_ITERATIVE_FOR, NULL, NULL, current_node_children, 0, NULL);
        if(root->addr!=NULL){
        root->addr->scopeLineNumberStart = root->childHead->next->next->next->next->next->next->token->line_no;
        root->addr->scopeLineNumberEnd = root->childHead->next->next->next->next->next->next->next->next->token->line_no;
        }
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 135:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->next->next->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->next->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_ITERATIVE_WHILE, NULL, NULL, current_node_children, 0, NULL);
        if(root->addr != NULL){
        root->addr->scopeLineNumberStart = root->childHead->next->next->next->next->token->line_no;
        root->addr->scopeLineNumberEnd = root->childHead->next->next->next->next->next->next->token->line_no;   
        }
        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 136:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->next->addr, current_node_children);

        root->addr = create_ast_node(AST_RANGE_FOR_LOOP, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 137:
    {
        astChildren *current_node_children = (astChildren *)malloc(sizeof(astChildren));
        current_node_children->head = current_node_children->tail = NULL;
        current_node_children->sibling_count = 0;

        update_ast_node_children(root->childHead->addr, current_node_children);
        update_ast_node_children(root->childHead->next->addr, current_node_children);

        root->addr = create_ast_node(AST_INDEX_FOR_LOOP, NULL, NULL, current_node_children, 0, NULL);

        update_current_node_parent(current_node_children, root->addr);
    }
    break;

    case 28:
        root->addr = root->childHead->addr;
        break;
    case 29:
        root->addr = root->childHead->addr;
        break;
    case 30:
        root->addr = root->childHead->addr;
        break;
    case 31:
        root->addr = root->childHead->addr;
        break;
    case 32:
        root->addr = root->childHead->addr;
        break;
    case 40:
        root->addr = root->childHead->addr;
        break;
    case 46:
        root->addr = root->childHead->addr;
        break;
    case 47:
        root->addr = root->childHead->addr;
        break;
    case 49:
        root->addr = root->childHead->addr;
        break;
    case 50:
        root->addr = root->childHead->addr;
        break;
    case 65:
        root->addr = root->childHead->addr;
        break;
    case 71:
        root->addr = root->childHead->addr;
        break;
    case 72:
        root->addr = root->childHead->addr;
        break;
    case 75:
        root->addr = root->childHead->addr;
        break;
    case 97:
        root->addr = root->childHead->addr;
        break;
    case 99:
        root->addr = root->childHead->addr;
        break;
    case 108:
        root->addr = root->childHead->addr;
        break;
    case 110:
        root->addr = root->childHead->addr;
        break;
        // null rules

    case 3:
        root->addr = NULL;
        break;
    case 6:
        root->addr = NULL;
        break;
    case 10:
        root->addr = NULL;
        break;
    case 13:
        root->addr = NULL;
        break;
    case 16:
        root->addr = NULL;
        break;
    case 27:
        root->addr = NULL;
        break;
    case 45:
        root->addr = NULL;
        break;
    case 58:
        root->addr = NULL;
        break;
    case 62:
        root->addr = NULL;
        break;
    case 68:
        root->addr = NULL;
        break;
    case 70:
        root->addr = NULL;
        break;
    case 128:
        root->addr = NULL;
        break;
    case 133:
        root->addr = NULL;
        break;
    case 141:
        root->addr = NULL;
        break;
        // now for leaf

    case 17:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 18:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 19:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 22:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 23:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 24:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 37:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 35:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 36:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 38:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 39:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 42:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 43:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 54:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 55:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 56:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 57:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 63:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 64:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 76:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 77:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 78:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 79:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 80:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 94:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 95:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 106:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 107:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 112:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_PLUS_OR_MINUS;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 113:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_PLUS_OR_MINUS;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 114:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_MULT_OR_DIV;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 115:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_MULT_OR_DIV;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 116:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_LOGICAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 117:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_LOGICAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 118:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 119:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 120:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 121:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 122:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 123:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        root->addr->label = AST_RELATOINAL_OPERATOR;
        ast_global->number_of_nodes += 1;
    }
    break;
    case 129:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 130:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 131:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 138:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 139:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;
    case 140:
    {
        root->addr = create_ast_leaf(root->childHead->token);
        ast_global->number_of_nodes += 1;
    }
    break;

    case 81:
    {
        helper_inorder(root, NULL);
    }
    break;
    case 84:
    {
        helper_inorder(root, NULL);
    }
    break;
    case 87:
    {
        helper_inorder(root, NULL);
    }
    break;
    case 90:
    {
        helper_inorder(root, NULL);
    }
    break;

    case 82:
    {
        root->addr = NULL;
    }
    break;
    case 83:
    {
        root->addr = NULL;
    }
    break;
    case 88:
    {
        root->addr = NULL;
    }
    break;
    case 89:
    {
        root->addr = NULL;
    }
    break;
    case 91:
    {
        root->addr = NULL;
    }
    break;
    case 92:
    {
        root->addr = NULL;
    }
    break;
    case 86:
    {
        root->addr = NULL;
    }
    break;
    case 85:
    {
        root->addr = NULL;
    }
    break;

    case 100:
    {
        helper_hundred(root, NULL);
    }
    break;
    case 103:
    {
        helper_hundred(root, NULL);
    }
    break;

    case 101:
    {
        root->addr = NULL;
    }
    break;
    case 102:
    {
        root->addr = NULL;
    }
    break;
    case 104:
    {
        root->addr = NULL;
    }
    break;
    case 105:
    {
        root->addr = NULL;
    }
    break;
    // to handle 85 86
    default:
        break;
    }
}

// khatm 