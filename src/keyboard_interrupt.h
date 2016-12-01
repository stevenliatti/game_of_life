#include "workers_management.h"

#ifndef _KEYBOARD_INTERRUPT_H_
#define _KEYBOARD_INTERRUPT_H_

SDL_Keycode keypress();
void* keypress_thread(void* arg);

#endif