TWS	= ../..
TWSBIN	= $(TWS)/bin
TWSPARSER	= $(TWS)/parser
INCLUDE	= $(TWS)/parser
CC      = gcc
CCFLAGS	= -c -I$(INCLUDE)
# These options are for the libraries to link in depending on which version
# of flex and yacc you are using.
#
# Turn this on for Linux machines.
# LOCAL_LIBRARIES = -lfl
# Turn this on for CIS Sun machines.
# LOCAL_LIBRARIES = -lfl -ll -ly
# Turn this on for OSX machines.
LOCAL_LIBRARIES = -ll -ly

parse:		$(TWSPARSER)/Main.o $(TWSPARSER)/GenericTrees.o \
		y.tab.o lex.yy.o $(TWSPARSER)/Trees.o \
		$(TWSPARSER)/DLists.o
		$(CC) -o parse -g -ansi $(TWSPARSER)/Main.o \
		$(TWSPARSER)/GenericTrees.o y.tab.o lex.yy.o \
		$(TWSPARSER)/Trees.o $(TWSPARSER)/DLists.o $(LOCAL_LIBRARIES)
		chmod 750 parse

y.tab.o:	y.tab.c
		$(CC) $(CCFLAGS) y.tab.c

y.tab.h y.tab.c:	code.y $(TWSPARSER)/Tokenizer.h \
			$(TWSPARSER)/Trees.h 
		yacc -dv code.y

code.y:	parse.tiny
		$(TWSBIN)/pgen < parse.tiny > /dev/null \
		2>/dev/null || exit 1
#		$(TWSBIN)/pgen < parse.tiny || exit 1 
#	USE THIS LAST COMMAND IF YOU WANT DEBUGGING TRACES
		chmod 640 parse.tiny

lex.yy.o:	lex.yy.c
		$(CC) $(CCFLAGS) lex.yy.c

lex.yy.c:	lex.tiny y.tab.h $(TWSPARSER)/Tokenizer.h
		flex lex.tiny
		chmod 640 lex.tiny

clean::
		rm -f y.tab.c y.tab.h y.output lex.yy.o lex.yy.c y.tab.o
		rm -f core _TREE code.y parse
