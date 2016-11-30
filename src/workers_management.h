#ifndef _WORKERS_MANAGEMENT_H_
#define _WORKERS_MANAGEMENT_H_


typedef struct square_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} square_t;

typedef struct board_st {
	square_t** matrix;
	int width;
	int height;
} board_t;

typedef struct worker_st {
	board_t* board;
	int workers_nb;
	int id;
	//ajouter la déclaration de la sémaphore de synchronisation entre les workers et le thread d'affichage
	//ce sémaphore sera initialisé avec le nombre de workers
} worker_t;

typedef struct sync_st {
	pthread_barrier_t stop_thread_sync;
} sync_t;

void init_matrix(square_t** matrix);
void init_board(board_t* board, square_t** matrix, int width, int height);
void init_workers(worker_t* workers, int workers_nb, board_t* board);

#endif
