#include "keyboard_interrupt.h"

SDL_Keycode keypress() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}


void* keypress_thread(void* arg){
	while (keypress() != SDLK_ESCAPE) {
		sleep(1);
	}
	escape_pressed = true;
	pthread_barrier_wait(&barrier);
}

