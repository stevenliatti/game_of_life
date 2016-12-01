#include "workers_management.h"
#include "workers_compute.h"

void update_square(square_t* square) {
	if (square->is_alive_past) {
		if (square->nb_neighbours < 2 || square->nb_neighbours > 3) {
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

void update_board(board_t* board) {
	for (int i = 1; i < board->width - 1; i++) {
		for (int j = 1; j < board->height - 1; j++) {
			update_neighbours(board->matrix, i, j);
			board->matrix[i][j].is_alive_past = board->matrix[i][j].is_alive;
		}
	}
}

void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;
	int squares_nb = worker->board->width * worker->board->height;
	int select = worker->id;

	while (!worker->sync->escape_pressed) {
		while (select <= squares_nb) {


			int row = select / worker->board->width;
			int col = select % worker->board->height;

			update_square(&(worker->board->matrix[row][col]));

			select += worker->workers_nb;
		}
		select = worker->id;

		// PASSER LA MAIN AU THREAD AFFICHAGE
		pthread_mutex_lock(&(worker->sync->compute_nb_mutex));
		worker->sync->compute_nb++;
		pthread_mutex_unlock(&(worker->sync->compute_nb_mutex));
		if (worker->sync->compute_nb == worker->workers_nb) {
			worker->sync->compute_nb = 0;
			sem_post(&(worker->sync->sem_display));
		}
		pthread_barrier_wait(&(worker->sync->workers_barrier));

		update_board(worker->board);
	}

	return NULL;
}
