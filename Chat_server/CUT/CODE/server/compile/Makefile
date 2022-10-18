SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
DATA = ../data
TESTDIR=../test
RM = rm * .o

INCLUDEFLAGS = -I../include
CFLAGS = -c -g -Wall ${INCLUDEFLAGS}
all:
	gcc ${CFLAGS} ${SRCDIR}/server.c -o ${OBJDIR}/server.o
	gcc ${OBJDIR}/server.o  -o ${BINDIR}/server.exe 
clean:
	rm ${OBJDIR}/server.o  ${BINDIR}/server.exe
