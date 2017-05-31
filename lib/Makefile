############################################
# makefile for my test OOP structure       #
# 2017 04 01, Happy Fool's Day XD          #
# Tseng Wei-Hsiang <dimsplendid@gmail.com> #
############################################

CC = gcc -Wall
CFLAG = 
OPTFLAG =
DEBUGFLAG = -g
LIBDIR = 
EXTRALIBS = -lgsl -lgslcblas -lm
INCLUDEDIR = 

OBJECTS = object.o parser.o params.o list.o tree.o

all: $(OBJECTS)

%.o: %.c %.h
	$(CC) -c $(CFLAG) $(INCLUDEDIR) $< -o $@ $(EXTRALIBS)

test_list: list.c 
	$(CC) $(DEBUGFLAG) -DMAIN $< -o $@.run $(EXTRALIBS)

test_tree: tree.c list.o
	$(CC) $(DEBUGFLAG) -DMAIN list.o $< -o $@.run

test_parser: parser.c
	$(CC) $(DEBUGFLAG) -DMAIN $< -o $@.run

test_params: params.c parser.o aux.o
	$(CC) $(DEBUGFLAG) -DMAIN parser.o aux.o $< -o $@.run $(EXTRALIBS)

clean: 
	rm -f *.o *.run
