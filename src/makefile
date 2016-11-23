GCC=gcc -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2 -lpthread

gameoflife: gameoflife.o gfx.o
	$(GCC) $^ -o $@ $(LIBS)

gameoflife.o: gameoflife.c gfx.h
	$(GCC) $< -c

gfx.o: gfx.c gfx.h
	$(GCC) $< -c

run: gameoflife
	./$<

clean:
	rm -f *.o gameoflife *~
