#include "display_board.h"
#include "workers_compute.h"

/// Render some white noise.
/// @param context graphical context to use.
void render(struct gfx_context_t *context, board_t *board) {
	gfx_clear(context, COLOR_BLACK);

	uint32_t color = COLOR_GREEN;

	for (int i = 0; i < board->height; ++i)
	{
		for (int j = 0; j < board->width; ++j)
		{
			if (board->matrix[j][i].is_alive_past)
				gfx_putpixel(context, j, i, color);
		}
	}
}

/**
 * @param arg
 * @return
 */
void* display(void* arg) {
	worker_t* worker = (worker_t*) arg;

	struct gfx_context_t *ctxt = gfx_create("Example", worker->board->width, worker->board->height);
	if (!ctxt) {
		fprintf(stderr, "Graphic mode initialization failed!\n");
	}else{
		//notify "keypress_thread" that the graphical context is built and initialized
		sem_post(&(worker->sync->sem_escape));
		while(!worker->sync->escape_pressed){

			//this thread must wait for the last worker thread to finish his treatment
			//by sem_workers
			sem_wait(&(worker->sync->sem_workers));

			// printf("avant\n");
			// print_board(worker->board);



			render(ctxt, worker->board);
			gfx_present(ctxt);


			update_board(worker->board);

			
			// printf("après\n");
			// print_board(worker->board);

			// sleep(4);

			//here, this thread will be the last thread achieving the barrier
			//so, all threads will resume their routines
			pthread_barrier_wait(&(worker->sync->workers_barrier));
		}

		gfx_destroy(ctxt);
	}

	return NULL;
}
