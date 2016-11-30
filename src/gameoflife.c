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

/**
 *
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

	// Judicieux de faire ça ici ? Risque de faire chier aux voisins
	// mieux vaut parcourir toutes les cases après avoir fait update_square et mettre à jour le nombre de voisins
	//square->is_alive_past = square->is_alive;
}

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



		int workers_nb = atoi(argv[6]);
		int width = atoi(argv[1]);
		int height = atoi(argv[2]);
		pthread_t t[workers_nb];

		worker_t workers[workers_nb];
		board_t board;
		square_t matrix[width][heigth];

		init_board(&board, matrix, width, heigth);
		init_workers(workers, workers_nb, &board);



		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_create(&t[i], NULL, work, &workers[i]), "pthread_create failed!");
		}
		for (int i = 0; i < workers_nb; i++) {
			CHECK_ERR(pthread_join(t[i], NULL), "pthread_join failed!");
		}

/*		clock_gettime(CLOCK_MONOTONIC, &finish);*/
/*		double elapsed = finish.tv_sec - start.tv_sec;*/
/*		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;*/
/*		printf("%f\n", elapsed);*/
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
