# Journal de bord

## Structures
Manque la gestion des mutex/sémaphores/etc. -> structure dédiée ou tout dans worker_t ?
````
typedef struct square_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} square_t;
````

````
typedef struct board_st {
	square_t** matrix;
	int width;
	int height;
} board_t;
````

````
typedef struct worker_st {
	board_t* board;
	int workers_nb;
	int id;
} worker_t;
````

## Thread Main
- init structures -> workers, board
- pthread_create
- barrière pour finir le main
- join les threads

## Thread Clavier
````
while (not_press_on_ESC) {
	usleep(50);
}
// mécanisme de boolean static pour annuler les autres threads
````

## Thread Affichage
Sans oublier le calcul du temps !
````
void* thread_affiche(board_t* b) {
	sem.wait();
	gfx_clear(); // fonction du prof
	for(square_t s : b) {
		if(s.isAlive) {
			gfx_put_pixel(color); // fonction du prof
		}
	}
	gfx_present(); // fonction du prof
}
````

## Thread Calcul
Au max on fait modulo le nombre de cases -> ne pas gérer si `threads_nb > cases_nb`
````
void* thread(board_t* b) {
	square_t square = select_the_good_square(b); // en fonction de l'id du thread
	update_square_status(square); // case/cellule vivante ou morte
	// ...
}
````

## Le reste ...
- On a mentionné le `seed` : comment on gère ?
- Je (Steven) suis pas encore tout à fait sûr pour la gestion des bords..