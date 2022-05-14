#include "include/exp-to-ast.h"
#include "include/ast-to-html.h"

#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME_LENGTH 256

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE *output;

void yyerror(ExpNode **program, char *s);

void tree_to_html(ExpNode *program, FILE *file)
{
    output = file;
    if(program->mode == STRING_MODE){
        P("%s",program->evaluateString(NULL, program));
    }else{ // INTEGER_MODE
        P("%d",program->evaluateInteger(NULL, program));
    }
}
