#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "gfx.h"

#ifndef _WORKERS_MANAGEMENT_H_
#define _WORKERS_MANAGEMENT_H_

/**
 * Struct that represent the mechanism of synchronization.
 *
 * @member sem_escape semaphore that wait for the graphical context to be built in "work" thread for the first time
 *			and notify "keypress_thread" that the graphical context is built and initialized
 * @member workers_barrier barrier to make sure that all the workers will start after the start of the timer and. Also, 
 *			threads will be blocked by this barrier and wait for display thread to be executed to resume their routines
 * @member sem_display semaphore that wait for the last worker thread to finish his treatment. If the current thread
 * 			is the last one it will allow display thread to be executed
 * @member compute_nb_mutex mutex to protect the workers counter
 * @member escape_pressed boolean true if the escape key is pressed
 * @member end_game boolean true if escape key is pressed and all workers and display thread finish their routines
 * @member compute_nb integer that contains the number of rhe workers that finish their routines
 */
typedef struct sync_st {
	sem_t sem_escape;
	pthread_barrier_t workers_barrier;
	sem_t sem_display;
	pthread_mutex_t compute_nb_mutex;
	bool escape_pressed;
	bool end_game;
	int compute_nb;
} sync_t;

/**
 * Struct that represent a square of the board
 *
 * @member x integer that contains abscissa value
 * @member y integer that contains ordinate value
 * @member is_alive boolean true if the square is alive in the current state
 * @member is_alive_past boolean true if the square is alive in the last state
 * @member nb_neighbours integer that contains the number of rhe the neighbours
 */
typedef struct square_st {
	int x;
	int y;
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} square_t;

/**
 * Struct that represent the board
 *
 * @member matrix of square
 * @member width integer that contains the matrix's width
 * @member height integer that contains the matrix's height
 */
typedef struct board_st {
	square_t** matrix;
	int width;
	int height;
} board_t;

/**
 * Struct that represent data that will be passed to threads
 *
 * @member id of the worker
 * @member workers_nb the number of workers
 * @member uperiod double that contains time for the display thread to wait
 * @member board a pointer to the board struct
 * @member sync a pointer to the sync struct
 * @member asigned_squares an array of pointers to squares of the board that will be treated by a worker
 * @member asigned_squares_nb integer that contains the number of the asigned_squares array
 */
typedef struct worker_st {
	int id;
	int workers_nb;
	double uperiod;
	board_t* board;
	sync_t* sync;
	square_t** assigned_squares;
	int assigned_squares_nb;
} worker_t;

worker_t* workers_init(int workers_nb, int width, int height, int seed, double prob, int freq);
void workers_free(worker_t* workers);
void update_neighbours(square_t** matrix, square_t* square);
void print_board(board_t* board);

#endif
