/**
 * @file workers_management.c
 * @brief The management of the workers
 * 
 * This file contains the functions for the initialisation of the board, 
 * the workers and the synchonisation's structure and their destruction.
 * 
 * @author Steven Liatti
 * @author Orphée Antoniadis
 * @author Raed Abdennadher
 * @bug No known bugs.
 * @date December 7, 2016
 * @version 1.0
 */

#include "workers_management.h"

/**
 * This function alloc the memory for the board of squares.
 *
 * @param width the width of the board in pixels
 * @param height the height of the board in pixels
 * @return the constructed board
 */
board_t* board_alloc(int width, int height) {
	board_t* board = malloc(sizeof(board_t));
	assert(board != NULL);
	square_t* temp = malloc(sizeof(square_t)*width*height);
	assert(temp != NULL);
	board->width = width;
	board->height = height;
	board->matrix = malloc(sizeof(square_t*)*width);
	assert(board->matrix != NULL);
	for (int i = 0; i < width; i++) {
		board->matrix[i] = temp + height * i;
	}
	return board;
}

/**
 * This function update the neighbours number of the square in argument.
 * Each square has 8 neighbours to update.
 *
 * @param matrix the matrix of squares
 * @param square the square considered
 * @return void
 */
void update_neighbours(square_t** matrix, square_t* square) {
	square->nb_neighbours = 0;
	for (int i = square->x - 1; i <= square->x + 1; i++) {
		for (int j = square->y - 1; j <= square->y + 1; j++) {
			if (matrix[i][j].is_alive) {
				square->nb_neighbours++;
			}
		}
	}
	if (square->is_alive) {
		square->nb_neighbours--;
	}
}

/**
 * This function generate the game board. Foreach matrix's square of the board, 
 * 
 *
 * @param width the width of the board in pixels
 * @param height the height of the board in pixels
 * @param seed the an integer used to randomly populate the board
 * @param prob the probability of having a live cell during initialization
 * @return the constructed board
 */
board_t* board_gen(int width, int height, int seed, double prob) {
	srand(seed);
	board_t* board = board_alloc(width, height);
	int cnt = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board->matrix[i][j].x = i;
			board->matrix[i][j].y = j;
			if (i == 0 || i == width-1 || j == 0 || j == height-1){
				board->matrix[i][j].is_alive_past = false;
				board->matrix[i][j].is_alive = false;
			}
			else {
				float random = (float) rand() / (float) RAND_MAX;
				if (prob != 0 && random < prob){
					cnt++;
			 		board->matrix[i][j].is_alive_past = true;
			 		board->matrix[i][j].is_alive = true;
			 	}
				else{
					board->matrix[i][j].is_alive_past = false;
					board->matrix[i][j].is_alive = false;
				}
			}
		}
	}
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			update_neighbours(board->matrix,&(board->matrix[i][j]));
		}
	}
	return board;
}

void asigned_squares_gen(worker_t* workers) {
	int count = 0;
	int count2 = 0;
	for (int i = 1; i < workers->board->width - 1; i++) {
		for (int j = 1; j < workers->board->height - 1; j++) {
			workers[count % workers->workers_nb].asigned_squares[count2] = &(workers->board->matrix[i][j]);
			count++;
			if (count % workers->workers_nb == 0) {
				count2++;
			}
		}
	}
}

sync_t* sync_init(int workers_nb) {
	sync_t* sync = malloc(sizeof(sync_t));
	assert(sync != NULL);
	assert(sem_init(&(sync->sem_escape), 0, 0) == 0);
	assert(sem_init(&(sync->sem_display), 0, 0) == 0);
	assert(pthread_barrier_init(&(sync->workers_barrier), NULL, workers_nb + 1) == 0);
	assert(pthread_mutex_init(&(sync->compute_nb_mutex), NULL) == 0);
	sync->escape_pressed = false;
	sync->end_game = false;
	sync->compute_nb = 0;
	return sync;
}

worker_t* workers_init(int workers_nb, int width, int height, int seed, double prob, int freq) {
	worker_t* workers = malloc(sizeof(worker_t)*workers_nb);
	assert(workers != NULL);
	board_t* board = board_gen(width, height, seed, prob);
	sync_t* sync = sync_init(workers_nb);
	int squares_nb = (width - 2) * (height - 2);
	int asigned_squares_nb = squares_nb / workers_nb;;
	if (squares_nb % workers_nb != 0){
		asigned_squares_nb++;
	}

	for(int i = 0; i < workers_nb; i++) {
		workers[i].board = board;
		workers[i].id = i;
		workers[i].workers_nb = workers_nb;
		workers[i].sync = sync;
		workers[i].uperiod = (1.0 / freq) * 1e6;
		workers[i].asigned_squares = malloc(sizeof(square_t*) * asigned_squares_nb);
		assert(workers[i].asigned_squares != NULL);
		workers[i].asigned_squares_nb = asigned_squares_nb;
	}
	asigned_squares_gen(workers);
	return workers;
}

void workers_free(worker_t* workers) {
	free(workers->board->matrix[0]);
	free(workers->board->matrix);
	free(workers->board);
	pthread_barrier_destroy(&(workers->sync->workers_barrier));
	sem_destroy(&(workers->sync->sem_display));
	sem_destroy(&(workers->sync->sem_escape));
	pthread_mutex_destroy(&(workers->sync->compute_nb_mutex));
	free(workers->sync);
	for (int i = 0; i < workers->workers_nb; i++)
		free(workers[i].asigned_squares);
	free(workers);
}
