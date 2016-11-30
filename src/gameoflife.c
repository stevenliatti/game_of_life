/**
* @file gameoflife.c
* @brief Game of life
*
*
*
* @author Steven Liatti
* @author Orphée Antoniadis
* @author Raed Abdennadher
* @bug No known bugs.
* @date November 23, 2016
* @version 1.0
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "workers_management.h"

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }


pthread_barrier_t barrier; 

/**
 * @param arg 
 * @return 
 */
void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;

}

/**
 * This is the main function. It initialize the variables, launch the threads,
 * print if the solution is not found, free the memory in use and calculate the
 * execution time of the program.
 *
 * @param argc
 * @param argv
 * @return the code's exit of program
 */
int main(int argc, char** argv) {
	if (argc == 7) {
/*		struct timespec start, finish;*/
/*		clock_gettime(CLOCK_MONOTONIC, &start);*/

		pthread_barrier_init(&barrier, NULL, 2);

		int workers_nb = atoi(argv[6]);
		int width = atoi(argv[1]);
		int height = atoi(argv[2]);
		pthread_t t[workers_nb];

		worker_t workers[workers_nb];
		board_t board;
		workers_init(workers,workers_nb,width,height);

		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_create(&t[i], NULL, work, &workers[i]), "pthread_create failed!");
		}

		pthread_barrier_wait(&barrier);

		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
		}

/*		clock_gettime(CLOCK_MONOTONIC, &finish);*/
/*		double elapsed = finish.tv_sec - start.tv_sec;*/
/*		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;*/
/*		printf("%f\n", elapsed);*/
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
