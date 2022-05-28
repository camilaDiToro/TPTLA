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
	state.succeed = TRUE;
    state.errorManager = newErrorManager();

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
            if(!state.succeed){
                printf("Error found building the AST. The output was not generated.\n");
                showErrors(state.errorManager);
                freeErrorManager(state.errorManager);
				return EXIT_FAILURE;
            }
            generator();
			if (state.succeed) {
                printf("The HTML file was generated sucessfuly \n");
			}
			else {
                if(state.errorManager->errorCount != 0){
                    showErrors(state.errorManager);
                    freeErrorManager(state.errorManager);
				    return EXIT_FAILURE;
                }
				printf("There was an error in the application \n");
				return EXIT_FAILURE;
			}
			break;
		case 1:
			printf("Bison finalized due to a syntax error.\n");
            freeErrorManager(state.errorManager);
			return EXIT_FAILURE;
		case 2:
			printf("Bison finalized due to a memory problem.\n");
            freeErrorManager(state.errorManager);
            return EXIT_FAILURE;
		default:
			printf("Unknoen error while executing Bison (code %d).\n", result);
            freeErrorManager(state.errorManager);
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