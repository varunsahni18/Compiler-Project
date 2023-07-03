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
#include <time.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"

void clear_terminal()
{
#ifdef _WIN32
    system("cls");
#endif
#ifdef _WIN64
    system("cls");
#endif
#ifdef __unix__
    system("clear");
#endif
#ifdef __linux__
    system("clear");
#endif
}

int main(int argc, char **argv)
{
    
    
    clear_terminal();
    int val = driver_load_grammar_func();
    if (val == (-1))
    {
        printf("Error loading grammar\n");
        exit(1);
    }

    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    start_time = clock();

    int option;
    ParseTree *ptree = NULL;
    int flag_opt_5 = 0;
    Ast_tree *a = NULL;
    

    do
    {
        printf("Select from the following options :\n");
        printf("0: Exit\n");
        printf("1: Lexer: Token list on console\n");
        printf("2: Parser: Verify syntactic correctness and print the parse tree on console\n");
        printf("3: AST: Printing AST on console\n");
        printf("4: Memory: For dispalying memory allocated to AST and parsetree along with compression ratio\n");
        printf("5: Symbol Table printing\n");
        printf("6: Activation record size\n");
        printf("7: Static and dynamic arrays\n");
        printf("8: Error reporting and total compile time\n");
        printf("9: Code generation\n");

        printf("Select an option : ");


        

        scanf("%d", &option);

        char *input_file_name = "t1.txt";
        char *clean_file = "clean_file.txt";

        switch (option)
        {
        case 0:
        {
            break;
        }
        case 1:
        {
            getAllTokens(argv[1]);
            break;
        }
        case 2:
        {
            
            ptree = driver_parse_function(argv[1], argv[2]);
            break;
        }
        case 3:
        {
            flag_opt_5 = 1;
            if (ptree == NULL)
            {
                printf("Parse tree not created - select option 2 first\n");
                break;
            }
            if(a==NULL)
                a = createAstfunction(ptree);
            printf("Printing AST: preorder traversal\n");
            abstract_syn_tree_printer(a);
            printf("AST PRINTING COMPLETE\n\n\n\n");

            break;   
        }
        case 4:
        {
            if(ptree == NULL){
                printf("Parse tree not created - select option 2 first\n");
                break;
            }
            if(a == NULL){
                printf("AST not created - select option 3 first\n");
                break;
            }

            int cur_ast_node_cnt = a->number_of_nodes , cur_ptree_node_cnt = ptree->parse_tree_node_cnt;
            double ast_mem_alloc = cur_ast_node_cnt*sizeof(AstNode);
            double ptree_mem_alloc = cur_ptree_node_cnt * sizeof(TreeNode);

            printf("Parse tree number of nodes = %d\n",cur_ptree_node_cnt);
            printf("AST number of nodes = %d\n",cur_ast_node_cnt);
            double compression_percentage = (ptree_mem_alloc-ast_mem_alloc)*100;
            compression_percentage=compression_percentage/ptree_mem_alloc;
            
            printf("Compression Percentage = %f\n",compression_percentage);
            break;
        }
        case 5:
        {
            if (flag_opt_5 == 0)
            {
                printf("AST not created - select option 5 first\n");
                break;
            }

            if (a != NULL)
            {
                initFst();
                AstTraversal(a->ast_tree_root_node);
                printf("\n\n PRINTING SYMBOL TABLE \n\n");
                printFunctionSymbolTable();
                printf("\n\n PRINTING COMPLETE \n\n");

            }
            break;
        }
        case 6:
        case 7:
        
        case 9:{
            printf("Really sorry ma'am - option not 6,7,9 implemented fully. Please select another option\n");
            break;
        }
        case 8:{
            initFst();
            AstTraversal(a->ast_tree_root_node);
            break;
        }
        default:
        {
            printf("Invalid option selected!");
            break;
        }
        }

    } while (option);
    printf("Program Eneded\n");
    
    return 0;
}