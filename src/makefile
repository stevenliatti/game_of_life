GCC=gcc -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2 -lpthread

all: gameoflife

gameoflife: gameoflife.o gfx.o workers_management.o workers_compute.o keyboard_interrupt.o display_board.o
	$(GCC) $^ -o $@ $(LIBS)

gameoflife.o: gameoflife.c
	$(GCC) $< -c

gfx.o: gfx.c
	$(GCC) $< -c

workers_management.o: workers_management.c
	$(GCC) $< -c

workers_compute.o: workers_compute.c
	$(GCC) $< -c

keyboard_interrupt.o: keyboard_interrupt.c
	$(GCC) $< -c
	
display_board.o: display_board.c
	$(GCC) $< -c

run: gameoflife
	./$<

clean:
	rm -f *.o gameoflife *~

rebuild: clean all
