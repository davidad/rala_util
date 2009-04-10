#include "commands.h"

void update_screen(cairo_t *cr) {
	SDL_Event event;
	cairosdl_surface_flush(cairo_get_target(cr));
	if(SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_USEREVENT)) == 0) {
		event.type = SDL_USEREVENT;
		SDL_PushEvent(&event);
	}
}

void cairo_setup_W(cairo_t* cr) {
	cairo_translate(cr,-0.9,0.1);
}

void cairo_setup_S(cairo_t* cr) {
	cairo_translate(cr,0.5,0.5);
	cairo_rotate(cr,M_PI/2);
	cairo_translate(cr,-1.4,-0.4);
}

void cairo_setup_E(cairo_t* cr) {
	cairo_translate(cr,0.5,0.5);
	cairo_rotate(cr,M_PI);
	cairo_translate(cr,-1.4,-0.4);
}

void cairo_setup_N(cairo_t* cr) {
	cairo_translate(cr,0.5,0.5);
	cairo_rotate(cr,3*M_PI/2);
	cairo_translate(cr,-1.4,-0.4);
}

int next_command_char(char c, cairo_t* cr) {
	static enum {
		NORMAL,
		ARROW,
		X_COORD,
		Y_COORD,
		COMMENT,
		AFFINE,
		COMMAND
	} state = NORMAL;
	static int x_coord, y_coord, x_coord_sign, y_coord_sign;
	static affine_stack_t* transforms = NULL;
	static char command_buf[256];
	static int command_buf_i;
	if(transforms == NULL) transforms = affine_init();

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
				case '[':
					affine_save(&transforms);
					break;
				case ']':
					if(affine_restore(&transforms)) {
						fprintf(stderr, "No transform to pop!\n");
					}
					break;
				case 'n':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					nand_gate_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'a':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					and_gate_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'o':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					or_gate_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'x':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					xor_gate_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'c':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					copy_cell_glyph(cr,0);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'd':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					delete_cell_glyph(cr,0);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'w':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					wire_cell_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'C':
					cairo_save(cr);
					cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
					blank_cell(cr);
					crossover_cell_glyph(cr);
					update_screen(cr);
					cairo_restore(cr);
					break;
				case 'W':
					if(applyv_x(transforms->cur,-1,0)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_W(cr);
					} else if(applyv_x(transforms->cur,-1,0)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_E(cr);
					} else if(applyv_y(transforms->cur,-1,0)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_S(cr);
					} else if(applyv_y(transforms->cur,-1,0)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_N(cr);
					}
					state = ARROW;
					break;
				case 'N':
					if(applyv_x(transforms->cur,0,1)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_W(cr);
					} else if(applyv_x(transforms->cur,0,1)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_E(cr);
					} else if(applyv_y(transforms->cur,0,1)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_S(cr);
					} else if(applyv_y(transforms->cur,0,1)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_N(cr);
					}
					state = ARROW;
					break;
				case 'E':
					if(applyv_x(transforms->cur,1,0)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_W(cr);
					} else if(applyv_x(transforms->cur,1,0)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_E(cr);
					} else if(applyv_y(transforms->cur,1,0)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_S(cr);
					} else if(applyv_y(transforms->cur,1,0)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_N(cr);
					}
					state = ARROW;
					break;
				case 'S':
					if(applyv_x(transforms->cur,0,-1)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_W(cr);
					} else if(applyv_x(transforms->cur,0,-1)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_E(cr);
					} else if(applyv_y(transforms->cur,0,-1)<0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_S(cr);
					} else if(applyv_y(transforms->cur,0,-1)>0) {
						cairo_save(cr);
						cairo_translate(cr,transforms->cur.wx,transforms->cur.wy);
						cairo_setup_N(cr);
					}
					state = ARROW;
					break;
				case '-':
					x_coord = 0;
					x_coord_sign = -1;
					state = X_COORD;
					break;
				case '0':
					x_coord = 0;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '1':
					x_coord = 1;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '2':
					x_coord = 2;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '3':
					x_coord = 3;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '4':
					x_coord = 4;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '5':
					x_coord = 5;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '6':
					x_coord = 6;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '7':
					x_coord = 7;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '8':
					x_coord = 8;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case '9':
					x_coord = 9;
					x_coord_sign = 1;
					state = X_COORD;
					break;
				case 13:
				case 10:
				case 32:
					break;
				default:
					fprintf(stderr, "Invalid command %d\n", c);
					return 1;
					break;
			}
			break;
		case ARROW:
			switch(c) {
				case '0':
					arrow_0_glyph(cr);
					cairo_restore(cr);
					update_screen(cr);
					break;
				case '1':
					arrow_1_glyph(cr);
					cairo_restore(cr);
					update_screen(cr);
					break;
				case 'x':
				case 'X':
					arrow_x_glyph(cr);
					cairo_restore(cr);
					update_screen(cr);
					break;
				case '_':
					arrow_none_glyph(cr);
					cairo_restore(cr);
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
					break;
				case '1':
					x_coord += 1;
					break;
				case '2':
					x_coord += 2;
					break;
				case '3':
					x_coord += 3;
					break;
				case '4':
					x_coord += 4;
					break;
				case '5':
					x_coord += 5;
					break;
				case '6':
					x_coord += 6;
					break;
				case '7':
					x_coord += 7;
					break;
				case '8':
					x_coord += 8;
					break;
				case '9':
					x_coord += 9;
					break;
				case ',':
					state = Y_COORD;
					x_coord /= 10;
					x_coord *= x_coord_sign;
					y_coord = 0;
					y_coord_sign = 1;
					break;
				default:
					fprintf(stderr, "Parse error while reading x coordinate\n");
					return 2;
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
					break;
				case '1':
					y_coord += 1;
					break;
				case '2':
					y_coord += 2;
					break;
				case '3':
					y_coord += 3;
					break;
				case '4':
					y_coord += 4;
					break;
				case '5':
					y_coord += 5;
					break;
				case '6':
					y_coord += 6;
					break;
				case '7':
					y_coord += 7;
					break;
				case '8':
					y_coord += 8;
					break;
				case '9':
					y_coord += 9;
					break;
				case ';':
					y_coord /= 10;
					y_coord *= y_coord_sign;
					state = NORMAL;
					affine_transform(&transforms, affine_translate(x_coord*2,y_coord*2));
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
