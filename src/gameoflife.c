/**
* @file gameoflife.c
* @brief Game of life
*
* @author Steven Liatti
* @author Orphée Antoniadis
* @author Raed Abdennadher
* @bug No known bugs.
* @date November 23, 2016
* @version 1.0
*/

#include "workers_management.h"
#include "workers_compute.h"
#include "keyboard_interrupt.h"
#include "display_board.h"

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

int main(int argc, char** argv) {
	if (argc == 7) {

		/// declaration section
		int width = atoi(argv[1]);
		int height = atoi(argv[2]);
		int seed = atoi(argv[3]);
		int prob = (int)(atof(argv[4])*100);
		int freq = atoi(argv[5]);
		int workers_nb = atoi(argv[6]);


		int squares_nb = (width - 2) * (height - 2);
		if (workers_nb > squares_nb) {
			printf("The maximum number of threads that can be used for the resolution %dx%d is %d.\n", width, height, squares_nb);
			printf("So that, the game will run with only %d threads.\n", squares_nb);
			workers_nb = squares_nb;
		}
		///

		/// initialization section
		worker_t* workers = workers_init(workers_nb, width, height, seed, prob, freq);
		///

		/// threads creation section
		pthread_t t[workers_nb];
		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_create(&t[i], NULL, work, &workers[i]), "pthread_create failed!");
		}
		pthread_t th_display;
		CHECK_ERR(pthread_create(&th_display, NULL, display, workers), "pthread_create failed!");
		pthread_t th_stop;
		CHECK_ERR(pthread_create(&th_stop, NULL, keypress_thread, workers->sync), "pthread_create failed!");
		///

		/// threads join section
		CHECK_ERR(pthread_join(th_stop, NULL), "pthread_join failed!");
		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
		}
		CHECK_ERR(pthread_join(th_display, NULL), "pthread_join failed!");
		///

		workers_free(workers);
		return EXIT_SUCCESS;
	}
	else {
		fprintf(stderr, "\ngameoflife <width> <height> <seed> <p> <freq> <#workers>\n\n"
					"• width and height are integers specifying the dimensions of the game (>= 4).\n"
					"• seed is an integer used to randomly populate the board.\n"
					"• p is a floating point value (range [0..1]) which is the\n"
					"  probability of having a live cell during initialization.\n"
					"• freq is an integer specifying the display frequency in Hz (> 0).\n"
					"• #workers is an integer specifying the number of worker threads (>= 1).\n\n"
					"Example: gameoflife 240 135 0 0.75 30 8\n\n");
		return EXIT_FAILURE;
	}
}
