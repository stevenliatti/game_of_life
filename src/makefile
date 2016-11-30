GCC=gcc -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2 -lpthread

all: gameoflife

gameoflife: gameoflife.o gfx.o workers_management.o
	$(GCC) $^ -o $@ $(LIBS)

gameoflife.o: gameoflife.c
	$(GCC) $< -c

gfx.o: gfx.c
	$(GCC) $< -c

workers_management.o: workers_management.c
	$(GCC) $< -c

run: gameoflife
	./$<

clean:
	rm -f *.o gameoflife *~
