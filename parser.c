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
#include <stdbool.h>
#include <string.h>
#include "parser.h"

int parse_node_cnt=0;
// 1. Generate Grammar

char *T_HashMap[] = {
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

char *NT_HashMap[] = {"AnyTerm",
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

Symbol *startSymbol;
int get_terminal_index(char *s)
{
    int i = 0;
    while (i < NO_OF_TERMINALS)
    {
        if (!strcmp(T_HashMap[i], s))
            return i;
        i++;
    }
    return -1;
}
int get_nonterminal_index(char *s)
{
    int i = 0;
    // printf("Entered %d\n",NO_OF_NON_TERMINALS);
    while (i < NO_OF_NON_TERMINALS)
    {
        // printf("Entered %d  %s\n",i,NT_HashMap[0]);

        if (!strcmp(NT_HashMap[i], s))
            return i;
        i++;
    }
    return -1;
}

Symbol *new_symbol(char *s, bool is_terminal)
{
    Symbol *t = (Symbol *)malloc(sizeof(Symbol));
    t->is_terminal = is_terminal;
    if (is_terminal)
        t->t = get_terminal_index(s);
    else
        t->nt = get_nonterminal_index(s);
    return t;
}
SymbolNode *new_symbol_node(char *s, bool is_terminal)
{
    SymbolNode *t = (SymbolNode *)malloc(sizeof(SymbolNode));
    t->symbol = new_symbol(s, is_terminal);
    t->next = NULL;
    return t;
}
Rule *new_rule(NonTerminal nt, int ruleNo)
{
    Rule *t = (Rule *)malloc(sizeof(Rule));
    t->head = NULL;
    t->next = NULL;
    t->length = 0;
    t->ruleNo = ruleNo;
    t->LHS = nt;
    return t;
}
Rules *new_rules(NonTerminal nt)
{
    Rules *t = (Rules *)malloc(sizeof(Rules));
    t->LHS = nt;
    t->noOfRules = 0;
    t->head = NULL;
    return t;
}

Grammar *g;
Rule **headRule;
Rules **headRules;
FirstAndFollow *set;

void generate_grammar(FILE *fp)
{
    int ruleNo = 0;
    char row[200];

    headRule = (Rule **)malloc(sizeof(Rule *) * NO_OF_RULES);

    while (fscanf(fp, "%[^\n] ", row) != -1)
    {
        // printf("%s\n",row);
        int ind = 0;
        char *lhsNT = (char *)malloc(sizeof(char) * MAX_SYMBOL_LENGTH);

        /* LHS */
        while (row[ind] != '>')
        {

            char c = row[ind];
            if (c == '<')
                ;
            else
            {
                lhsNT[ind - 1] = c;
            }
            ind++;
        }

        lhsNT[ind - 1] = '\0';
        ind++;
        // printf("%d\n",get_nonterminal_index(lhsNT));
        //  printf("%s\n%d\n",lhsNT,get_nonterminal_index(lhsNT));
        headRule[ruleNo] = new_rule(get_nonterminal_index(lhsNT), ruleNo);

        free(lhsNT);

        /* --> */
        while (row[ind] != '>')
        {
            ind++;
        }
        ind++;

        SymbolNode *tempNode = NULL;

        /* RHS */
        while (row[ind] != '\0')
        {
            char c = row[ind];
            if (c == ' ')
            {
                ind++;
                continue;
            }

            char *rhsSym = (char *)malloc(sizeof(char) * MAX_SYMBOL_LENGTH);
            bool is_terminal = false;

            if (c == '<') // Non-Terminal
            {
                is_terminal = false;
                ind++;
                int rhsNTind = ind;
                while (row[ind] != '>')
                {
                    rhsSym[ind - rhsNTind] = row[ind];
                    ind++;
                }
                rhsSym[ind - rhsNTind] = '\0';
            }
            else // Terminal
            {
                is_terminal = true;
                int rhsTind = ind;
                while (row[ind] >= 33 && row[ind] <= 125)
                {
                    rhsSym[ind - rhsTind] = row[ind];
                    ind++;
                }
                rhsSym[ind - rhsTind] = '\0';
            }

            headRule[ruleNo]->length++;

            if (tempNode == NULL)
            {
                tempNode = new_symbol_node(rhsSym, is_terminal);
                headRule[ruleNo]->head = tempNode;
            }
            else
            {
                tempNode->next = new_symbol_node(rhsSym, is_terminal);
                tempNode = tempNode->next;
            }

            free(rhsSym);
            ind++;
        }

        memset(row, '\0', sizeof(row));
        ruleNo++;
    }

    headRules = (Rules **)malloc(sizeof(Rules *) * NO_OF_NON_TERMINALS);

    int nonTerminalNo = 0;
    while (nonTerminalNo < NO_OF_NON_TERMINALS)
    {

        headRules[nonTerminalNo] = new_rules(nonTerminalNo);
        Rule *tempRule = NULL;

        ruleNo = 0;
        while (ruleNo < NO_OF_RULES)
        {
            if (headRule[ruleNo]->LHS == nonTerminalNo)
            {
                if (tempRule == NULL)
                {
                    tempRule = headRule[ruleNo];
                    headRules[nonTerminalNo]->head = tempRule;
                }
                else
                {
                    tempRule->next = headRule[ruleNo];
                    tempRule = tempRule->next;
                }
                headRules[nonTerminalNo]->noOfRules++;
            }
            ruleNo++;
        }

        nonTerminalNo++;
    }

    g = (Grammar *)malloc(sizeof(Grammar));

    g->rules = (Rules **)malloc(sizeof(Rules *) * NO_OF_NON_TERMINALS);
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        g->rules[i] = headRules[i];
    }
}

/* FOR TESTING GRAMMAR READ */
void test_grammar()
{
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        Rule *tempRule = g->rules[i]->head;

        for (int j = 0; j < g->rules[i]->noOfRules; j++)
        {
            printf("%d %d | <%s> -->", tempRule->ruleNo, tempRule->length, NT_HashMap[g->rules[i]->LHS]);
            SymbolNode *temp = tempRule->head;
            while (temp != NULL)
            {
                if (temp->symbol->is_terminal)
                    printf(" %s", T_HashMap[temp->symbol->t]);
                else
                    printf(" <%s>", NT_HashMap[temp->symbol->nt]);
                temp = temp->next;
            }
            tempRule = tempRule->next;
            printf("\n");
        }
    }
}

