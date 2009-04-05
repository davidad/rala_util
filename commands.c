#include "commands.h"

void update_screen(cairo_t *cr) {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	cairosdl_surface_flush(cairo_get_target(cr));
	SDL_PushEvent(&event);
}

int next_command_char(char c, cairo_t* cr) {
	static enum {
		NORMAL,
		ARROW,
		X_COORD,
		Y_COORD
	} state_type = NORMAL;
	static int x_coord, y_coord;

	switch(state_type) {
		case NORMAL:
			switch(c) {
				case '[':
					cairo_save(cr);
					break;
				case ']':
					cairo_restore(cr);
					break;
				case 'n':
					blank_cell(cr);
					nand_gate_glyph(cr);
					update_screen(cr);
					break;
				case 'a':
					blank_cell(cr);
					and_gate_glyph(cr);
					update_screen(cr);
					break;
				case 'o':
					blank_cell(cr);
					or_gate_glyph(cr);
					update_screen(cr);
					break;
				case 'x':
					blank_cell(cr);
					xor_gate_glyph(cr);
					update_screen(cr);
					break;
				case 'c':
					blank_cell(cr);
					copy_cell_glyph(cr,0);
					update_screen(cr);
					break;
				case 'd':
					blank_cell(cr);
					delete_cell_glyph(cr,0);
					update_screen(cr);
					break;
				case 'w':
					blank_cell(cr);
					wire_cell_glyph(cr);
					update_screen(cr);
					break;
				case 'C':
					blank_cell(cr);
					crossover_cell_glyph(cr);
					update_screen(cr);
					break;
				case 'W':
					cairo_save(cr);
					cairo_translate(cr,-0.9,0.1);
					state_type = ARROW;
					break;
				case 'N':
					cairo_save(cr);
					cairo_translate(cr,0.5,0.5);
					cairo_rotate(cr,3*M_PI/2);
					cairo_translate(cr,-1.4,-0.4);
					state_type = ARROW;
					break;
				case 'E':
					cairo_save(cr);
					cairo_translate(cr,0.5,0.5);
					cairo_rotate(cr,M_PI);
					cairo_translate(cr,-1.4,-0.4);
					state_type = ARROW;
					break;
				case 'S':
					cairo_save(cr);
					cairo_translate(cr,0.5,0.5);
					cairo_rotate(cr,M_PI/2);
					cairo_translate(cr,-1.4,-0.4);
					state_type = ARROW;
					break;
				case '0':
					x_coord = 0;
					state_type = X_COORD;
					break;
				case '1':
					x_coord = 1;
					state_type = X_COORD;
					break;
				case '2':
					x_coord = 2;
					state_type = X_COORD;
					break;
				case '3':
					x_coord = 3;
					state_type = X_COORD;
					break;
				case '4':
					x_coord = 4;
					state_type = X_COORD;
					break;
				case '5':
					x_coord = 5;
					state_type = X_COORD;
					break;
				case '6':
					x_coord = 6;
					state_type = X_COORD;
					break;
				case '7':
					x_coord = 7;
					state_type = X_COORD;
					break;
				case '8':
					x_coord = 8;
					state_type = X_COORD;
					break;
				case '9':
					x_coord = 9;
					state_type = X_COORD;
					break;
				case 13:
				case 10:
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
			}
			state_type = NORMAL;
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
					state_type = Y_COORD;
					x_coord /= 10;
					y_coord = 0;
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
					state_type = NORMAL;
					cairo_translate(cr,x_coord*2,y_coord*2);
					break;
				default:
					fprintf(stderr, "Parse error while reading y coordinate\n");
					return 3;
					break;
			}
			break;
	}
	return 0;
}
