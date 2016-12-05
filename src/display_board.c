#include "display_board.h"
#include "workers_compute.h"

/**
 * Render some white noise.
 * @param context graphical context to use
 * @return
 */
void render(struct gfx_context_t *context, board_t *board) {
	gfx_clear(context, COLOR_BLACK);
	uint32_t color = COLOR_GREEN;
	for (int i = 0; i < board->height; ++i) {
		for (int j = 0; j < board->width; ++j) {
			if (board->matrix[j][i].is_alive_past)
				gfx_putpixel(context, j, i, color);
		}
	}
}

void* display(void* arg) {
	struct timespec start, finish;
	worker_t* worker = (worker_t*) arg;

	struct gfx_context_t *ctxt = gfx_create("Example", worker->board->width, worker->board->height);
	if (!ctxt) {
		fprintf(stderr, "Graphic mode initialization failed!\n");
	} else {
		// notify "keypress_thread" that the graphical context is built and initialized
		sem_post(&(worker->sync->sem_escape));
		
		while (!worker->sync->end_game) {
			clock_gettime(CLOCK_MONOTONIC, &start);
			// the barrier is here to make sure that all the workers will
			// start after the start of the timer 
			pthread_barrier_wait(&(worker->sync->workers_barrier));
			// this thread must wait for the last worker thread to finish
			// his treatment by sem_workers
			sem_wait(&(worker->sync->sem_display));
			render(ctxt, worker->board);
			clock_gettime(CLOCK_MONOTONIC, &finish); 
			
			// calculation of the real time the thread must sleep (in microseconds)
			double elapsed = (finish.tv_sec - start.tv_sec) * 1e6;
			elapsed += (finish.tv_nsec - start.tv_nsec) / 1e3;
			if (elapsed < worker->uperiod) {
				useconds_t uperiod = worker->uperiod - elapsed;
				usleep(uperiod);
			}
			
			gfx_present(ctxt);

			if (worker->sync->escape_pressed){
				worker->sync->end_game = true;
			}

			// here, this thread will be the last thread achieving the barrier
			// so, all threads will resume their routines
			pthread_barrier_wait(&(worker->sync->workers_barrier));
		}

		gfx_destroy(ctxt);
	}

	return NULL;
}
