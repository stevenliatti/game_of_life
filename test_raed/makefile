GCC=gcc -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2 -lpthread

all: test

test: test.o
	$(GCC) $^ -o $@ $(LIBS)

test.o: test.c
	$(GCC) $< -c

run: test
	./$<

clean:
	rm -f *.o test *~
