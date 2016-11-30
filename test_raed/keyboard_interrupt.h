#ifndef _KEYBOARD_INTERRUPT_H_
#define _KEYBOARD_INTERRUPT_H_

#include <stdbool.h>
#include <pthread.h>
#include <SDL2/SDL.h>



SDL_Keycode keypress();
void* keypress_thread(void* arg);

#endif