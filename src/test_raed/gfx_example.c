#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "gfx.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

typedef struct {
	bool pressed;
	sem_t sem;
} sync_t;

/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
static SDL_Keycode keypress() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}

/// Render some white noise.
/// @param context graphical context to use.
static void render(struct gfx_context_t *context) {
	gfx_clear(context, COLOR_BLACK);

	for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT/10; i++) {
		int x = rand() % context->width;
		int y = rand() % context->height;
		uint32_t intensity = rand() % 256;  // 8-bit per color channel
		uint32_t color = MAKE_COLOR(intensity,intensity,intensity);
		gfx_putpixel(context, x, y, color);
	}
}

static void sync_init(sync_t* sync) {
	sync->pressed = false;
	sem_init(&sync->sem, 0, 0);
}

void* keypress_thread(void* arg){
	sync_t* b = (sync_t*) arg;
	//wait for the graphical context to be built in "work" thread
	sem_wait(&b->sem);
	while (!b->pressed) {
		b->pressed = keypress() == SDLK_ESCAPE;
	}
	//pthread_barrier_wait(&barrier);
}

void* work(void* arg) {
	sync_t* b = (sync_t*) arg;
	struct gfx_context_t *ctxt = gfx_create("Example", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!ctxt) {
		fprintf(stderr, "Graphic mode initialization failed!\n");
	}else{
		printf("Hello\n");
		//notify "keypress_thread" that the graphical context is built
		sem_post(&b->sem);
		while (!b->pressed) {
			render(ctxt);
			gfx_present(ctxt);
		}
		gfx_destroy(ctxt);
	}
}

/// Program entry point.
/// @return the application status code (0 if success).
int main() {

	sync_t sync;
	sync_init(&sync);

	pthread_t t;
	CHECK_ERR(pthread_create(&t, NULL, work, &sync), "pthread_create failed!");

	pthread_t th_stop;
	CHECK_ERR(pthread_create(&th_stop, NULL, keypress_thread, &sync), "pthread_create failed!");

	CHECK_ERR(pthread_join(t, NULL), "pthread_join failed!");
	CHECK_ERR(pthread_join(th_stop, NULL), "pthread_join failed!");

	printf("Bye Bye\n");

	return EXIT_SUCCESS;
}
