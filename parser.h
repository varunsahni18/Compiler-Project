/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#ifndef PARSER
#define PARSER
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parserDef.h"
// #include "our_tree.c"

int get_terminal_index(char *s);
int get_nonterminal_index(char *s);

Symbol *new_symbol(char *s, bool is_terminal);
SymbolNode *new_symbol_node(char *s, bool is_terminal);
Rule *new_rule(NonTerminal nt, int ruleNo);
Rules *new_rules(NonTerminal nt);
int driver_load_grammar_func();
void generate_grammar(FILE *fp);
void test_grammar();
ParseTree* driver_parse_function(char* input_file_name, char* parse_tree_file_output);

#endif
