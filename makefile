# Makefile for signals exercise

all: main

main: signal.c
	gcc -o main signal.c

clean:
	rm -f *.o main

runs:
	./main