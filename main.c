#include "include/exp-to-ast.h"
#include "include/ast-to-html.h"
#include <getopt.h>

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse(ExpResultNode **program);
extern int yylineno;

extern FILE *yyin;
FILE *out;
ExpResultNode *program;
char *out_file = "program";

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char **argv)
{
    // Option parsing
    int c;
    while (1)
    {
        c = getopt(argc, argv, "o:");
        if (c == -1)
            break;
        if (c == 'o')
        {
            out_file = optarg;
        }
    }
    if (optind >= argc)
    {
        printf("Reading program from STDIN\n");
    }
    else if (optind < argc - 1)
    {
        if (optind < argc - 1)
        {
            fprintf(stderr, "Usage: %s [-o] [in.file]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        yyin = fopen(argv[optind], "r");
        if (yyin == NULL)
        {
            perror("File can not be opened");
            exit(EXIT_FAILURE);
        }
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


    // Llevado del AST a un html
    out = fopen("out.html", "w+");
    if (out == NULL)
    {
        perror("Error creating auxiliary file");
        exit(EXIT_FAILURE);
    }

    fprintf(out, "<!DOCTYPE html>\n");

    tree_to_html(program, out);
    fclose(out);

    printf("\nEnd\n");
}

void yyerror(ExpResultNode **param, char * err_msg) {
    printf("%s at line %d\n", err_msg, yylineno);
}