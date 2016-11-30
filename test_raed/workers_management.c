
#include "workers_management.h"

void init_matrix(square_t** matrix) {

}

void init_board(board_t* board, square_t** matrix, int width, int height) {
	board->width = width;
	board->height = height;
	board->matrix = matrix;
	init_matrix(board->matrix);
}

void init_workers(worker_t* workers, int workers_nb, board_t* board) {
	for(int i = 0; i < workers_nb; i++) {
		workers[i].board = board;
		workers[i].id = i;
		workers[i].workers_nb = workers_nb;
	}
}
