#include "workers_management.h"

#ifndef _DISPLAY_BOARD_H_
#define _DISPLAY_BOARD_H_

void render(struct gfx_context_t *context, board_t *board);
void* display(void* arg);

#endif