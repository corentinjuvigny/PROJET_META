#----------------------------------------------------------------------
# Copyright (c) 2019-2020, Adrien BLASSIAU and Corentin JUVIGNY

# Permission to use, copy, modify, and/or distribute this software
# for any purpose with or without fee is hereby granted, provided
# that the above copyright notice and this permission notice appear
# in all copies.

# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
# WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
# AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#----------------------------------------------------------------------


#----------------------------------------------------------------------
#           GLOBAL MAKEFILE, NOT TO BE MODIFIED
#----------------------------------------------------------------------

include Makefile.options

PARAMETERS=${FILE_NAME} ${COMMUNICATION_RADIUS} ${CAPTURE_RADIUS} ${SIZE} ${PHI} ${STEP} ${T_INIT} ${NB_ITERATIONS} ${G_TIME}

CC=gcc -Wall -Wextra -ansi -std=c99 -O2 -pthread -lm -I/usr/include/${PYTHON_VERSION} -l${PYTHON_VERSION} -DDEBUG=${DEBUG} -DGRAPHIC=${GRAPHIC} -DFILE_MODE=${FILE_MODE}

all : main

run :
	make && ./bin/main ${PARAMETERS}

run_test :
	make test && ./bin/test

run_free :
	make main && valgrind --track-origins=yes ./bin/main ${PARAMETERS}

test : main_test.o test_unit.o point.o tools.o rwfile.o dist.o greedy.o avl.o queue.o draw.o annealing.o neighbourhood.o kdtree.o dfs.o test_unit.o
	cd obj/ && $(CC) $^ -o ../bin/$@ -lm -I/usr/include/${PYTHON_VERSION} -l${PYTHON_VERSION} -lcunit

main : main.o point.o tools.o rwfile.o dist.o greedy.o avl.o queue.o draw.o annealing.o neighbourhood.o kdtree.o dfs.o check.o
	cd obj/ && $(CC) $^ -o ../bin/$@ -lm -I/usr/include/${PYTHON_VERSION} -l${PYTHON_VERSION} -lcunit

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

check.o : src/check.c src/check.h src/include.h
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
