/*
GROUP 14
2020A7PS0075P - SHREYAS KETKAR
2020A7PS0133P - MRIDUL CHANDAK
2020A7PS0144P - VARUN SAHNI
2020A7PS0974P - KUSHAGRA SAHNI
2020A7PS0983P - CHIRAG MAHESHWARI
*/

#include "lexer.h"
#include <stdbool.h>

typedef enum nonTerminal
{
    AnyTerm,
    K,
    N1,
    N3,
    N4,
    N5,
    N7,
    N8,
    N9,
    N_10,
    N_11,
    P1,
    U,
    actual_para_list,
    actual_para_list_dash,
    arithmeticExpr,
    arithmeticOrBooleanExpr,
    arrExpr,
    arrFactor,
    arrTerm,
    arr_N4,
    arr_N5,
    assignmentStmt,
    boolConstt,
    caseStmts,
    condionalStmt,
    dataType,
    deFault,
    declareStmt,
    driverModule,
    element_index_with_expressions,
    expression,
    factor,
    idListComma,
    id_num_rnum,
    index_arr,
    index_for_loop,
    input_plist,
    ioStmt,
    iterativeStmt,
    logicalOp,
    lvalueARRStmt,
    lvalueIDStmt,
    module,
    moduleDeclaration,
    moduleDeclarations,
    moduleDef,
    moduleReuseStmt,
    new_NT,
    new_index,
    new_index_for_loop,
    op1,
    op2,
    optional,
    otherModules,
    program,
    range_arrays,
    range_for_loop,
    relationalOp,
    ret,
    sign,
    sign_for_loop,
    simpleStmt,
    statement,
    statements,
    term,
    type,
    unary_op,
    value,
    var_id_num,
    var_print,
    whichStmt,

} NonTerminal;

typedef struct Symbol
{
    union
    {
        Terminal t;
        NonTerminal nt;
    };
    bool is_terminal;
} Symbol;

typedef struct SymbolNode
{
    Symbol *symbol;
    struct SymbolNode *next;
} SymbolNode;

typedef struct Rule
{
    NonTerminal LHS;
    SymbolNode *head;
    struct Rule *next;
    int length;
    int ruleNo;
} Rule;

typedef struct Rules
{
    NonTerminal LHS;
    Rule *head;
    int noOfRules;
} Rules;

typedef struct Grammar
{
    Rules **rules;
} Grammar;