#To run, put this file together with lexer.h, and lexer.c
#in the same directory. Run "make". Then the executable
#is "schemer," which just takes a line of input and
#breaks it up into tokens.
scheme: parsertester2.o parser2.o  lexer.o
	gcc -o schemer parsertester2.o parser2.o lexer.o

parsertester2.o: parsertester2.c
	gcc -c parsertester2.c

parser2.o: parser2.c
	gcc -c parser2.c

lexer.o: lexer.c
	gcc -c lexer.c

clean:
	rm -f *~ *.o *.a

#^^^^^^This space must be a TAB!!.
