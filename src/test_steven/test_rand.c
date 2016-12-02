#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int main() {
	srand(time(NULL));

	printf("%d\n", rand() % 2);
}