#Makefile for SIL compiler
#Maximilian Hasselbusch
#Last Updated: 16 January 2016

OBJ = token.o lexer.o parser.o silc.o
OPTS = -g -c -Wall -Werror

parser: $(OBJ)
	g++ -o silc $(OBJ)

token.o: token.cc token.h
	g++ $(OPTS) token.cc

lexer.o: lexer.cc lexer.h
	g++ $(OPTS) lexer.cc

parser.o: parser.cc parser.h
	g++ $(OPTS) parser.cc

silc.o: silc.cc parser.o
	g++ $(OPTS) silc.cc

clean:
	rm -f *.o silc *~