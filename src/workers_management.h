#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef _WORKERS_MANAGEMENT_H_
#define _WORKERS_MANAGEMENT_H_

typedef struct sync_st {
	sem_t sem_escape;
	pthread_barrier_t workers_barrier;
	sem_t sem_workers;
	pthread_mutex_t compute_nb_mutex;
	bool escape_pressed;
	int compute_nb;
} sync_t;

typedef struct square_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} square_t;

typedef struct board_st {
	square_t** matrix;
	int width;
	int height;
} board_t;

typedef struct worker_st {
	board_t* board;
	int workers_nb;
	int id;
	sync_t* sync;
} worker_t;

worker_t* workers_init(int workers_nb, int width, int height, int seed, int prob);
void workers_free(worker_t* workers);
void update_neighbours(square_t** matrix, int x, int y);
void print_board(board_t* board);

#endif
