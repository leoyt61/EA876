CC = gcc
CFLAGS = -Wextra -lfl
DIR = src
FILENAME = $(DIR)/main.c
YYTABH = $(DIR)/y.tab.h
YYTABC = $(DIR)/y.tab.c
LEXOUT = $(DIR)/lex.yy.c
YACCFILE = $(DIR)/main.y
LEXFILE = $(DIR)/main.l
TARGET = ./main
BJS = $(SRCS:.c=.o)
YACC = bison
LEX = flex


TEST_SCRIPT = assemble.sh

ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = trabalho1.zip
EXTENSIONS = *.c *.h *.in *.out *.sh *.l *.y *.md *.pdf

.PHONY: depend clean

all:$(TARGET)

$(TARGET):$(LEXOUT) $(YYTABC)
	$(CC) -o$(TARGET) $(LEXOUT) $(YYTABC) $(CFLAGS) -lm

$(LEXOUT):$(LEXFILE) $(YYTABC)
	$(LEX) -o$(LEXOUT) $(LEXFILE)

$(YYTABC):$(YACCFILE)
	$(YACC) -o$(YYTABC) -dy $(YACCFILE)

test:all
	./$(TEST_SCRIPT)

zip:clean
	$(ZIP) -R $(ZIPFILE)  Makefile $(EXTENSIONS)

clean:
	$(RM) $(YYTABC)
	$(RM) $(YYTABH)
	$(RM) $(LEXOUT)
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o
	$(RM) ./$(ZIPFILE)
	$(RM) saidas/*
