GCC=gcc -g -Wall -std=gnu99
LIBS=-lSDL2 -lpthread

all: test_management

test_management: test_management.o workers_management.o
	$(GCC) $^ -o $@ $(LIBS)

test_management.o: test_management.c
	$(GCC) $< -c

workers_management.o: workers_management.c
	$(GCC) $< -c

run: test_management
	./$<

clean:
	rm -f *.o test_management *~
