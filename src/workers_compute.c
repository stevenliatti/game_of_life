#include "workers_compute.h"

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
 *
 */
void update_neighbours(square_t** matrix, int x, int y) {
	matrix[x][y].nb_neighbours = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (matrix[i][j].is_alive) {
				matrix[x][y].nb_neighbours++;
			}
		}
	}
	// This condition is for exlcude the square considered.
	if (matrix[x][y].is_alive) {
		matrix[x][y].nb_neighbours--;
	}
}

/**
 *
 */
void update_board(square_t** matrix, int x, int y) {

}

/**
 * @param arg 
 * @return 
 */
void* work(void* arg) {
	worker_t* worker = (worker_t*) arg;

}