#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>
#include "rala_glyphs.h"
#include "cairosdl.h"
#include "commands.h"

#define WIDTH 1280
#define HEIGHT 1024

cairo_t *cr;
SDL_Thread *rendert;

void fill_with_stems(int w, int h) {
	double user_w = w;
	double user_h = h;
	cairo_device_to_user_distance(cr, &user_w, &user_h);
	int user_w_int = ceil(user_w)+1;
	int user_h_int = ceil(user_h)+1;
	double user_x = 0.0;
	double user_y = 0.0;
	cairo_device_to_user(cr, &user_x, &user_y);
	int user_x_int = floor(user_x);
	int user_y_int = floor(user_y);
	int x,y;
	for(y=user_y_int; y<user_y_int+user_h_int; y++) {
		for(x=user_x_int; x<user_x_int+user_w_int; x++) {
			cairo_save(cr);
			cairo_translate(cr,x*2,y*2);
			blank_cell(cr);
			stem_cell_glyph(cr);
			cairo_restore(cr);
		}
	}
}

int render_thread (void* data) {
	// Set up drawing environment
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	cairo_paint(cr);
	
	cairo_translate(cr, 10,10);
	cairo_scale (cr, 30, 30);

	fill_with_stems(WIDTH, HEIGHT);
	cairosdl_surface_flush(cairo_get_target(cr));
	SDL_Event event;
	event.type = SDL_USEREVENT;
	SDL_PushEvent (&event);
	printf("pushed event\n");

	// Set up socket
	char** argv = (char**)data;
	IPaddress ip;
	TCPsocket server_sock;
	if(SDLNet_ResolveHost(&ip,NULL,21234) < 0) {
		printf("Could not resolve host: %s\n", SDLNet_GetError());
		exit(3);
	}
	server_sock = SDLNet_TCP_Open(&ip);
	if(!server_sock) {
		printf("Could not open socket: %s\n", SDLNet_GetError());
		exit(4);
	}

	TCPsocket sock = NULL;
	while(sock == NULL) {
		sock = SDLNet_TCP_Accept(server_sock);
	}
	printf("got connection\n");

	//Wait for commands
	char buf;
	while(SDLNet_TCP_Recv(sock, &buf, 1) > 0) {
		next_command_char(buf, cr);
	}
}

void init_video(void) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
}

void init_video_net(void) {
	init_video();
	if(SDLNet_Init() < 0) {
		fprintf(stderr, "Unable to init SDLNet: %s\n", SDLNet_GetError());
		exit(2);
	}
	atexit(SDLNet_Quit);
}

void set_canvas_cairosdl(SDL_Surface **screen, SDL_Surface **canvas) {
	*screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RESIZABLE);
	*canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
	cr = cairosdl_create(*canvas);
}

void sdl_event_loop(SDL_Surface **screen, SDL_Surface **canvas) {
	SDL_Event event;
	while(SDL_WaitEvent (&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_q) {
					SDL_KillThread(rendert);
					exit(0);
				}
				break;
			case SDL_QUIT:
				SDL_KillThread(rendert);
				exit(0);
			case SDL_VIDEORESIZE:
				*screen = SDL_SetVideoMode (event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_RESIZABLE);
				/*canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
				cr = cairosdl_create(canvas);
				fill_with_stems(event.resize.w, event.resize.h);*/
			case SDL_USEREVENT:
				SDL_BlitSurface(*canvas, NULL, *screen, NULL);
				SDL_Flip (*screen);
				printf("Repainted screen\n");
				break;
		}
	}
	printf("Terminated: %s\n", SDL_GetError());
	SDL_KillThread(rendert);
	exit(5);
}

int main(int argc, char **argv) {
	SDL_Surface *screen, *canvas;
	init_video_net();
	set_canvas_cairosdl(&screen, &canvas);
	rendert = SDL_CreateThread(render_thread, argv);
	sdl_event_loop(&screen, &canvas);
	return 0;
}
