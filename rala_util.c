#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>
#include "rala_glyphs.h"
#include "cairosdl.h"
#include "commands.h"

#define WIDTH 1024
#define HEIGHT 1024

cairo_t *cr;
SDL_Thread *rendert;
char* from_hostname = NULL;
int from_port = 0;
FILE* from_file = NULL;
int cell_size = 30;

void set_up_drawing_environment(void) {
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	cairo_paint(cr);
	
	cairo_translate(cr, WIDTH/2-cell_size/2, HEIGHT/2-cell_size/2);
	cairo_scale (cr, cell_size, cell_size);

	clear(cr, WIDTH, HEIGHT);
	cairosdl_surface_flush(cairo_get_target(cr));
	SDL_Event event;
	event.type = SDL_USEREVENT;
	SDL_PushEvent (&event);

}

int from_socket_render_thread (void* data) {
	set_up_drawing_environment();

	// Set up socket
	char** argv = (char**)data;
	IPaddress ip;
	TCPsocket server_sock;
	if(SDLNet_ResolveHost(&ip,from_hostname,from_port) < 0) {
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
	printf("Got connection.\n");

	//Wait for commands
	char buf;
	while(SDLNet_TCP_Recv(sock, &buf, 1) > 0) {
		next_command_char(buf, cr);
	}
}

int from_file_render_thread (void* data) {
	set_up_drawing_environment();

	//Read commands
	char buf;
	while((buf = fgetc(from_file)) != EOF) {
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
				clear(cr, event.resize.w, event.resize.h);*/
			case SDL_USEREVENT:
				SDL_BlitSurface(*canvas, NULL, *screen, NULL);
				SDL_Flip (*screen);
				break;
		}
	}
	printf("Terminated: %s\n", SDL_GetError());
	SDL_KillThread(rendert);
	exit(5);
}

void print_help(char** argv) {
	printf("Usage:\n\t%s [--blank=stem|hollow|none] [--to=sdl|png filename|ps filename|pdf filename|file filename|listen-on port|connect-to host port] [--from=file filename|listen-on port|connect-to host port]\n",argv[0]);
	exit(0);
}

int main(int argc, char **argv) {
	enum {TO_SDL, TO_PNG, TO_PS, TO_PDF, TO_SOCKET, TO_FILE} output = TO_SDL;
	enum {FROM_SOCKET, FROM_FILE} input = FROM_SOCKET;
	FILE* to_file = NULL;
	char* to_hostname = NULL;
	int to_port = 0;
	int i = 1;
	char* c;
	while(i < argc) {
		if(!strcmp(argv[i], "--blank=stem")) {
			blank_cell_style=STEM;
		}
		else if(!strcmp(argv[i], "--blank=hollow")) {
			blank_cell_style=HOLLOW_STEM;
		}
		else if(!strcmp(argv[i], "--blank=none")) {
			blank_cell_style=NONE;
		}
		else if(!strncmp(argv[i], "--cell-size=", 12)) {
			cell_size=atoi(strchr(argv[i],'=')+1);
		}
		else if(!strcmp(argv[i], "--to=sdl")) {
			output=TO_SDL;
		}
		else if(!strcmp(argv[i], "--to=png")) {
			output=TO_PNG;
			if(i+1>=argc) printf("no filename specified\n");
			to_file=fopen(argv[++i], "w");
		}
		else if(!strcmp(argv[i], "--to=ps")) {
			output=TO_PS;
			if(i+1>=argc) printf("no filename specified\n");
			to_file=fopen(argv[++i], "w");
		}
		else if(!strcmp(argv[i], "--to=pdf")) {
			output=TO_PDF;
			if(i+1>=argc) printf("no filename specified\n");
			to_file=fopen(argv[++i], "w");
		}
		else if(!strcmp(argv[i], "--to=file")) {
			input=TO_FILE;
			if(i+1>=argc) printf("no filename specified\n");
			to_file=fopen(argv[++i], "w");
		}
		else if(!strcmp(argv[i], "--to=listen-on")) {
			input=TO_SOCKET;
			if(i+1>=argc) printf("no port specified\n");
			to_hostname = NULL;
			to_port = atoi(argv[++i]);
		}
		else if(!strcmp(argv[i], "--to=connect-to")) {
			input=TO_SOCKET;
			if(i+1>=argc) printf("no hostname specified\n");
			to_hostname = argv[++i];
			if(i+1>=argc) printf("no port specified\n");
			to_port = atoi(argv[++i]);
		}
		else if(!strcmp(argv[i], "--from=file")) {
			input=FROM_FILE;
			if(i+1>=argc) printf("no filename specified\n");
			from_file=fopen(argv[++i], "r");
			if(from_file==NULL) printf("couldn't open file %s\n",argv[i]);
		}
		else if(!strcmp(argv[i], "--from=listen-on")) {
			input=FROM_SOCKET;
			from_hostname = NULL;
			if(i+1>=argc) printf("no port specified\n");
			from_port = atoi(argv[++i]);
		}
		else if(!strcmp(argv[i], "--from=connect-to")) {
			input=FROM_SOCKET;
			if(i+1>=argc) printf("no hostname specified\n");
			from_hostname = argv[++i];
			if(i+1>=argc) printf("no port specified\n");
			from_port = atoi(argv[++i]);
		}
		else if(!strcmp(argv[i], "--help")) {
			print_help(argv);
		}
		else if(c = strchr(argv[i], ':')) {
			if(from_port == 0 && from_file == NULL) {
				input=FROM_SOCKET;
				if(c==argv[i]) {
					from_hostname = NULL;
				} else {
					from_hostname = malloc(c-argv[i]);
					memcpy(argv[i],from_hostname,c-argv[i]);
				}
				from_port = atoi(c+1);
			} else {
				input=TO_SOCKET;
				to_hostname = malloc(c-argv[i]);
				memcpy(argv[i],to_hostname,c-argv[i]);
				to_port = atoi(c);
			}
		}
		else if(from_port == 0 && from_file == NULL && (from_file = fopen(argv[i],"r"))) {
			input = FROM_FILE;
		}
		else if(to_file = fopen(argv[i],"r")) {
			output = TO_FILE;
		}
		i++;
	}
	if(output != TO_SDL) {
		printf("sorry, this output method is not implemented. defaulting to SDL output.\n");
		output = TO_SDL;
	}
	if(from_port == 0 && from_file == NULL) {
		from_file = stdin;
		input = FROM_FILE;
	}
	SDL_Surface *screen, *canvas;
	if(input==FROM_SOCKET || output==TO_SOCKET) {
		init_video_net();
	} else {
		init_video();
	}
	set_canvas_cairosdl(&screen, &canvas);
	if(input==FROM_SOCKET) {
		rendert = SDL_CreateThread(from_socket_render_thread, argv);
	} else if (input==FROM_FILE) {
		rendert = SDL_CreateThread(from_file_render_thread, argv);
	}
	sdl_event_loop(&screen, &canvas);
		
	return 0;
}
