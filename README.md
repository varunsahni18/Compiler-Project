# Compiler Construction

A complete compiler for a custom strongly-typed programming language, built from scratch in **~6,000 lines of C** as part of the Compiler Construction course (CS F363) at BITS Pilani (Spring 2023).

The compiler implements all major phases from raw source to validated intermediate representation: lexical analysis, syntax analysis, AST construction, and semantic analysis with type checking.

---

## Architecture

```
Source File --> [Lexer: 51-state DFA, twin-buffer I/O] --> Token Stream
                                                              |
                                                              v
            [LL(1) Parser: 141 rules, auto FIRST/FOLLOW] --> Parse Tree
                                                              |
                                                              v
                    [AST Builder: 70% node reduction] --> Abstract Syntax Tree
                                                              |
                                                              v
              [Symbol Table + Type Checker + Scope Resolution] --> Validated IR
```

---

## Compilation Phases

### 1. Lexical Analysis (lexer.c, 1389 lines)

- **51-state DFA** for tokenizing identifiers, keywords, integer/real literals, operators, and comments
- **Twin-buffer I/O** with 64-byte blocks: never holds the full source file in memory; swaps buffers on boundary crossings
- Handles multi-character operators (<=, >=, !=, :=, ..) and single-line comments
- Tracks line numbers for error reporting; identifier length capped at 20 characters

### 2. Syntax Analysis (parser.c, 1011 lines)

- **LL(1) predictive parser** driven by a computed parsing table (72 non-terminals x 57 terminals)
- Reads **141 grammar rules** from grammar.txt and auto-generates **FIRST and FOLLOW sets** at startup
- Builds a full parse tree using an explicit stack (no recursive descent)
- **Panic-mode error recovery**: on syntax error, synchronizes on the FOLLOW set of the current non-terminal

### 3. AST Construction (ast.c, 1855 lines)

- Transforms the verbose parse tree into a compact **Abstract Syntax Tree**
- **~70% node count reduction**: eliminates epsilon nodes, grammar-sugar chains, and redundant wrappers
- 40+ AST node types (AST_PROGRAM, AST_MODULE, AST_ASSIGNMENT, AST_ITERATIVE_WHILE, AST_CONDITIONAL_SWITCH, etc.)
- Inherited and synthesized attribute propagation during construction

### 4. Semantic Analysis (symbolTable.c, 1386 lines)

- **Scoped symbol table** with hash-map buckets and chaining (O(1) average lookup)
- Tracks per-variable: lexeme, module, scope range, data type, array bounds, memory width/offset, nesting level
- **Type checking**: assignment compatibility, array index bounds, function argument type/count matching, redeclaration detection
- Supports **nested scopes** via nesting-level tracking; reports errors with line numbers

---

## The Language

| Feature | Syntax Example |
|---------|---------------|
| Primitive types | integer, real, boolean |
| Arrays | declare a: array[2..20] of integer; |
| Modules (functions) | <<module foo>> takes input [...]; returns [...]; start ... end |
| Driver module | <<driverDef>> ... start ... end |
| Switch-case | switch(x) { case 1: ... break; default: ... } |
| For loops | for (i IN 1..10) start ... end |
| While loops | while (condition) start ... end |
| Boolean ops | AND, OR, relational comparisons |
| I/O | get_value(x), print(x) |

Full grammar: [grammar.txt](grammar.txt) (141 productions)

---

## Key Results

| Metric | Value |
|--------|-------|
| Total source | ~6,000 lines of C |
| DFA states | 51 |
| Grammar rules | 141 (LL(1) conflict-free) |
| Non-terminals / Terminals | 72 / 57 |
| AST memory savings | ~70% vs. full parse tree |
| Error recovery | Panic-mode (syncs on FOLLOW sets) |
| Buffer | Twin-buffer, 64-byte blocks |

---

## Build and Run

```
make compile
./a.out t10.txt
```

Interactive menu:
- 0: Exit
- 1: Lexer: print token list
- 2: Parser: build parse tree
- 3: AST: construct abstract syntax tree
- 4: AST memory stats (node count, compression ratio)
- 5: Symbol table: build and display
- 6: Semantic analysis: type check and report errors

---

## Project Structure

```
driver.c              Main entry, interactive menu
lexer.c / lexer.h     DFA-based twin-buffer lexer (1,389 LOC)
lexerDef.h            Token type definitions
parser.c / parser.h   LL(1) parser with FIRST/FOLLOW (1,011 LOC)
parserDef.h           Parse table structures (72 NT x 57 T)
ast.c / ast.h         Parse-tree to AST transformation (1,855 LOC)
astDef.h              40+ AST node type enum
symbolTable.c/.h      Scoped symbol table + type checker (1,386 LOC)
symbolTableDef.h      SymbolTableEntry struct
stack.c / stack.h     Explicit stack for predictive parsing
tree.c / tree.h       Parse tree node structure
hashTable.h           Hash map for keyword lookup
grammar.txt           Full LL(1) grammar (141 rules)
makefile              Build command
t10.txt - t19.txt     Test programs
```

---

## Design Decisions

1. **Twin-buffer over full-file read**: The lexer uses two alternating 64-byte buffers. When forward_ptr crosses a boundary, only the back buffer is refilled from disk. Handles arbitrarily large source files in constant memory.

2. **Table-driven over recursive descent**: The LL(1) table is computed from grammar.txt at startup. Grammar changes only require editing the text file, not rewriting code.

3. **AST as a separate pass**: Rather than building the AST inline during parsing (coupling grammar to AST logic), we first build a full parse tree and then transform. This made grammar iteration painless.

4. **Hash-table symbol lookup**: Both the lexer (keyword detection) and symbol table use hash maps with chaining for O(1) average-case lookup.

5. **Explicit stack over recursion**: The parser uses a manually managed stack, avoiding stack-overflow on deeply nested programs and making the parsing state inspectable for error recovery.

---

## Test Cases

t10.txt through t19.txt cover:
- Variable declarations (primitives and arrays)
- Nested modules with input/output parameters
- Arithmetic and boolean expressions
- Control flow (for, while, switch-case)
- Semantic errors (redeclaration, type mismatch, undeclared)
- Array bound checking (static bounds)
- Multi-module programs with inter-module calls

---

## Team

| Name | ID |
|------|-----|
| Varun Sahni | 2020A7PS0144P |
| Shreyas Ketkar | 2020A7PS0075P |
| Mridul Chandak | 2020A7PS0133P |
| Kushagra Sahni | 2020A7PS0974P |
| Chirag Maheshwari | 2020A7PS0983P |

## Course

CS F363 - Compiler Construction, BITS Pilani, Pilani Campus (Spring 2023)
