
# makefile for Assembler4

TARGET=AssemblerProjet


OBJFILES =	funcBasic.o funcFirstRound.o funcSecondRoun.o main.o funcSyntax.o 

HEDER = 	definitions.h 


cc =gcc
CFLAGS =-g -ansi -Wall -pedantic
CFLAGS2 =-c -ansi -Wall -pedantic
LDFLAGS =-o



$(TARGET): $(OBJFILES) $(HEDER) 
	$(cc) $(CFLAGS) $(OBJFILES) $(LDFLAGS) $@


%.o: %.c %.h
	$(cc) $(CFLAGS2) $*.c $(LDFLAGS) $@ 







