#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }



pthread_barrier_t barrier; 
bool escape_pressed;

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

void* work(void* arg) {
	while(!escape_pressed){
		
	}
}

int main(int argc, char** argv) {
	escape_pressed = false;

	pthread_barrier_init(&barrier, NULL, 2);

	pthread_t t;
	CHECK_ERR(pthread_create(&t, NULL, work, NULL), "pthread_create failed!");
	
	pthread_t th_stop;
	CHECK_ERR(pthread_create(&th_stop, NULL, keypress_thread, NULL), "pthread_create failed!");

	pthread_barrier_wait(&barrier);

	CHECK_ERR(pthread_join(t, NULL), "pthread_join failed!");
	CHECK_ERR(pthread_join(th_stop, NULL), "pthread_join failed!");

	printf("Bye Bye\n");
	return EXIT_SUCCESS;
}