// 2. First & Follow Set Automation

void insert_in_set(int *set, int index)
{
    set[index] = 1;
}
void remove_from_set(int *set, int index) // removes the terminal from the set
{
    set[index] = 0;
}
bool find_in_set(int *set, int index) // finds whether the given terminal is present in the set or not
{
    return set[index];
}
void combine_sets(int *set1, int *set2)
{
    // Used to insert the elements of set2 into set1
    for (int term = 0; term < NO_OF_TERMINALS; term++)
    {

        if (term == EPSILON)
            continue;

        if (find_in_set(set2, term))
            insert_in_set(set1, term);
    }
}

void compute_follow_set()
{
    int iterations = 100;
    while (iterations--)
    {
        // printf("w");
        for (int i = 0; i < NO_OF_RULES; i++) // traversing over all 108 rules
        {

            SymbolNode *temp = headRule[i]->head;

            while (temp != NULL) // traversing over the RHS symbols in a rule
            {

                if (!(temp->symbol->is_terminal)) // temp->symbol is a non-terminal
                {
                    SymbolNode *nextSymbol = temp->next; // compute nextSymbol : going ahead in list for the NT symbol (temp)

                    while (nextSymbol != NULL)
                    {
                        if (nextSymbol->symbol->is_terminal) // next symbol is a terminal
                        {
                            insert_in_set(set->follow_set[temp->symbol->nt], nextSymbol->symbol->t);
                            break;
                        }
                        else // FIRST(nextSymbol) : next symbol is a non terminal
                        {
                            combine_sets(set->follow_set[temp->symbol->nt], set->first_set[nextSymbol->symbol->nt]);
                            if (!find_in_set(set->first_set[nextSymbol->symbol->nt], EPSILON))
                            {
                                break;
                            }
                        }
                        nextSymbol = nextSymbol->next;
                    }

                    if (nextSymbol == NULL) // FOLLOW(LHSnt)
                    {
                        combine_sets(set->follow_set[temp->symbol->nt], set->follow_set[headRule[i]->LHS]);
                    }
                }

                temp = temp->next;
            }
        }
    }
}
void compute_first_set(NonTerminal nonTerminal)
{

    // int **first_set = set->first_set;

    Rules *rules = g->rules[nonTerminal]; // returns all rules corresponding to a non terminal
    Rule *rule = rules->head;

    while (rule != NULL) // traversing over all the rules corresponding to a non terminal
    {
        SymbolNode *temp = rule->head;
        // printf("%d",rule->ruleNo);

        while (temp != NULL) // traversing over the symbols in a rule
        {

            // symbol is a terminal

            if (temp->symbol->is_terminal)
            {

                int index = temp->symbol->t;
                insert_in_set(set->first_set[nonTerminal], index); // adding the terminal to the first set of the corresponding non terminal
                break;
            }

            // symbol is a non-terminal
            else
            {

                compute_first_set(temp->symbol->nt);                                         // compute first set of symbol
                combine_sets(set->first_set[nonTerminal], set->first_set[temp->symbol->nt]); // add to first set of non-terminal the first set of symbol

                if (!find_in_set(set->first_set[temp->symbol->nt], EPSILON))
                { // if first set of symbol does not contain epsilon
                    break;
                }
            }

            temp = temp->next;
        }
        if (!temp)
            insert_in_set(set->first_set[nonTerminal], EPSILON);

        rule = rule->next;
    }
}
void init_first_and_follow_sets()
{

    set = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    set->follow_set = (int **)malloc(NO_OF_NON_TERMINALS * sizeof(int *));
    set->first_set = (int **)malloc(NO_OF_NON_TERMINALS * sizeof(int *));

    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        set->follow_set[i] = (int *)malloc(NO_OF_TERMINALS * sizeof(int));
        set->first_set[i] = (int *)malloc(NO_OF_TERMINALS * sizeof(int));
        // allocating the memory to the first and follow sets and initialising them to 0

        // Initializing bit vector to 0
        memset(set->follow_set[i], 0, sizeof(set->follow_set[i]));
        memset(set->first_set[i], 0, sizeof(set->first_set[i]));
    }

    // first=set->first_set;
    // follow=set->follow_set;
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        compute_first_set(i);
    }

    compute_follow_set();
}

