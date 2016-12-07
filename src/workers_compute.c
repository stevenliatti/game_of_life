/**
 * @file workers_compute.c
 * @brief Workers functions
 *
 * This file contains functions for the workers. It's here where 
 * the rules of the game are applied on each cells and where the 
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
 * This function update the state of the cell in argument in function 
 * of the game of life's rules and the past state of the cell itself.
 *
 * @param cell the cell considered
 * @return void
 */
void update_cell(cell_t* cell) {
	if (cell->is_alive_past) {
		if (cell->nb_neighbours < 2 || cell->nb_neighbours > 3) {
			cell->is_alive = false;
		}
		else if (cell->nb_neighbours == 2 || cell->nb_neighbours == 3) {
			cell->is_alive = true;
		}
	}
	else {
		if (cell->nb_neighbours == 3) {
			cell->is_alive = true;
		}
	}
}

/**
 * This function update the cells that are assigned to the worker. Foreach cell, 
 * the past state and the neighbours are updated.
 *
 * @param worker the worker considered
 * @return void
 */
void update_board(worker_t* worker) {
	for (int i = 0; i < worker->assigned_cells_nb; i++) {
		if (worker->assigned_cells[i] != NULL) {
			update_neighbours(worker->board->matrix, worker->assigned_cells[i]);
			worker->assigned_cells[i]->is_alive_past = worker->assigned_cells[i]->is_alive;
		}
	}
}

/**
 * It's the main function for the workers : while the escape button is not
 * pressed, the workers will calculate the next state of their cells. With 
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
		for (int i = 0; i < worker->assigned_cells_nb; i++) {
			if (worker->assigned_cells[i] != NULL) {
				update_cell(worker->assigned_cells[i]);
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
