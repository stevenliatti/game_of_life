#include "keyboard_interrupt.h"

/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
SDL_Keycode keypress() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN){
			return event.key.keysym.sym;
		}
	}
	return 0;
}

/**
 *
 */
void* keypress_thread(void* arg) {
	sync_t* sync = (sync_t*) arg;
	sem_wait(&(sync->sem_escape));
	while(!sync->escape_pressed){
		if (keypress() == SDLK_ESCAPE) {
			sync->escape_pressed = true;
		}
		usleep(500000);
	}
	return NULL;
}

