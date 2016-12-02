#include "workers_compute.h"

/**
 *
 */
void update_square(square_t* square) {
	if (square->is_alive_past) {
		if ((square->nb_neighbours < 2) || (square->nb_neighbours > 3)) {
			square->is_alive = false;
		}
		else if (square->nb_neighbours == 2 || square->nb_neighbours == 3) {
			square->is_alive = true;
		}
	}
	else {
		if (square->nb_neighbours == 3) {
			square->is_alive = true;
		}
	}
}

/**
 *
 */
void update_board(board_t* board) {
	for (int i = 1; i < board->width - 1; i++) {
		for (int j = 1; j < board->height - 1; j++) {
			update_neighbours(board->matrix, i, j);
			board->matrix[i][j].is_alive_past = board->matrix[i][j].is_alive;
		}
	}
}

/**
 * 
 */
void update_board_threaded(worker_t* worker, int squares_nb, int select) {
	
}

/**
 * @param arg
 * @return
 */
void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;
	//int squares_nb = (worker->board->width - 1) * (worker->board->height - 1);
	//int select = worker->id;

	while (!worker->sync->escape_pressed) {
		for (int i = 0; i < worker->points_array_size; i++) {
			if (worker->squares_to_compute[i].i > 0) {
				int row = worker->squares_to_compute[i].i;
				int col = worker->squares_to_compute[i].j;

				//printf("id %d [%d,%d] ", worker->id, row, col);

				// printf("\n");

				update_square(&(worker->board->matrix[row][col]));

				//printf("id %d [%d,%d] ", worker->id, row, col);
			}
		}
				// printf("\n");

		// PASSER LA MAIN AU THREAD AFFICHAGE
		pthread_mutex_lock(&(worker->sync->compute_nb_mutex));
		worker->sync->compute_nb++;
		pthread_mutex_unlock(&(worker->sync->compute_nb_mutex));

		

		if (worker->sync->compute_nb == worker->workers_nb) {
			worker->sync->compute_nb = 0;
			//if the current thread is the last one, it will allow display thread
			//to be executed by sem_workers
			sem_post(&(worker->sync->sem_workers));
		}

		

		//threads will be bocked here and wait for display thread to be executed to resume
		// their routines
		pthread_barrier_wait(&(worker->sync->workers_barrier));

		
	}

	return NULL;
}
