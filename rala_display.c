#include <stdlib.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>
#include <cairo/cairo.h>
#include <cairo/cairo-ps.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>
#include "rala_glyphs.h"
#include "rala_glyph_cb.h"
#include "cairosdl.h"
#include "commands.h"

cairo_t *cr;
SDL_Thread *rendert;
int width = -1;
int height = -1;
int center_x = 0;
int center_y = 0;
enum output_type {TO_SDL, TO_SDL_PNGS, TO_PNG, TO_PS, TO_EPS, TO_PDF, TO_SVG, TO_SOCKET, TO_FILE, OUTPUT_TYPE_LENGTH} output = TO_SDL;
enum input_type {FROM_SOCKET, FROM_FILE, INPUT_TYPE_LENGTH} input = FROM_SOCKET;
int output_map = 0;
FILE* to_file = NULL;
char* to_filename = NULL;
char* to_hostname = NULL;
int to_port = 0;
FILE* from_file = NULL;
char* from_hostname = NULL;
int from_port = 0;
int cell_size = 40;

void set_canvas_cairosdl(SDL_Surface **screen, SDL_Surface **canvas) {
	if(width==-1) width=1024;
	if(height==-1) height=1024;
	*screen = SDL_SetVideoMode(width,height,32,SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RESIZABLE);
	*canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
	cr = cairosdl_create(*canvas);
}

void set_canvas_cairo_image() {
	if(width==-1) width=1024;
	if(height==-1) height=1024;
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(surface);
}

void set_canvas_cairo_ps() {
	if(width==-1) width=612;
	if(height==-1) height=792;
	cairo_surface_t *surface;
	surface = cairo_ps_surface_create(to_filename, (double)width, (double)height);
	cr = cairo_create(surface);
}

void set_canvas_cairo_eps() {
	if(width==-1) width=600;
	if(height==-1) height=600;
	cairo_surface_t *surface;
	surface = cairo_ps_surface_create(to_filename, (double)width, (double)height);
	cairo_ps_surface_set_eps(surface, 1);
	cr = cairo_create(surface);
}

void set_canvas_cairo_pdf() {
	if(width==-1) width=600;
	if(height==-1) height=600;
	cairo_surface_t *surface;
	surface = cairo_pdf_surface_create(to_filename, (double)width, (double)height);
	cr = cairo_create(surface);
}

void set_canvas_cairo_svg() {
	if(width==-1) width=600;
	if(height==-1) height=600;
	cairo_surface_t *surface;
	surface = cairo_svg_surface_create(to_filename, (double)width, (double)height);
	cr = cairo_create(surface);
}

void updater_nop(void *cl) {
}

void updater_sdl(void *cl) {
	cairo_t* cr = (cairo_t*) cl;
	SDL_Event event;
	cairosdl_surface_flush(cairo_get_target(cr));
	if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_USEREVENT)) == 0) {
		event.type = SDL_USEREVENT;
		SDL_PushEvent(&event);
	}
}

void updater_sdl_pngs(void* cl) {
	cairo_t* cr = (cairo_t*) cl;
	static int filenum = 0;
	SDL_Event event;
	cairosdl_surface_flush(cairo_get_target(cr));
	if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_USEREVENT)) == 0) {
		event.type = SDL_USEREVENT;
		SDL_PushEvent(&event);
	}
	char filename[256];
	sprintf(filename, "%s%.5d.png\0", to_filename, filenum++);
	cairo_surface_write_to_png(cairo_get_target(cr), filename);
}

void updater_ps(void* cl) {
	cairo_t* cr = (cairo_t*) cl;
	cairo_copy_page(cr);
}

void atexit_write_png(void) {
	cairo_surface_write_to_png(cairo_get_target(cr), to_filename);
}

void atexit_write_ps(void) {
	cairo_surface_destroy(cairo_get_target(cr));
	cairo_surface_finish(cairo_get_target(cr));
}

void atexit_write_pdf(void) {
	cairo_show_page(cr);
	cairo_surface_destroy(cairo_get_target(cr));
	cairo_surface_finish(cairo_get_target(cr));
}

updater_t updaters[OUTPUT_TYPE_LENGTH];

