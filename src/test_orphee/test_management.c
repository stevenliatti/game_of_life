#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "workers_management.h"

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

const int workers_nb = 2;
const int width = 10;
const int height = 10;

void* work(void* arg) {
	return NULL;
}

int main(int argc, char** argv) {
	pthread_t t[workers_nb];
	worker_t workers[workers_nb];
	workers_init(workers,workers_nb,width,height);
	print_board(workers->board);
	printf("%f\n", 1e6);
	
	for (int i = 0; i < workers_nb; i++) {
		CHECK_ERR(pthread_create(&t[i], NULL, work, &workers[i]), "pthread_create failed!");
	}
	for (int i = 0; i < workers_nb; i++) {
		CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
	}
	
	workers_free(workers);
	return EXIT_SUCCESS;
}
