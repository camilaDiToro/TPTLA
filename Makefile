CCFLAGS = -Wall -Wno-unused-function -std=c99 -g -Wdiv-by-zero #-fsanitize=address COMMENTED FOR VALGRIND USAGE
YLFLAGS = -lfl
MACYLFLAGS = -ll
SRC_FOLDER = .

PARSER_OBJ = gcan.o lex.yy.o y.tab.o
PROGRAM_OBJ = program aux.c
OBJ = lex.yy.c y.tab.c y.tab.h

all: parser
parser:
	lex $(SRC_FOLDER)/lang.l
	yacc -d $(SRC_FOLDER)/lang.y
	$(CC) $(CCFLAGS) $(SRC_FOLDER)/symbolTable.c $(SRC_FOLDER)/exp-to-ast.c $(SRC_FOLDER)/ast-to-html.c lex.yy.c y.tab.c $(SRC_FOLDER)/main.c $(YLFLAGS) -o main
	rm -rf $(OBJ)

clean: 
	rm -rf $(PARSER_OBJ) $(PROGRAM_OBJ) $(OBJ) main


PHONY = all clean