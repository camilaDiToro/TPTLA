#include "include/expToAst.h"
#include "include/astToHTML.h"
#include "include/shared.h"
#include <getopt.h>

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

CompilerState state;

static void generator(void);

int main(int argc, char **argv) {
    yyin = fopen(argv[optind], "r");
    if (yyin == NULL) {
        perror("File can not be opened");
        exit(EXIT_FAILURE);
    }

    //Initialize the state of the compiler
    state.program = NULL;
	state.succeed = FALSE;
    state.undefinedVariables = FALSE;

    // Adding arguments as variables, all the arguments are considered as strings
    state.table = newEmptySymbolTable();
    char argument[12];
    for(int i=0 ; i<argc ; ++i) { 
        sprintf(argument, "argv[%d]", i);
        addSymbolToTable(state.table, newSymbol(argument, argv[i], STRING));
    }

    // Semantic evaluation and AST building
    printf("Compiling...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
            generator();
			if (state.succeed) {
                printf("The HTML file was generated sucessfuly \n");
			}
			else {
                if(state.undefinedVariables){
                    printf("There were undefined variables in the JSON \n");
				    return EXIT_FAILURE;
                }
				printf("There was an error in the application \n");
				return EXIT_FAILURE;
			}
			break;
		case 1:
			printf("Bison finalized due to a syntax error.\n");
			return EXIT_FAILURE;
		case 2:
			printf("Bison finalizo abruptamente debido a que ya no hay memoria disponible.\n");
			break;
            return EXIT_FAILURE;
		default:
			printf("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).\n", result);
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;   
}

void yyerror(char * err_msg) {
    printf("%s at line %d\n", err_msg, yylineno);
}

void generator(void) {
    //Open output file
    yyout = fopen("out.html", "w+");
    if (yyout == NULL) {
        perror("Error creating auxiliary file");
        exit(EXIT_FAILURE);
    }
    
    //Print headers
    fprintf(yyout, "<!DOCTYPE html>\n");

    treeToHTML(state.program, yyout, newScope(state.table));
    fclose(yyout);
}