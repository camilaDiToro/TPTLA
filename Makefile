CCFLAGS = -Wall -Wno-unused-function -std=c99 -g -Wdiv-by-zero #-fsanitize=address COMMENTED FOR VALGRIND USAGE
YLFLAGS = -lfl
MACYLFLAGS = -ll
SRC_FOLDER = .

PARSER_OBJ = lex.yy.o y.tab.o
PROGRAM_OBJ = program aux.c
OBJ = lex.yy.c y.tab.c y.tab.h

all: parser
parser:
	lex $(SRC_FOLDER)/lang.l
	yacc -d $(SRC_FOLDER)/lang.y
	$(CC) $(CCFLAGS) $(SRC_FOLDER)/symbolTable.c $(SRC_FOLDER)/errorManager.c $(SRC_FOLDER)/jsonToAst.c $(SRC_FOLDER)/itoa.c $(SRC_FOLDER)/genericToAst.c $(SRC_FOLDER)/basicTypeToAst.c $(SRC_FOLDER)/expToAst.c $(SRC_FOLDER)/astToHTML.c lex.yy.c y.tab.c $(SRC_FOLDER)/main.c $(YLFLAGS) -o jtoh
	rm -rf $(OBJ)

clean: 
	rm -rf $(PARSER_OBJ) $(PROGRAM_OBJ) $(OBJ) jtoh


PHONY = all clean