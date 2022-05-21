#include "include/expToAst.h"
#include "include/astToHTML.h"
#include <getopt.h>

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse(GenericNode **program);
extern int yylineno;

extern FILE *yyin;
FILE *out;
GenericNode *program;
char *out_file = "program";

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char **argv)
{
    yyin = fopen(argv[optind], "r");
    if (yyin == NULL) {
        perror("File can not be opened");
        exit(EXIT_FAILURE);
    }
    

    // Evaluacion semantica y construccion de AST
    // try to modify this, its not doing whats expected to be done
    printf("Compilando...\n");
	const int result = yyparse(&program);
	switch (result) {
		case 0:
			//if (state.succeed) {
				printf("La compilacion fue exitosa. \n");
				//Generator(state.result);
			//}
			//else {
			//	printf("Se produjo un error en la aplicacion.");
			//	return -1;
			//}
			break;
		case 1:
			printf("Bison finalizo debido a un error de sintaxis.\n");
			return EXIT_FAILURE;
		case 2:
			printf("Bison finalizo abruptamente debido a que ya no hay memoria disponible.\n");
			break;
            return EXIT_FAILURE;
		default:
			printf("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).\n", result);
            return EXIT_FAILURE;
    }


    // AST to html
    out = fopen("out.html", "w+");
    if (out == NULL) {
        perror("Error creating auxiliary file");
        exit(EXIT_FAILURE);
    }
    
    // Adding arguments as variables
    // All the arguments are considered as strings
    SymbolTable * table = newEmptySymbolTable();
    char argument[12];
    for(int i=0 ; i<argc ; ++i) { 
        sprintf(argument, "argv[%d]", i);
        addSymbolToTable(table, newSymbol(argument, argv[i], STRING));
    }

    //Print headers
    fprintf(out, "<!DOCTYPE html>\n");

    treeToHTML(program, out, newScope(table));
    fclose(out);

    printf("\nEnd\n");
}

void yyerror(GenericNode **param, char * err_msg) {
    printf("%s at line %d\n", err_msg, yylineno);
}