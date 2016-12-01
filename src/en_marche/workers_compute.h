#include "workers_management.h"

#ifndef _WORKERS_COMPUTE_H_
#define _WORKERS_COMPUTE_H_

void update_square(square_t* square);
void update_board(board_t* board);
void* work(void* arg);

#endif
