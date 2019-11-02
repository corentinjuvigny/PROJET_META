CC=gcc -Wall -Wextra -ansi -std=c99 -O2 -pthread -lm -I/usr/include/python3.6m -lpython3.6m

all : main

run :
	make && ./bin/main
run_test :
	make test && ./bin/test

test : main_test.o test_unit.o point.o tools.o rwfile.o dist.o greedy.o avl.o queue.o draw.o annealing.o neighbourhood.o kdtree.o dfs.o test_unit.o
	cd obj/ && $(CC) $^ -o ../bin/$@ -lm -I/usr/include/python3.6m -lpython3.6m -lcunit

main : main.o point.o tools.o rwfile.o dist.o greedy.o avl.o queue.o draw.o annealing.o neighbourhood.o kdtree.o dfs.o
	cd obj/ && $(CC) $^ -o ../bin/$@ -lm -I/usr/include/python3.6m -lpython3.6m

main.o : src/main.c
	$(CC) -c $< -o obj/$@

old_main.o : src/old_main.c
	$(CC) -c $< -o obj/$@

point.o : src/point.c src/point.h src/include.h
	$(CC) -c $< -o obj/$@

tools.o : src/tools.c src/tools.h src/include.h
	$(CC) -c $< -o obj/$@

rwfile.o : src/rwfile.c src/rwfile.h src/include.h
	$(CC) -c $< -o obj/$@

dist.o : src/dist.c src/dist.h src/include.h
	$(CC) -c $< -o obj/$@

greedy.o : src/greedy.c src/greedy.h src/include.h
	$(CC) -c $< -o obj/$@

avl.o : src/avl.c src/avl.h src/include.h
	$(CC) -c $< -o obj/$@

queue.o : src/queue.c src/queue.h src/include.h
	$(CC) -c $< -o obj/$@

draw.o : src/draw.c src/draw.h src/include.h
	$(CC) -c $< -o obj/$@

annealing.o : src/annealing.c src/annealing.h src/include.h
	$(CC) -c $< -o obj/$@

neighbourhood.o : src/neighbourhood.c src/neighbourhood.h src/include.h
	$(CC) -c $< -o obj/$@

kdtree.o : src/kdtree.c src/kdtree.h src/include.h
	$(CC) -c $< -o obj/$@

dfs.o : src/dfs.c src/dfs.h src/include.h
	$(CC) -c $< -o obj/$@

main_test.o : test/main_test.c
	$(CC) -c $< -o obj/$@

test_unit.o : test/test_unit.c src/include.h
	$(CC) -c $< -o obj/$@

doxygen : doc/Doxyfile
	cd doc && doxygen ../$<

.PHONY: clean
clean :
	rm -f obj/*
	rm -rf doc/html/*
	rm -rf doc/latex/*
	rm -rf bin/*
