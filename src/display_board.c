/**
 * @file display_board.c
 * @brief display context
 *
 * This file contains functions for the thread that display the board
 *
 * @author Steven Liatti
 * @author Orphée Antoniadis
 * @author Raed Abdennadher
 * @bug No known bugs.
 * @date December 7, 2016
 * @version 1.0
 */

#include "display_board.h"
#include "workers_compute.h"

/**
 * Render the actual board state.
 *
 * @param context graphical context to use
 * @param board board to be displayed
 * @return void
 */
void render(struct gfx_context_t *context, board_t *board) {
	gfx_clear(context, COLOR_BLACK);
	uint32_t color = COLOR_WHITE;
	for (int i = 0; i < board->height; ++i) {
		for (int j = 0; j < board->width; ++j) {
			if (board->matrix[j][i].is_alive_past)
				gfx_putpixel(context, j, i, color);
		}
	}
}

/**
 * Calculation of the real time the thread must sleep (in microseconds).
 *
 * @param start time
 * @param finish time
 * @return void
 */
void adapt_frequency(struct timespec start, struct timespec finish, double uperiod){
	double elapsed = (finish.tv_sec - start.tv_sec) * 1e6;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1e3;
	if (elapsed < uperiod) {
		useconds_t uperiod_sleep = uperiod - elapsed;
		usleep(uperiod_sleep);
	}
}

/**
 * Function that called by the thread that will dirplay the actual board status. It create the 
 * the graphical context, notify the context has been created, wait for all workers finish their treatments
 * and adapt frequency, then display the board. Finally, it will destroy the graphical context.
 *
 * @param arg worker_t struct
 * @return
 */
void* display(void* arg) {
	worker_t* worker = (worker_t*) arg;
	struct timespec start, finish;

	struct gfx_context_t *ctxt = gfx_create("Example", worker->board->width, worker->board->height);
	if (!ctxt) {
		fprintf(stderr, "Graphic mode initialization failed!\n");
	} else {
		sem_post(&(worker->sync->sem_escape));
		clock_gettime(CLOCK_MONOTONIC, &start);
		while (!worker->sync->end_game) {
			pthread_barrier_wait(&(worker->sync->workers_barrier));
			sem_wait(&(worker->sync->sem_display));

			render(ctxt, worker->board);

			clock_gettime(CLOCK_MONOTONIC, &finish); 
			
			adapt_frequency(start, finish, worker->uperiod);
			
			gfx_present(ctxt);

			if (worker->sync->escape_pressed){
				worker->sync->end_game = true;
			}

			pthread_barrier_wait(&(worker->sync->workers_barrier));
			clock_gettime(CLOCK_MONOTONIC, &start);
		}

		gfx_destroy(ctxt);
	}

	return NULL;
}