void do_output_map(void) {
	int w = cairo_image_surface_get_width(cairo_get_target(cr));
	int h = cairo_image_surface_get_height(cairo_get_target(cr));
	if(blank_cell_style > NONE) {
		double user_w = w;
		double user_h = h;
		cairo_device_to_user_distance(cr, &user_w, &user_h);
		int user_w_int = ceil(user_w)+1;
		int user_h_int = ceil(user_h)+1;
		double user_x = 0.0;
		double user_y = 0.0;
		cairo_device_to_user(cr, &user_x, &user_y);
		int user_x_int = floor(user_x/2)*2;
		int user_y_int = floor(user_y/2)*2;
		int x,y;
		double cell_size = 1.0, dummy =0.0;
		cairo_user_to_device_distance(cr, &cell_size, &dummy);
		double half_cell_size = cell_size/2;
		for(y=user_y_int; y<user_y_int+user_h_int; y+=2) {
			for(x=user_x_int; x<user_x_int+user_w_int; x+=2) {
				int xc = x/2;
				int yc = -y/2;
				double x0, y0, x1, y1, xo = (double)x, yo = (double)y;
				cairo_user_to_device(cr, &xo, &yo);
				x1 = xo+cell_size;
				y1 = yo+cell_size;
				x0 = (xo<0)?0:xo;
				y0 = (yo<0)?0:yo;
				x1 = (x1>w)?w:x1;
				y1 = (y1>h)?h:y1;
				printf("<area shape=\"rect\" class=\"rala_area\" coords=\"%.0f,%.0f,%.0f,%.0f\" id=\"%d,%d;\" onmouseover=\"ralaMouseOver('%d,%d;',event)\" onmouseout=\"ralaMouseOut('%d,%d;',event)\" onclick=\"ralaClick('%d,%d;',event)\" onkeydown=\"ralaKeyDown('%d,%d;',event)\">\n", x0, y0, x1, y1, xc, yc, xc, yc, xc, yc, xc, yc, xc, yc);
				xo-=cell_size;
				x1=x0;
				y1=yo+half_cell_size;
				x0 = (xo<0)?0:xo;
				y0 = (yo<0)?0:yo;
				x1 = (x1>w)?w:x1;
				y1 = (y1>h)?h:y1;
				printf("<area shape=\"rect\" class=\"rala_area\" coords=\"%.0f,%.0f,%.0f,%.0f\" id=\"%d,%d;W\" onmouseover=\"ralaMouseOver('%d,%d;W',event)\" onmouseout=\"ralaMouseOut('%d,%d;W',event)\" onclick=\"ralaClick('%d,%d;W',event)\" onkeydown=\"ralaKeyDown('%d,%d;W',event)\">\n", x0, y0, x1, y1, xc, yc, xc, yc, xc, yc, xc, yc, xc, yc);
				xo+=cell_size+half_cell_size;
				yo-=cell_size;
				x1+=cell_size;
				y1=yo+cell_size;
				x0 = (xo<0)?0:xo;
				y0 = (yo<0)?0:yo;
				x1 = (x1>w)?w:x1;
				y1 = (y1>h)?h:y1;
				printf("<area shape=\"rect\" class=\"rala_area\" coords=\"%.0f,%.0f,%.0f,%.0f\" id=\"%d,%d;N\" onmouseover=\"ralaMouseOver('%d,%d;N',event)\" onmouseout=\"ralaMouseOut('%d,%d;N',event)\" onclick=\"ralaClick('%d,%d;N',event)\" onkeydown=\"ralaKeyDown('%d,%d;N',event)\">\n", x0, y0, x1, y1, xc, yc, xc, yc, xc, yc, xc, yc, xc, yc);
				xo+=half_cell_size;
				yo+=cell_size+half_cell_size;
				x1+=cell_size;
				y1+=cell_size;
				x0 = (xo<0)?0:xo;
				y0 = (yo<0)?0:yo;
				x1 = (x1>w)?w:x1;
				y1 = (y1>h)?h:y1;
				printf("<area shape=\"rect\" class=\"rala_area\" coords=\"%.0f,%.0f,%.0f,%.0f\" id=\"%d,%d;E\" onmouseover=\"ralaMouseOver('%d,%d;E',event)\" onmouseout=\"ralaMouseOut('%d,%d;E',event)\" onclick=\"ralaClick('%d,%d;E',event)\" onkeydown=\"ralaKeyDown('%d,%d;E',event)\">\n", x0, y0, x1, y1, xc, yc, xc, yc, xc, yc, xc, yc, xc, yc);
				xo-=cell_size;
				yo+=half_cell_size;
				x1=xo+half_cell_size;
				y1+=cell_size;
				x0 = (xo<0)?0:xo;
				y0 = (yo<0)?0:yo;
				x1 = (x1>w)?w:x1;
				y1 = (y1>h)?h:y1;
				printf("<area shape=\"rect\" class=\"rala_area\" coords=\"%.0f,%.0f,%.0f,%.0f\" id=\"%d,%d;S\" onmouseover=\"ralaMouseOver('%d,%d;S',event)\" onmouseout=\"ralaMouseOut('%d,%d;S',event)\" onclick=\"ralaClick('%d,%d;S',event)\" onkeydown=\"ralaKeyDown('%d,%d;S',event)\">\n", x0, y0, x1, y1, xc, yc, xc, yc, xc, yc, xc, yc, xc, yc);
			}
		}
	}
}

void set_up_drawing_environment(void) {
	if(output == TO_PNG) {
		cairo_set_source_rgba(cr,1.0,1.0,1.0,1.0);
	} else {
		cairo_set_source_rgb(cr,1.0,1.0,1.0);
	}
	cairo_paint(cr);
	
	cairo_translate(cr, width/2-cell_size/2, height/2-cell_size/2);
	cairo_scale (cr, cell_size, cell_size);
	cairo_translate(cr, -center_x, center_y);

	if(output_map) do_output_map();
	clear(cr);
	updaters[output](cr);
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

	//Wait for commands
	char buf;
	while(SDLNet_TCP_Recv(sock, &buf, 1) > 0) {
		next_command_char(buf, cr, rala_glyph_set_cell_cb, rala_glyph_set_arrow_cb, clear, updaters[output]);
	}
}

