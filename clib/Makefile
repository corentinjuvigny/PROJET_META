CC=gcc -Wall -Wextra -ansi -std=c99 -O2 -pthread -lm -lkdtree

all : main

test : main.o point.o tools.o rwfile.o
	$(CC) $^ -o main -g

main : main.o point.o tools.o rwfile.o
	$(CC) $^ -o main

main.o : main.c
	$(CC) -c $< -o $@

point.o : point.c point.h include.h
	$(CC) -c $< -o $@

tools.o : tools.c tools.h include.h
	$(CC) -c $< -o $@

rwfile.o : rwfile.c rwfile.h include.h
	$(CC) -c $< -o $@

.PHONY: clean
clean :
	rm -f *.o
