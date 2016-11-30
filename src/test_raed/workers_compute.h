
#ifndef _WORKERS_COMPUTE_H_
#define _WORKERS_COMPUTE_H_

void update_square(square_t* square);
void update_neighbours(square_t** matrix, int x, int y);
void update_board(square_t** matrix, int x, int y);
void* work(void* arg);

#endif
