#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "gfx.h"

#ifndef _WORKERS_MANAGEMENT_H_
#define _WORKERS_MANAGEMENT_H_

typedef struct sync_st {
	sem_t sem_escape;
	pthread_barrier_t workers_barrier;
	sem_t sem_display;
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
	int id;
	int workers_nb;
	double uperiod;
	board_t* board;
	sync_t* sync;
	square_t** asigned_squares;
	int asigned_squares_nb;
} worker_t;

worker_t* workers_init(int workers_nb, int width, int height, int seed, int prob, int freq);
void workers_free(worker_t* workers);
void update_neighbours(square_t** matrix, int x, int y);
void print_board(board_t* board);

#endif
