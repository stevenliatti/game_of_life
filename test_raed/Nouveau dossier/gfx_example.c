#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "gfx.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define CHECK_ERR(expr, msg) if (expr) { fprintf(stderr, "%s\n", msg); return EXIT_FAILURE; }

pthread_barrier_t barrier; 
bool escape_pressed;
struct gfx_context_t *ctxt;

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

void* keypress_thread(void* arg){
	while (keypress() != SDLK_ESCAPE) {
		//sleep(1);
	}
	escape_pressed = true;
	pthread_barrier_wait(&barrier);
}

void* work(void* arg) {
	printf("Hello\n");
	while(!escape_pressed){
		ctxt = gfx_create("Example", SCREEN_WIDTH, SCREEN_HEIGHT);
		if (!ctxt) {
			fprintf(stderr, "Graphic mode initialization failed!\n");
			return EXIT_FAILURE;
		}
		render(ctxt);
		gfx_present(ctxt);
	}
}

/// Program entry point.
/// @return the application status code (0 if success).
int main() {
	escape_pressed = false;

	pthread_barrier_init(&barrier, NULL, 2);

	
	pthread_t th_stop;
	CHECK_ERR(pthread_create(&th_stop, NULL, keypress_thread, NULL), "pthread_create failed!");

	pthread_t t;
	CHECK_ERR(pthread_create(&t, NULL, work, NULL), "pthread_create failed!");
	
	
	pthread_barrier_wait(&barrier);

	CHECK_ERR(pthread_join(t, NULL), "pthread_join failed!");
	CHECK_ERR(pthread_join(th_stop, NULL), "pthread_join failed!");

	gfx_destroy(ctxt);

	printf("Bye Bye\n");
	return EXIT_SUCCESS;
}
