#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct square_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} square_t;

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
	square->is_alive_past = square->is_alive;
}

void print_square(square_t* square) {
	printf("is_alive : %s\n", square->is_alive ? "true" : "false");
	printf("is_alive_past : %s\n", square->is_alive_past ? "true" : "false");
	printf("nb_neighbours : %d\n", square->nb_neighbours);
}

int main(int argc, char** argv) {
	if (argc == 2) {
		int nb_neighbours = atoi(argv[1]);
		square_t square;
		square.is_alive = false;
		square.is_alive_past = true;
		square.nb_neighbours = nb_neighbours;

		print_square(&square);
		update_square(&square);
		print_square(&square);
	}
}