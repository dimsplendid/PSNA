############################################
# makefile for my test OOP structure       #
# 2017 04 01, Happy Fool's Day XD          #
# Tseng Wei-Hsiang <dimsplendid@gmail.com> #
############################################

CC = gcc -Wall
CFLAG = -c
OPTFLAG =
DEBUGFLAG = -g
LIBDIR = 
EXTRALIBS = -lgsl -lgslcblas -lm
INCLUDEDIR = 

EXEC = tree.run
OBJECTS = tree.o

all: $(EXEC) $(OBJECTS)

%.o: %.c %.h
	$(CC) -c $(CFLAG) $(INCLUDEDIR) $< -o $@

$(EXEC): main.c $(OBJECTS)
	$(CC)  tree.o $< -o $@ $(LIBDIR) $(EXTRALIBS)

test_list: list.c 
	$(CC) $(DEBUGFLAG) -DMAIN $< -o $@.run

test_tree: tree.c list.o
	$(CC) $(DEBUGFLAG) -DMAIN list.o $< -o $@.run

clean: 
	rm -f *.o *.run
