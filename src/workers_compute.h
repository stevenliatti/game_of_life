#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "gfx.h"
#include "workers_management.h"

#ifndef _WORKERS_COMPUTE_H_
#define _WORKERS_COMPUTE_H_

void update_square(square_t* square);
void update_neighbours(square_t** matrix, int x, int y);
void update_board(board_t* board);
void* work(void* arg);

#endif
