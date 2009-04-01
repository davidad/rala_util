#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>
#include "rala_glyphs.h"
#include "cairosdl.h"

#define WIDTH 1280
#define HEIGHT 1024

cairo_t *cr;

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
	char cell_type[5];
	while(SDLNet_TCP_Recv(sock, &cell_type, 5) > 0) {
		printf("received command: %c\n", cell_type[0]);
		char coord_buf[12];
		int x, y;
		int i=-1;
		memset(coord_buf, 0, 12);
		do {
			SDLNet_TCP_Recv(sock, &coord_buf[++i], 1);
		} while(coord_buf[i] != ',');
		x = atoi(coord_buf);
		i = -1;
		memset(coord_buf, 0, 12);
		do {
			SDLNet_TCP_Recv(sock, &coord_buf[++i], 1);
		} while(coord_buf[i] != '\n');
		y = atoi(coord_buf);
		cairo_save(cr);
		cairo_translate(cr,x*2,y*2);
		blank_cell(cr);
		switch(cell_type[0]) {
			case 'n':
				nand_gate_glyph(cr);
				break;
			case 'a':
				and_gate_glyph(cr);
				break;
			case 'o':
				or_gate_glyph(cr);
				break;
			case 'x':
				xor_gate_glyph(cr);
				break;
			case 'c':
				copy_cell_glyph(cr,0);
				break;
			case 'd':
				delete_cell_glyph(cr,0);
				break;
			case 'w':
				wire_cell_glyph(cr);
				break;
			case 'C':
				crossover_cell_glyph(cr);
				break;
		}
		cairo_save(cr);
		cairo_translate(cr,0.5,0.5);
		switch(cell_type[1]) {
			case 'w':
				break;
			case 'n':
				cairo_rotate(cr,3*M_PI/2);
				break;
			case 'e':
				cairo_rotate(cr,M_PI);
				break;
			case 's':
				cairo_rotate(cr,M_PI/2);
				break;
		}
		cairo_translate(cr,-1.4,-0.4);
		switch(cell_type[2]) {
			case 'x':
				arrow_x_glyph(cr);
				break;
			case '0':
				arrow_0_glyph(cr);
				break;
			case '1':
				arrow_1_glyph(cr);
				break;
		}
		cairo_restore(cr);
		cairo_save(cr);
		cairo_translate(cr,0.5,0.5);
		switch(cell_type[3]) {
			case 'w':
				break;
			case 'n':
				cairo_rotate(cr,3*M_PI/2);
				break;
			case 'e':
				cairo_rotate(cr,M_PI);
				break;
			case 's':
				cairo_rotate(cr,M_PI/2);
				break;
		}
		cairo_translate(cr,-1.4,-0.4);
		switch(cell_type[4]) {
			case 'x':
				arrow_x_glyph(cr);
				break;
			case '0':
				arrow_0_glyph(cr);
				break;
			case '1':
				arrow_1_glyph(cr);
				break;
		}
		cairo_restore(cr);
		memset(cell_type,0,5);
		cairo_restore(cr);
		cairosdl_surface_flush(cairo_get_target(cr));
		SDL_PushEvent (&event);
		printf("pushed event\n");
	}
	/*and_gate_glyph(cr);
	cairo_translate(cr,1.0,0);
	cairo_translate(cr,1.0,0);
	xor_gate_glyph(cr);
	cairo_translate(cr,1.0,0);
	or_gate_glyph(cr);
	cairo_translate(cr,1.0,0.5);
	stem_cell_glyph(cr);
	cairo_translate(cr,-1.0,0.5);
	crossover_cell_glyph(cr);
	cairo_translate(cr,-1.0,0);
	wire_cell_glyph(cr);
	cairo_translate(cr,-1.0,0);
	delete_cell_glyph(cr,3);
	cairo_translate(cr,-1.0,0);
	copy_cell_glyph(cr,3);
	cairo_translate(cr,0,1.0);
	arrow_none_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_x_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_0_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_1_glyph(cr);*/
}

int main(int argc, char **argv) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	if(SDLNet_Init() < 0) {
		fprintf(stderr, "Unable to init SDLNet: %s\n", SDLNet_GetError());
		exit(2);
	}
	atexit(SDLNet_Quit);
	
	SDL_Surface *screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RESIZABLE);
	SDL_Surface *canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
	cr = cairosdl_create(canvas);

	SDL_Thread *rendert = SDL_CreateThread(render_thread, argv);
	
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
				screen = SDL_SetVideoMode (event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_RESIZABLE);
				/*canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
				cr = cairosdl_create(canvas);
				fill_with_stems(event.resize.w, event.resize.h);*/
			case SDL_USEREVENT:
				SDL_BlitSurface(canvas, NULL, screen, NULL);
				SDL_Flip (screen);
				printf("Repainted screen\n");
				break;
		}
	}
	printf("Terminated: %s\n", SDL_GetError());
	SDL_KillThread(rendert);
	return 5;
}
