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
 * This function alloc the memory for the board of cells.
 *
 * @param width the width of the board in pixels
 * @param height the height of the board in pixels
 * @return the constructed board
 */
board_t* board_alloc(int width, int height) {
	board_t* board = malloc(sizeof(board_t));
	assert(board != NULL);
	cell_t* temp = malloc(sizeof(cell_t)*width*height);
	assert(temp != NULL);
	board->width = width;
	board->height = height;
	board->matrix = malloc(sizeof(cell_t*)*width);
	assert(board->matrix != NULL);
	for (int i = 0; i < width; i++) {
		board->matrix[i] = temp + height * i;
	}
	return board;
}

/**
 * This function update the neighbours number of the cell in argument.
 * Each cell has 8 neighbours to update.
 *
 * @param matrix the matrix of cells
 * @param cell the cell considered
 * @return void
 */
void update_neighbours(cell_t** matrix, cell_t* cell) {
	cell->nb_neighbours = 0;
	for (int i = cell->x - 1; i <= cell->x + 1; i++) {
		for (int j = cell->y - 1; j <= cell->y + 1; j++) {
			if (matrix[i][j].is_alive) {
				cell->nb_neighbours++;
			}
		}
	}
	if (cell->is_alive) {
		cell->nb_neighbours--;
	}
}

/**
 * This function generate the game board. It sets the state of each cell of
 * the board and then the number of alive cells near each cell.
 *
 * @param width the width of the board in pixels
 * @param height the height of the board in pixels
 * @param seed an integer used to randomly populate the board
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
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1){
				board->matrix[i][j].is_alive_past = false;
				board->matrix[i][j].is_alive = false;
			}
			else {
				float random = (float) rand() / (float) RAND_MAX;
				if (prob != 0 && random < prob) {
					cnt++;
			 		board->matrix[i][j].is_alive_past = true;
			 		board->matrix[i][j].is_alive = true;
			 	}
				else {
					board->matrix[i][j].is_alive_past = false;
					board->matrix[i][j].is_alive = false;
				}
			}
		}
	}
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			update_neighbours(board->matrix, &(board->matrix[i][j]));
		}
	}
	return board;
}

/**
 * This function sets the cells that each worker will work on.
 *
 * @param workers array of worker_t
 * @return void
 */
void assigned_cells_gen(worker_t* workers) {
	int worker_index = 0;
	int assigned_cell_index = 0;
	for (int i = 1; i < workers->board->width - 1; i++) {
		for (int j = 1; j < workers->board->height - 1; j++) {
			workers[worker_index % workers->workers_nb].assigned_cells[assigned_cell_index]
				= &(workers->board->matrix[i][j]);
			worker_index++;
			if (worker_index % workers->workers_nb == 0) {
				assigned_cell_index++;
			}
		}
	}
}

/**
 * This function initialize all the synchronization primitives.
 *
 * @param workers_nb number of thread workers
 * @return sync the structure that contains our synchronization primitives
 */
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

/**
 * This function calls all the others to generate the board and the
 * synchronization primitives and then initialize each worker.
 *
 * @param workers_nb number of thread workers
 * @param width the width of the board in pixels
 * @param height the height of the board in pixels
 * @param seed an integer used to randomly populate the board
 * @param prob the probability of having a live cell during initialization
 * @param freq the refreshing frequency
 * @return workers the array of worker_t
 */
worker_t* workers_init(int workers_nb, int width, int height, int seed, double prob, int freq) {
	worker_t* workers = malloc(sizeof(worker_t)*workers_nb);
	assert(workers != NULL);
	board_t* board = board_gen(width, height, seed, prob);
	sync_t* sync = sync_init(workers_nb);

	int cells_nb = (width - 2) * (height - 2);
	int assigned_cells_nb = cells_nb / workers_nb;;
	if (cells_nb % workers_nb != 0){
		assigned_cells_nb++;
	}

	for (int i = 0; i < workers_nb; i++) {
		workers[i].board = board;
		workers[i].id = i;
		workers[i].workers_nb = workers_nb;
		workers[i].sync = sync;
		workers[i].uperiod = (1.0 / freq) * 1e6;
		workers[i].assigned_cells = malloc(sizeof(cell_t*) * assigned_cells_nb);
		assert(workers[i].assigned_cells != NULL);
		workers[i].assigned_cells_nb = assigned_cells_nb;
	}
	assigned_cells_gen(workers);
	return workers;
}

/**
 * This function frees all the structures and synchronization primitives
 *
 * @param workers the array of worker_t
 * @return void
 */
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
		free(workers[i].assigned_cells);
	free(workers);
}
