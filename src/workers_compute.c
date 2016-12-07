/**
 * @file workers_compute.c
 * @brief Workers functions
 *
 * This file contains functions for the workers. It's here where 
 * the rules of the game are applied on each squares and where the 
 * board is updated.
 *
 * @author Steven Liatti
 * @author Orphée Antoniadis
 * @author Raed Abdennadher
 * @bug No known bugs.
 * @date December 7, 2016
 * @version 1.0
 */

#include "workers_compute.h"

/**
 * This function update the state of the square in argument in function 
 * of the game of life's rules and the past state of the square itself.
 *
 * @param square the square considered
 * @return void
 */
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

/**
 * This function update the squares that are assigned to the worker. Foreach square, 
 * the past state and the neighbours are updated.
 *
 * @param worker the worker considered
 * @return void
 */
void update_board(worker_t* worker) {
	for (int i = 0; i < worker->assigned_squares_nb; i++) {
		if (worker->assigned_squares[i] != NULL) {
			update_neighbours(worker->board->matrix, worker->assigned_squares[i]);
			worker->assigned_squares[i]->is_alive_past = worker->assigned_squares[i]->is_alive;
		}
	}
}

/**
 * It's the main function for the workers : while the escape button is not
 * pressed, the workers will calculate the next state of their squares. With 
 * differents synchronization primitives, there is the guarantee that the display
 * will wait on workers before executing.
 *
 * @param arg a pointer on void
 * @return void
 */
void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;

	while (!worker->sync->end_game) {
		pthread_barrier_wait(&(worker->sync->workers_barrier));
		for (int i = 0; i < worker->assigned_squares_nb; i++) {
			if (worker->assigned_squares[i] != NULL) {
				update_square(worker->assigned_squares[i]);
			}
		}

		pthread_mutex_lock(&(worker->sync->compute_nb_mutex));
		worker->sync->compute_nb++;
		pthread_mutex_unlock(&(worker->sync->compute_nb_mutex));

		if (worker->sync->compute_nb == worker->workers_nb) {
			worker->sync->compute_nb = 0;
			sem_post(&(worker->sync->sem_display));
		}

		pthread_barrier_wait(&(worker->sync->workers_barrier));
		update_board(worker);
	}

	return NULL;
}
