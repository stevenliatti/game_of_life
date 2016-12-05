#include "workers_compute.h"

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

void update_board(worker_t* worker) {
	for (int i = 0; i < worker->asigned_squares_nb; i++) {
		if (worker->asigned_squares[i] != NULL) {
			update_neighbours(worker->board->matrix, worker->asigned_squares[i]);
			worker->asigned_squares[i]->is_alive_past = worker->asigned_squares[i]->is_alive;
		}
	}
}

void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;

	while (!worker->sync->end_game) {
		pthread_barrier_wait(&(worker->sync->workers_barrier));
		for (int i = 0; i < worker->asigned_squares_nb; i++) {
			if (worker->asigned_squares[i] != NULL) {
				update_square(worker->asigned_squares[i]);
			}
		}

		// mutex to protect the workers counter
		pthread_mutex_lock(&(worker->sync->compute_nb_mutex));
		worker->sync->compute_nb++;
		pthread_mutex_unlock(&(worker->sync->compute_nb_mutex));

		if (worker->sync->compute_nb == worker->workers_nb) {
			worker->sync->compute_nb = 0;
			// if the current thread is the last one, it will allow display thread
			// to be executed by sem_workers
			sem_post(&(worker->sync->sem_display));
		}

		// threads will be blocked here and wait for display thread
		// to be executed to resume their routines
		pthread_barrier_wait(&(worker->sync->workers_barrier));
		update_board(worker);
	}

	return NULL;
}
