#ifndef _WORKERS_MANAGEMENT_H_
#define _WORKERS_MANAGEMENT_H_


typedef struct sync_st {
	pthread_barrier_t barrier;
	sem_t sem_display;
	sem_t sem_work;
	bool escape_pressed;
} sync_t;

typedef struct cell_st {
	bool is_alive;
	bool is_alive_past;
	int nb_neighbours;
} cell_t;

typedef struct board_st {
	cell_t** matrix;
	int width;
	int height;
} board_t;

typedef struct worker_st {
	board_t* board;
	int workers_nb;
	int id;
	sync_t* sync;
} worker_t;

void workers_init(worker_t* workers, int workers_nb, int width, int height);
void workers_free(worker_t* workers);
void print_board(board_t* board);

#endif
