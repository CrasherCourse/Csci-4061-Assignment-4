# a makefile for assignment 4

assgn4: assgn4.o
	gcc -g -o assgn4 assgn4.o -lpthread

assgn.o: assgn.c
	gcc -g -o assgn4.o assgn4.c -lpthread

clean:
	rm *.o
	rm assgn4