int from_file_render_thread (void* data) {
	set_up_drawing_environment();

	//Read commands
	char buf;
	while((buf = fgetc(from_file)) != EOF) {
		next_command_char(buf, cr, rala_glyph_set_cell_cb, rala_glyph_set_arrow_cb, clear, updaters[output]);
	}
	if(output != TO_SDL && output != TO_SDL_PNGS) {
		exit(0);
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

void init_net(void) {
	if(SDL_Init(0) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	if(SDLNet_Init() < 0) {
		fprintf(stderr, "Unable to init SDLNet: %s\n", SDLNet_GetError());
		exit(2);
	}
	atexit(SDLNet_Quit);
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
				/*canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, CAIROSDL_RMASK, CAIROSDL_GMASK, CAIROSDL_BMASK, 0);
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
		else if(!strncmp(argv[i], "--center-x=", 10)) {
			center_x=2*atoi(strchr(argv[i],'=')+1);
			if(strstr(argv[i], ".5")) {
				center_x++;
			}
		}
		else if(!strncmp(argv[i], "--center-y=", 10)) {
			center_y=2*atoi(strchr(argv[i],'=')+1);
			if(strstr(argv[i], ".5")) {
				center_y++;
			}
		}
		else if(!strncmp(argv[i], "--width=", 8)) {
			width=atoi(strchr(argv[i],'=')+1);
		}
		else if(!strncmp(argv[i], "--height=", 9)) {
			height=atoi(strchr(argv[i],'=')+1);
		}
		else if(!strncmp(argv[i], "--cell-size=", 12)) {
			cell_size=atoi(strchr(argv[i],'=')+1);
		}
		else if(!strcmp(argv[i], "--to=sdl")) {
			output=TO_SDL;
		}
		else if(!strcmp(argv[i], "--to=sdl+pngs")) {
			output=TO_SDL_PNGS;
			if(i+1>=argc) printf("no basename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=png")) {
			output=TO_PNG;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=eps")) {
			output=TO_EPS;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=ps")) {
			output=TO_PS;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=pdf")) {
			output=TO_PDF;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=svg")) {
			output=TO_SVG;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
		}
		else if(!strcmp(argv[i], "--to=file")) {
			input=TO_FILE;
			if(i+1>=argc) printf("no filename specified\n");
			to_filename=argv[++i];
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
		else if(!strcmp(argv[i], "--output-map")) {
			output_map = 1;
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
		else if(to_file = fopen(argv[i],"a")) {
			output = TO_FILE;
		}
		i++;
	}

	updaters[TO_SDL]=updater_sdl;
	updaters[TO_SDL_PNGS]=updater_sdl_pngs;
	updaters[TO_PS]=updater_ps;
	updaters[TO_PDF]=updater_nop;
	updaters[TO_EPS]=updater_nop;
	updaters[TO_PNG]=updater_nop;
	updaters[TO_SVG]=updater_nop;
	if(output == TO_FILE || output == TO_SOCKET) {
		printf("sorry, this output method is not implemented. defaulting to SDL output.\n");
		output = TO_SDL;
	}
	if(from_port == 0 && from_file == NULL) {
		from_file = stdin;
		input = FROM_FILE;
	}
	SDL_Surface *screen, *canvas;
	if (output == TO_SDL || output == TO_SDL_PNGS) {
		init_video();
		if(input==FROM_SOCKET || output==TO_SOCKET) {
			init_video_net();
		}
	} else if(input==FROM_SOCKET || output==TO_SOCKET) {
			init_net();
	}
	if (output == TO_SDL || output == TO_SDL_PNGS) {
		set_canvas_cairosdl(&screen, &canvas);
	} else if (output == TO_PNG) {
		set_canvas_cairo_image();
		atexit(atexit_write_png);
	} else if (output == TO_PS) {
		set_canvas_cairo_ps();
		atexit(atexit_write_ps);
	} else if (output == TO_EPS) {
		set_canvas_cairo_eps();
		atexit(atexit_write_ps);
	} else if (output == TO_PDF) {
		set_canvas_cairo_pdf();
		atexit(atexit_write_pdf);
	} else if (output == TO_SVG) {
		set_canvas_cairo_svg();
		atexit(atexit_write_ps);
	}
	if(input==FROM_SOCKET) {
		rendert = SDL_CreateThread(from_socket_render_thread, argv);
	} else if (input==FROM_FILE) {
		if(output == TO_SDL || output == TO_SDL_PNGS) {
			rendert = SDL_CreateThread(from_file_render_thread, argv);
		} else {
			from_file_render_thread(argv);
		}
	}
	if(output==TO_SDL || output == TO_SDL_PNGS) {
		sdl_event_loop(&screen, &canvas);
	}
		
	return 0;
}
