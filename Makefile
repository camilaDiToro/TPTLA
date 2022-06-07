CCFLAGS = -Wall -Wno-unused-function -std=c99 -g -Wdiv-by-zero -fsanitize=address 
YLFLAGS = -lfl
MACYLFLAGS = -ll
SRC_FOLDER = .
SOURCES_C=$(wildcard $(SRC_FOLDER)/*.c)
SOURCES_OBJ=$(SOURCES_C:%.c:%.o)
PARSER_OBJ = lex.yy.o y.tab.o
PROGRAM_OBJ = program aux.c
OBJ = lex.yy.c y.tab.c y.tab.h

INSTALL_PATH = ~/.jtoh

all: parser
parser:
	lex $(SRC_FOLDER)/lang.l
	yacc -d $(SRC_FOLDER)/lang.y
	$(CC) $(CCFLAGS) $(SOURCES_C) lex.yy.c y.tab.c $(YLFLAGS) -o jtoh
	rm -rf $(OBJ)

#%.o:%.c
#	$(CC) $(CCFLAGS) $(YLFLAGS) -c $< -o $@

clean: 
	rm -rf $(PARSER_OBJ) $(PROGRAM_OBJ) $(OBJ) jtoh

install: all
	mkdir -p $(INSTALL_PATH) 
	mv jtoh $(INSTALL_PATH)/jtoh
	make clean
	cp jtoh.1 /usr/local/man/man1/jtoh.1
	alias jtoh="~/.jtoh/jtoh"

uninstall: clean
	rm -rf $(INSTALL_PATH) 
	rm /usr/local/man/man1/jtoh.1


check:
	mkdir check
	cppcheck --quiet --enable=all --force --inconclusive . 2> ./check/cppout.txt

PHONY = all clean check