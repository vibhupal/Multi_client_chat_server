SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
DATA = ../data
TESTDIR=../test
RM = rm * .o

INCLUDEFLAGS = -I../include
CFLAGS = -c -g -Wall ${INCLUDEFLAGS}
all:
	gcc ${CFLAGS} ${SRCDIR}/client.c -o ${OBJDIR}/client.o
	gcc ${OBJDIR}/client.o -o ${BINDIR}/client.exe 
clean:
	rm ${OBJDIR}/client.o  ${BINDIR}/client.exe
