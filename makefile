# makefile for assignment 4

# Link the output file to an executable
assgn4: assgn4.o
	gcc -g -o assgn4 assgn4.o -lpthread

# Compile the C file
assgn.o: assgn.c
	gcc -g -o assgn4.o assgn4.c -lpthread

# Clean up command
clean:
	rm *.o
	rm assgn4
