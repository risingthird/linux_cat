SHELL:=/bin/bash -O extglob
objects = hw1a.c hw1b.c microcat.c


all: $(objects)
	@gcc hw1a.c -o hw1a && gcc hw1b.c -o hw1b && gcc microcat.c -o microcat

hw1a : hw1a.c
	@gcc hw1a.c -o hw1a

hw1b: hw1b.c
	@gcc hw1b.c -o hw1b

microcat: microcat.c
	@gcc microcat.c -o microcat

clean:
	@rm -f !(*.c|Makefile|README.md)
