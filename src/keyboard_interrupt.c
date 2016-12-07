/**
 * @file keyboard_interrupt.c
 * @brief key interruption context
 *
 * This file contains functions for the thread that detect the key press event.
 *
 * @author Steven Liatti
 * @author Orphée Antoniadis
 * @author Raed Abdennadher
 * @bug No known bugs.
 * @date December 7, 2016
 * @version 1.0
 */

#include "keyboard_interrupt.h"

/**
 * This function detect all events and return the escape key press event.
 *
 * @param void
 * @return SDL_Keycode the pressed key code
 */
SDL_Keycode keypress() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}

/**
 * Function that called by the thread that will detect the escape key press.
 *
 * @param arg sync_t struct
 * @return void
 */
void* keypress_thread(void* arg) {
	sync_t* sync = (sync_t*) arg;
	sem_wait(&(sync->sem_escape));
	while(!sync->escape_pressed) {
		if (keypress() == SDLK_ESCAPE) {
			sync->escape_pressed = true;
		}
		usleep(500000);
	}
	return NULL;
}

