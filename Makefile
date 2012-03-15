all:
	yacc -d qc.y
	lex qc.l
	cc -g -o qc y.tab.c lex.yy.c -ly -ll
clean:
	rm -f *~ qc lex.yy.c y.tab.c y.tab.h

