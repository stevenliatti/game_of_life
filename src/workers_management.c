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
	board_t* board = board_alloc(width, height);
	int cnt = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i == 0 || i == width-1 || j == 0 || j == height-1){
				board->matrix[i][j].is_alive_past = false;
				board->matrix[i][j].is_alive = false;
			}
			else {
				int random = rand() % 101;
				//printf("%d ", random);
				if (prob != 0 && random <= prob){
					cnt++;
			 		board->matrix[i][j].is_alive_past = true;
			 		board->matrix[i][j].is_alive = true;
			 	}
				else{
					board->matrix[i][j].is_alive_past = false;
					board->matrix[i][j].is_alive = false;
				}
			}
		}
	}
	printf("cnt : %d\n", cnt);
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			update_neighbours(board->matrix,i,j);
		}
	}
	return board;
}

void squares_to_compute_gen(worker_t* workers) {
	int count = 0;
	int count2 = 0;
	for (int i = 1; i < workers->board->width - 1; i++) {
		for (int j = 1; j < workers->board->height - 1; j++) {
			
			workers[count % workers->workers_nb].squares_to_compute[count2].i = i;			
			workers[count % workers->workers_nb].squares_to_compute[count2].j = j;
			count++;
			if (count % workers->workers_nb == 0) {
				count2++;
			}
		}
	}
	///////
	// for(int i = 0; i < workers->workers_nb; i++) {

	// 	for (int j = 0; j < workers[i].points_array_size; j++)
	// 	{
	// 		printf("[%d,%d] ", workers[i].squares_to_compute[j].i, workers[i].squares_to_compute[j].j);
	// 	}
	// 	printf("\n");
	// }
}

sync_t* sync_init(int workers_nb) {
	sync_t* sync = malloc(sizeof(sync_t));
	sem_init(&(sync->sem_escape),0,0);
	pthread_barrier_init(&(sync->workers_barrier),NULL,workers_nb+1);
	sem_init(&(sync->sem_workers),0,0);
	pthread_mutex_init(&(sync->compute_nb_mutex), NULL);
	sync->escape_pressed = false;
	sync->compute_nb = 0;
	return sync;
}

worker_t* workers_init(int workers_nb, int width, int height, int seed, int prob) {
	worker_t* workers = malloc(sizeof(worker_t)*workers_nb);
	board_t* board = board_gen(width, height, seed, prob);
	sync_t* sync = sync_init(workers_nb);

	int effective_squares_nb = (width -2) * ( height - 2);
	double double_size = ((double) effective_squares_nb) / workers_nb;
	int int_size = effective_squares_nb / workers_nb;
	int points_array_size = int_size;
	if (double_size != (double) int_size){
		points_array_size = (int) (double_size - (double_size - int_size)) + 1;
	}

	printf("effective_squares_nb : %d\n", effective_squares_nb);
	printf("point_array_size : %d\n", points_array_size);
	for(int i = 0; i < workers_nb; i++) {
		workers[i].board = board;
		workers[i].id = i;
		workers[i].workers_nb = workers_nb;
		workers[i].sync = sync;
		workers[i].squares_to_compute = malloc(sizeof(point_t) * points_array_size);
		workers[i].points_array_size = points_array_size;
		for (int j = 0; j < points_array_size; j++) {
			workers[i].squares_to_compute[j].i = -42;
			workers[i].squares_to_compute[j].j = -42;
		}
	}
	squares_to_compute_gen(workers);
	return workers;
}

void workers_free(worker_t* workers) {
	free(workers->board->matrix[0]);
	free(workers->board->matrix);
	free(workers->board);
	pthread_barrier_destroy(&(workers->sync->workers_barrier));
	sem_destroy(&(workers->sync->sem_workers));
	sem_destroy(&(workers->sync->sem_escape));
	pthread_mutex_destroy(&(workers->sync->compute_nb_mutex));
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
