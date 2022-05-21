#ifndef _TREE_TO_HTML_H_
#define _TREE_TO_HTML_H_

#include <stdio.h>
#include "genericToAst.h"
#include "symbolTable.h"

// From AST tree to HTML
void treeToHTML(GenericNode *program, FILE *file, SymbolTable * table);

#endif