#include "rala_glyph_cb.h"
#include "commands.h"

void update_screen(cairo_t *cr) {
	SDL_Event event;
	cairosdl_surface_flush(cairo_get_target(cr));
	if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_USEREVENT)) == 0) {
		event.type = SDL_USEREVENT;
		SDL_PushEvent(&event);
	}
}

int next_command_char(char c, cairo_t* cr) {
	static enum {
		NORMAL,
		ARROW,
		X_COORD,
		Y_COORD,
		COMMENT,
		AFFINE,
		COMMAND,
		RANGE_X_MAX,
		RANGE_X_OFFSET,
		RANGE_Y_COORD,
		RANGE_Y_MIN,
		RANGE_Y_MAX,
		RANGE_Y_OFFSET
	} state = NORMAL;
	static arrow_dir_t arrow_dir;
	static int x_coord, y_coord, x_coord_sign, y_coord_sign;
	static affine_stack_t* transforms = NULL;
	static char command_buf[256];
	static int command_buf_i;
	if(transforms == NULL) transforms = affine_init();
	affine_par_t *par_iter;
	int x, y;
	set_cell_cb_t cell_cb_data;
	set_arrow_cb_t arrow_cb_data;
	cell_cb_data.cr = arrow_cb_data.cr = cr;

	switch(state) {
		case COMMENT:
			if(c == '\n') state = NORMAL;
			break;
		case COMMAND:
			if(c != '%') {
				command_buf[command_buf_i++] = c;
			} else {
				command_buf[command_buf_i] = '\0';
				if(!strcmp(command_buf, "clear") || !strcmp(command_buf, "CLEAR")) {
					clear(cr, cairo_image_surface_get_width(cairo_get_target(cr)), cairo_image_surface_get_height(cairo_get_target(cr)));
					update_screen(cr);
				}
				state = NORMAL;
			}
			break;
		case NORMAL:
			switch(c) {
				case '#':
					state = COMMENT;
					break;
				case '%':
					state = COMMAND;
					command_buf_i=0;
					break;
				case '~':
					state = AFFINE;
					break;
				case '{':
					affine_save(&transforms);
					break;
				case '}':
					if(affine_restore(&transforms)) {
						fprintf(stderr, "No transform to pop!\n");
					}
					break;
				case '[':
					affine_start_split(&transforms);
					break;
				case ']':
					affine_end_split(&transforms);
					break;
				case 'n':
					cell_cb_data.cell_type=CELL_TYPE_NAND;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'a':
					cell_cb_data.cell_type=CELL_TYPE_AND;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'o':
					cell_cb_data.cell_type=CELL_TYPE_OR;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'x':
					cell_cb_data.cell_type=CELL_TYPE_XOR;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'c':
					cell_cb_data.cell_type=CELL_TYPE_COPY_W;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'd':
					cell_cb_data.cell_type=CELL_TYPE_DELETE_W;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'w':
					cell_cb_data.cell_type=CELL_TYPE_WIRE;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'C':
					cell_cb_data.cell_type=CELL_TYPE_CROSSOVER;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 's':
					cell_cb_data.cell_type=CELL_TYPE_STEM;
					affine_operate(transforms,set_cell_cb,&cell_cb_data);
					update_screen(cr);
					break;
				case 'W':
				case 'N':
				case 'E':
				case 'S':
					if(applyd_x(transforms->cur,c)<0) {
						arrow_dir = ARROW_DIR_W;
					} else if(applyd_x(transforms->cur,c)>0) {
						arrow_dir = ARROW_DIR_E;
					} else if(applyd_y(transforms->cur,c)<0) {
						arrow_dir = ARROW_DIR_S;
					} else if(applyd_y(transforms->cur,c)>0) {
						arrow_dir = ARROW_DIR_N;
					}
					state = ARROW;
					break;
				case '-':
					x_coord = 0;
					x_coord_sign = -1;
					state = X_COORD;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					x_coord = c-'0';
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case 13:
				case 9:
				case 10:
				case 32:
					break;
				default:
					fprintf(stderr, "Invalid command character %c\n", c);
					return 1;
					break;
			}
			break;
		case ARROW:
			switch(c) {
				case '0':
					arrow_cb_data.arrow_type=ARROW_TYPE_0;
					arrow_cb_data.arrow_dir=arrow_dir;
					affine_operate(transforms,set_arrow_cb,&arrow_cb_data);
					update_screen(cr);
					break;
				case '1':
					arrow_cb_data.arrow_type=ARROW_TYPE_1;
					arrow_cb_data.arrow_dir=arrow_dir;
					affine_operate(transforms,set_arrow_cb,&arrow_cb_data);
					update_screen(cr);
					break;
				case 'x':
				case 'X':
					arrow_cb_data.arrow_type=ARROW_TYPE_X;
					arrow_cb_data.arrow_dir=arrow_dir;
					affine_operate(transforms,set_arrow_cb,&arrow_cb_data);
					update_screen(cr);
					break;
				case '_':
					arrow_cb_data.arrow_type=ARROW_TYPE_NONE;
					arrow_cb_data.arrow_dir=arrow_dir;
					affine_operate(transforms,set_arrow_cb,&arrow_cb_data);
					update_screen(cr);
					break;
				default:
					cairo_restore(cr);
					break;
			}
			state = NORMAL;
			break;
		case X_COORD:
			x_coord *= 10;
			switch(c) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					x_coord += c-'0';
					break;
				case ',':
					state = Y_COORD;
					x_coord /= 10;
					x_coord *= x_coord_sign;
					y_coord = 0;
					y_coord_sign = 1;
					break;
				case ':':
					if(transforms->par_cur == NULL) {
						printf("range invalid outside []s\n");
						return 4;
					} else {
						x_coord /= 10;
						x_coord *= x_coord_sign;
						transforms->par_cur->x_range_min=x_coord;
						x_coord = 0;
						x_coord_sign = 1;
						state=RANGE_X_MAX;
					}
					break;
				default:
					fprintf(stderr, "Parse error while reading x coordinate\n");
					return 2;
					break;
			}
			break;
		case RANGE_X_MAX:
			x_coord *= 10;
			switch(c) {
				case '-':
					x_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					x_coord += (c-'0');
					break;
				case ',':
					x_coord /= 10;
					x_coord *= x_coord_sign;
					transforms->par_cur->x_range_max = x_coord;
					transforms->par_cur->x_offset = 1;
					y_coord = 0;
					y_coord_sign = 1;
					state = RANGE_Y_COORD;
					break;
				case '*':
					x_coord /= 10;
					x_coord *= x_coord_sign;
					transforms->par_cur->x_range_max = x_coord;
					x_coord = 0;
					x_coord_sign = 1;
					state = RANGE_X_OFFSET;
					break;
				default:
					fprintf(stderr, "Parse error while reading range\n");
					return 2;
					break;
			}
			break;
		case RANGE_X_OFFSET:
			x_coord *= 10;
			switch(c) {
				case '-':
					x_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					x_coord += (c-'0');
					break;
				case ',':
					x_coord /= 10;
					x_coord *= x_coord_sign;
					transforms->par_cur->x_offset = x_coord;
					y_coord = 0;
					y_coord_sign = 1;
					state = RANGE_Y_COORD;
					break;
				default:
					fprintf(stderr, "Parse error while reading range\n");
					return 2;
					break;
			}
			break;
		case RANGE_Y_COORD:
			y_coord *= 10;
			switch(c) {
				case '-':
					y_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					y_coord += (c-'0');
					break;
				case ':':
					if(transforms->par_cur == NULL) {
						printf("range invalid outside []s\n");
						return 4;
					} else {
						y_coord /= 10;
						y_coord *= y_coord_sign;
						transforms->par_cur->y_range_min=y_coord;
						y_coord = 0;
						y_coord_sign = 1;
						state=RANGE_Y_MAX;
					}
					break;
				case ';':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					transforms->par_cur->y_range_min=y_coord;
					transforms->par_cur->y_range_max=y_coord;
					transforms->par_cur->y_offset=1;
					state = NORMAL;
					break;
				default:
					fprintf(stderr, "Parse error while reading y coordinate\n");
					return 3;
					break;
			}
			break;
		case RANGE_Y_MAX:
			y_coord *= 10;
			switch(c) {
				case '-':
					y_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					y_coord += (c-'0');
					break;
				case '*':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					transforms->par_cur->y_range_max=y_coord;
					y_coord = 0;
					y_coord_sign = 1;
					state = RANGE_Y_OFFSET;
					break;
				case ';':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					transforms->par_cur->y_range_max=y_coord;
					transforms->par_cur->y_offset=1;
					state = NORMAL;
					break;
				default:
					fprintf(stderr, "Parse error while reading y coordinate\n");
					return 3;
					break;
			}
			break;
		case RANGE_Y_OFFSET:
			y_coord *= 10;
			switch(c) {
				case '-':
					y_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					y_coord += (c-'0');
					break;
				case ';':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					transforms->par_cur->y_offset=y_coord;
					state = NORMAL;
					break;
				default:
					fprintf(stderr, "Parse error while reading y coordinate\n");
					return 3;
					break;
			}
			break;
		case Y_COORD:
			y_coord *= 10;
			switch(c) {
				case '-':
					y_coord_sign = -1;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					y_coord += (c-'0');
					break;
				case ':':
					if(transforms->par_cur == NULL) {
						printf("range invalid outside []s\n");
						return 4;
					} else {
						transforms->par_cur->x_range_min=x_coord;
						transforms->par_cur->x_range_max=x_coord;
						transforms->par_cur->x_offset=1;
						transforms->par_cur->y_range_min=y_coord;
						y_coord = 0;
						y_coord_sign = 1;
						state=RANGE_Y_MAX;
					}
					break;
				case ';':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					affine_transform(&transforms, affine_translate(x_coord,y_coord));
					state = NORMAL;
					break;
				default:
					fprintf(stderr, "Parse error while reading y coordinate\n");
					return 3;
					break;
			}
			break;
		case AFFINE:
			switch(c) {
				case '|':
				case 'x':
					affine_transform(&transforms, flip_x);
					state=NORMAL;
					break;
				case '_':
				case 'y':
					affine_transform(&transforms, flip_y);
					state=NORMAL;
					break;
				case '\'':
				case 'n':
					affine_transform(&transforms, rot90);
					state=NORMAL;
					break;
				case '-':
				case 'w':
					affine_transform(&transforms, rot180);
					state=NORMAL;
					break;
				case ',':
				case 's':
					affine_transform(&transforms, rot270);
					state=NORMAL;
					break;
				case '\\':
					affine_transform(&transforms, flip_neg_diag);
					state=NORMAL;
					break;
				case '/':
					affine_transform(&transforms, flip_pos_diag);
					state=NORMAL;
					break;
				default:
					fprintf(stderr, "Unknown affine transform\n");
					break;
			}
			break;
	}
	return 0;
}
