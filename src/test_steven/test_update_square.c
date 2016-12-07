#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct cell_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} cell_t;

void update_cell(cell_t* cell) {
	if (cell->is_alive_past) {
		if (cell->nb_neighbours < 2 || cell->nb_neighbours > 3) {
			cell->is_alive = false;
		}
		else if (cell->nb_neighbours == 2 || cell->nb_neighbours == 3) {
			cell->is_alive = true;
		}
	}
	else {
		if (cell->nb_neighbours == 3) {
			cell->is_alive = true;
		}
	}

	// Judicieux de faire ça ici ? Risque de faire chier aux voisins
	// mieux vaut parcourir toutes les cases après avoir fait update_cell et mettre à jour le nombre de voisins
	cell->is_alive_past = cell->is_alive;
}

void print_cell(cell_t* cell) {
	printf("is_alive : %s\n", cell->is_alive ? "true" : "false");
	printf("is_alive_past : %s\n", cell->is_alive_past ? "true" : "false");
	printf("nb_neighbours : %d\n", cell->nb_neighbours);
}

int main(int argc, char** argv) {
	if (argc == 2) {
		int nb_neighbours = atoi(argv[1]);
		cell_t cell;
		cell.is_alive = false;
		cell.is_alive_past = true;
		cell.nb_neighbours = nb_neighbours;

		print_cell(&cell);
		update_cell(&cell);
		print_cell(&cell);
	}
}