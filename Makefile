CC = gcc

all: main

main: main.c
	$(CC) -o $@ $^ -lSDL2

.PHONY: run
run: main
	./$<

.PHONY: clean
clean:
	$(RM) main
