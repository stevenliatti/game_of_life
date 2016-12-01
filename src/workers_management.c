#include "workers_management.h"

board_t* board_alloc(int width, int height) {
	board_t* board = malloc(sizeof(board_t));
	square_t* temp = malloc(sizeof(square_t)*width*height);
	board->width = width;
	board->height = height;
	board->matrix = malloc(sizeof(square_t*)*width);
	for (int i = 0; i < width; i++) {
		board->matrix[i] = temp + height * i;
	}
	return board;
}

void update_neighbours(square_t** matrix, int x, int y) {
	matrix[x][y].nb_neighbours = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (matrix[i][j].is_alive) {
				matrix[x][y].nb_neighbours++;
			}
		}
	}
	if (matrix[x][y].is_alive) {
		matrix[x][y].nb_neighbours--;
	}
}

board_t* board_gen(int width, int height, int seed, int prob) {
	srand(seed);
	board_t* board = board_alloc(width,height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i == 0 || i == width-1 || j == 0 || j == height-1)
				board->matrix[i][j].is_alive_past = 0;
			else {
				if (rand() % 101 <= prob)
			 		board->matrix[i][j].is_alive_past = true;
				else
					board->matrix[i][j].is_alive_past = false;
			}
		}
	}
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			update_neighbours(board->matrix,i,j);
		}
	}
	return board;
}

sync_t* sync_init(int workers_nb) {
	sync_t* sync = malloc(sizeof(sync_t));
	pthread_barrier_init(&(sync->escape_barrier),NULL,2);
	pthread_barrier_init(&(sync->escape_barrier),NULL, workers_nb);
	sem_init(&(sync->sem_display),0,0);
	pthread_mutex_init(&(sync->mutex_compute_nb), NULL);
	sync->escape_pressed = false;
	sync->compute_nb = 0;
	return sync;
}

worker_t* workers_init(int workers_nb, int width, int height, int seed, int prob) {
	worker_t* workers = malloc(sizeof(worker_t)*workers_nb);
	board_t* board = board_gen(width, height, seed, prob);
	sync_t* sync = sync_init(workers_nb);
	for(int i = 0; i < workers_nb; i++) {
		workers[i].board = board;
		workers[i].id = i;
		workers[i].workers_nb = workers_nb;
		workers[i].sync = sync;
	}
	return workers;
}

void workers_free(worker_t* workers) {
		free(workers->board->matrix[0]);
		free(workers->board->matrix);
		free(workers->board);
		free(workers->sync);
		free(workers);
}

void print_board(board_t* board) {
	for (int i = 0; i < board->width; i++) {
		for (int j = 0; j < board->height; j++) {
			printf("%d ", board->matrix[i][j].is_alive_past);
		}
		printf("\n");
	}
}