void printFirstSet()
{
    printf("First Set:\n");
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        printf("%s: ", NT_HashMap[i]);
        printf("{ ");
        for (int j = 0; j < NO_OF_TERMINALS; j++)
        {
            if (find_in_set(set->first_set[i], j))
            {
                printf("%s ", T_HashMap[j]);
            }
        }
        printf("}\n");
    }
}
void printFollowSet()
{
    printf("Follow Set:\n");
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        printf("%s: ", NT_HashMap[i]);
        printf("{ ");
        for (int j = 0; j < NO_OF_TERMINALS; j++)
        {
            if (find_in_set(set->follow_set[i], j))
            {
                printf("%s ", T_HashMap[j]);
            }
        }
        printf("}\n");
    }
}

// // 3. Parse Table Creation(Automated)

void createParseTable(Grammar *g, int **first_set, int **follow_set, ParsingTable *table)
{

    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        // rules[i] is a rules* which corresponds to head and length of rules of the non terminal at the index i
        Rule *rule_ptr = g->rules[i]->head; // Rule at index i
        for (int j = 0; j < g->rules[i]->noOfRules; j++)
        {
            SymbolNode *sym = rule_ptr->head;
            for (int itr = 0; itr < rule_ptr->length; itr++)
            {
                if (sym->symbol->is_terminal)
                {
                    if (sym->symbol->t == EPSILON)
                    {
                        int *follow_set_of_i = follow_set[i];
                        for (int k = 0; k < NO_OF_TERMINALS; k++)
                        {
                            if (find_in_set(follow_set_of_i, k))
                            {
                                table->cells[i][k] = rule_ptr;
                            }
                        }
                    }
                    else
                    {
                        table->cells[i][sym->symbol->t] = rule_ptr;
                    }
                    break;
                }
                else
                {
                    int *first_set_NT_i = first_set[sym->symbol->nt]; // first set corresponding to the first non terminal symbol of ruleptr

                    for (int k = 0; k < NO_OF_TERMINALS; k++)
                    {

                        if (find_in_set(first_set_NT_i, k))
                        {
                            if (k != EPSILON)
                                table->cells[i][k] = rule_ptr;
                        }
                    }

                    if (!find_in_set(first_set_NT_i, EPSILON))
                        break;
                }
                sym = sym->next;
            }

            rule_ptr = rule_ptr->next;
        }
    }
}

void print_parse_table(ParsingTable *parsing_table)
{

    printf("\nPrinting parse table now\n\n");

    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        printf("%s -> ", NT_HashMap[i]);

        for (int j = 1; j < NO_OF_TERMINALS; j++)
        {
            if (parsing_table->cells[i][j] != NULL)
            {
                printf("%s ", T_HashMap[j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// void print_parse_table(ParsingTable* parsing_table){
//     printf("\n Printing parse table now\n\n");
//     for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
//     {
//         printf("%s ", NT_HashMap[i]);
//         for (int j = 0; j < NO_OF_TERMINALS; j++)
//         {
//             if(parsing_table->cells[i][j]!=NULL){
//                 printf("%s ", parsing_table->cells[i][j]->ruleNo);
//             }
//             else{
//                 printf("N ");
//             }
//         }
//         printf("\n");
//     }
//     printf("\n");

// }

ParsingTable *initialise_parse_table()
{
    ParsingTable *parse_table = (ParsingTable *)malloc(sizeof(ParsingTable));
    for (int i = 0; i < NO_OF_NON_TERMINALS; i++)
    {
        for (int j = 0; j < NO_OF_TERMINALS; j++)
        {
            parse_table->cells[i][j] = NULL;
        }
    }
    return parse_table;
}

void print_stack(Stack *s, FILE *fptr)
{
    if (s->size == 0)
        return;
    StackNode *elem = s->top;
    if (elem == NULL)
        printf("NULL element\n");
    while (elem != NULL)
    {
        if (elem->treeNodePointer->symbol->is_terminal == 0)
            printf("%s ", NT_HashMap[elem->treeNodePointer->symbol->nt]);
        else
            printf("%s ", T_HashMap[elem->treeNodePointer->symbol->t]);

        elem = elem->next;
    }
    printf("\n\n");
    return;
}

ParseTree *parseInputSourceCode(char *input_file_name, ParsingTable *parseTable, int *syntaxErorrs, Grammar *grammar, FILE *fptr)
{
    if (fptr == NULL)
        return NULL;
    int *lexicalErrors;

    // Initialise Parse Tree
    ParseTree *tree = (ParseTree *)malloc(sizeof(ParseTree));

    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    root->noOfChildren = 0;
    root->childHead = NULL;
    root->next = NULL;
    root->parent = NULL;
    root->symbol = startSymbol;
    root->token = NULL;
    tree->root = root;

    TreeNode *temp = tree->root;

    // Initialise PDA : Stack
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->size = 0;
    s->top = NULL;

    pushToStack(s, tree->root);

    FILE *fptr_from_lexer = lexerInit(input_file_name);
    if (fptr_from_lexer == NULL)
    {
        printf("Error opening source code file\n");
        return NULL;
    }

    Token *L = getNextToken(&fptr_from_lexer);

    int error_line_no = -1;

    while (L != NULL)
    {
        // print_stack(s,fptr);

        if (L->type_of_value == (-1))
        {
            printf("Lexical error\n");
            L = getNextToken(&fptr_from_lexer);
            continue;
        }

        if (s->size == 0)
        {
            *syntaxErorrs = 1;
            printf("ERROR in Parsing : Empty Stack\n");
            break;
        }

        // Parsing Algorithm Begins
        // fprintf(fptr, "%s\n", L->lexeme);
        // fprintf(fptr, "%s\n", T_HashMap[L->type]);

        StackNode *stackTopNode = popFromStack(s);
        TreeNode *treeNode = stackTopNode->treeNodePointer;

        // Case 1: Top of Stack is a terminal
        if (treeNode->symbol->is_terminal)
        {
            if (treeNode->symbol->t == L->type) // Case 1.a: Top of Stack matches with Lookahead Token
            {
                treeNode->token = L;
                L = getNextToken(&fptr_from_lexer);

                continue;
            }
            else // Case 1.b: Top of Stack does not match with Lookahead Token - ERROR
            {
                *syntaxErorrs = 1;
                if (error_line_no != L->line_no)
                    printf("ERROR in Parsing line - %d: Terminal Mismatch Error\n", L->line_no);
                L = getNextToken(&fptr_from_lexer);
                pushToStack(s, treeNode);
                error_line_no = L->line_no;

                continue;
            }
        }
        else // Case 2: Top of Stack is a non-terminal
        {
            NonTerminal nt = treeNode->symbol->nt;
            Terminal t = L->type;

            Rule *rule = parseTable->cells[nt][t];

            if (rule != NULL) // Case 2a: Valid Rule Present in ParseTable
            {
                addChildToTree(treeNode, rule);
                addToStack(s, treeNode);
            }
            else // Case 2b: ERROR NULL access from parse table
            {
                *syntaxErorrs = 1;
                if (error_line_no != L->line_no)
                    printf("ERROR in Parsing line - %d : Null access from parse table\n", L->line_no);
                pushToStack(s, treeNode);
                L = getNextToken(&fptr_from_lexer);
                error_line_no = L->line_no;
                continue;
            }
        }
    }

    if (s->size > 1)
    {
        *syntaxErorrs = 1;
        printf("ERROR in Parsing\n");
    }

    if (fptr_from_lexer != NULL)
        fclose(fptr_from_lexer);
    return tree;
}

void print_node(TreeNode *tnode, FILE *fptr)
{
    if (fptr == NULL)
    {
        return;
    }
    // printing current node
    char *str;
    if (tnode->symbol->is_terminal)
        str = T_HashMap[tnode->symbol->t];
    else
        str = NT_HashMap[tnode->symbol->nt];

    // fprintf(fptr, "Lexeme \tLineNo \tTokenName \tRealOrIntegerValue \t GrammarSymbolOfParent \t IsLeafNode \t NodeSymbol \n");
    if (tnode->token != NULL)
    {
        printf("     %-15s %-15d %-15s ", tnode->token->lexeme, tnode->token->line_no, T_HashMap[tnode->token->type]);
        if (tnode->token->type_of_value == 0)
            printf("%-15d ", tnode->token->val->num);
        else if (tnode->token->type_of_value == 1)
        {
            printf(" %-15lf ", tnode->token->val->rnum);
        }
        else
            printf("    -----    ");
    }
    else
        printf("    -----        -----        -----        -----    ");

    if (tnode->parent != NULL)
        printf(" %-15s ", NT_HashMap[tnode->parent->symbol->nt]);
    else
        printf("     ROOT    ");

    if (tnode->noOfChildren == 0)
        printf("      YES     ");
    else
        printf("      NO      ");

    if (tnode->symbol->is_terminal)
        printf(" %-15s\n ", T_HashMap[tnode->symbol->t]);
    else
        printf(" %-15s\n ", NT_HashMap[tnode->symbol->nt]);

    parse_node_cnt++;
    return;
}

void print_tree(TreeNode *Tnode, FILE *fptr)
{
    if (Tnode == NULL)
        return;
    if (Tnode->noOfChildren == 0)
    {
        // this is a leaf
        print_node(Tnode, fptr);
        return;
    }
    if (fptr == NULL)
        return;
    // print the leftmost child of the tree
    TreeNode *chldnode = Tnode->childHead;
    print_tree(chldnode, fptr);

    // print the current node of the tree
    print_node(Tnode, fptr);

    // print the rest of the children
    chldnode = chldnode->next;
    for (int i = 1; i < Tnode->noOfChildren; i++)
    {
        print_tree(chldnode, fptr);
        chldnode = chldnode->next;
    }
    return;
}

int driver_load_grammar_func()
{
    FILE *fp;
    fp = fopen("grammar.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening grammar file!");
        return -1;
    }

    generate_grammar(fp);
    return 1;
    // test_grammar();
}

ParseTree* driver_parse_function(char *input_file_name, char *parse_tree_file_output)
{
    parse_node_cnt = 0;
    // printf("UNSUDNUN|N\n\n");
    init_first_and_follow_sets();
    startSymbol = (Symbol *)malloc(sizeof(Symbol));
    startSymbol->is_terminal = 0;
    startSymbol->nt = program;

    // printFirstSet(); - uncomment these lines to print the first and follow sets
    // printFollowSet();
    //     char* parse_output="parse_output_file.txt";
    // FILE* fptr=fopen(parse_tree_file_output,"w+");

    ParsingTable *ptable = initialise_parse_table();
    createParseTable(g, set->first_set, set->follow_set, ptable);
    // print_parse_table(ptable); // invoke this function to parses table
   
    int *syntxerr = malloc(sizeof(int));

    //     // printf("Back To main\n");
    //     // printf("%s\n", NT_HashMap[ptree->root->childHead->next->symbol->nt]);
    //     char* parse_tree_file = "parse_tree_outpur_file.txt";
    FILE *parse_file_ptr = fopen(parse_tree_file_output, "w+");
    ParseTree *ptree = parseInputSourceCode(input_file_name, ptable, syntxerr, g, parse_file_ptr);
    ParseTree* retnode = ptree;
    
    if(retnode == NULL){
        printf("NULL retnode\n");
    }

    printf("  Lexeme   \t LineNo  \t  TokenName   \t  RealOrIntegerValue \t GrammarSymbolOfParent \t  IsLeafNode  \t NodeSymbol \n");
    print_tree(ptree->root, parse_file_ptr); // this will create the file

    //     // printf("%s\n",NT_HashMap[ptable->cells[arithmeticOrBooleanExpr][ID]->head->symbol->nt]);
    if (*syntxerr == 0)
    {
        printf("\nParsing Completed Successfully \n\n\n\n");
    }
    else
    {
        printf("\nError in Parsing \n\n\n\n");
    }
    if (parse_file_ptr != NULL)
        fclose(parse_file_ptr);
    else
        printf("NULLPTR\n");
    retnode->parse_tree_node_cnt = parse_node_cnt;
    return retnode;
}
// int main()
// {

//     FILE *fp;
//     fp = fopen("grammar.txt", "r");
//     if (fp == NULL)
//     {
//         printf("Error opening file!");
//         exit(1);
//     }

//     char *input_file_name = "t3.txt";

//     generate_grammar(fp);
//     //test_grammar();
//     init_first_and_follow_sets();
//     startSymbol = (Symbol *)malloc(sizeof(Symbol));
//     startSymbol->is_terminal = 0;
//     startSymbol->nt = program;

//     //printFirstSet();
//     //printFollowSet();
//     char* parse_output="parse_output_file.txt";
//     FILE* fptr=fopen(parse_output,"w+");

//     ParsingTable *ptable = initialise_parse_table();
//     createParseTable(g, set->first_set, set->follow_set, ptable);
//     //print_parse_table(ptable);

//     int* syntxerr = malloc(sizeof(int));
//     ParseTree *ptree = parseInputSourceCode(input_file_name, ptable, syntxerr, g,fptr);
//     // printf("Back To main\n");
//     // printf("%s\n", NT_HashMap[ptree->root->childHead->next->symbol->nt]);
//     char* parse_tree_file = "parse_tree_outpur_file.txt";
//     FILE* parse_file_ptr = fopen(parse_tree_file,"w+");
//     print_tree(ptree->root,parse_file_ptr);

//     // printf("%s\n",NT_HashMap[ptable->cells[arithmeticOrBooleanExpr][ID]->head->symbol->nt]);
//     if(*syntxerr == 0){
//         printf("\nParsing Completed Successfully \n");
//     }
//     else{
//         printf("\nError in Parsing \n");
//     }
//     fclose(fp);
// }