#include "rala_glyph_cb.h"
#include "rala_glyphs.h"

void setup_arrow(cairo_t* cr, arrow_dir_t arrow_dir) {
	switch(arrow_dir) {
		case ARROW_DIR_W:
			cairo_translate(cr,-0.9,0.1);
			break;
		case ARROW_DIR_S:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,M_PI/2);
			cairo_translate(cr,-1.4,-0.4);
			break;
		case ARROW_DIR_E:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,M_PI);
			cairo_translate(cr,-1.4,-0.4);
			break;
		case ARROW_DIR_N:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,3*M_PI/2);
			cairo_translate(cr,-1.4,-0.4);
			break;
	}
}

void set_cell_cb(void* v, int x, int y) {
	cairo_t *cr = ((set_cell_cb_t*)v)->cr;
	cairo_save(cr);
	cairo_translate(cr,2*x, 2*y);
	blank_cell(cr);
	switch(((set_cell_cb_t*)v)->cell_type) {
		case CELL_TYPE_BLANK:
			break;
		case CELL_TYPE_STEM:
			stem_cell_glyph(cr);
			break;
		case CELL_TYPE_AND:
			and_gate_glyph(cr);
			break;
		case CELL_TYPE_OR:
			or_gate_glyph(cr);
			break;
		case CELL_TYPE_XOR:
			xor_gate_glyph(cr);
			break;
		case CELL_TYPE_NAND:
			nand_gate_glyph(cr);
			break;
		case CELL_TYPE_WIRE:
			wire_cell_glyph(cr);
			break;
		case CELL_TYPE_CROSSOVER:
			crossover_cell_glyph(cr);
			break;
		case CELL_TYPE_COPY_E:
			copy_cell_glyph(cr,2);
			break;
		case CELL_TYPE_COPY_N:
			copy_cell_glyph(cr,3);
			break;
		case CELL_TYPE_COPY_W:
			copy_cell_glyph(cr,0);
			break;
		case CELL_TYPE_COPY_S:
			copy_cell_glyph(cr,1);
			break;
		case CELL_TYPE_DELETE_E:
			delete_cell_glyph(cr,2);
			break;
		case CELL_TYPE_DELETE_N:
			delete_cell_glyph(cr,3);
			break;
		case CELL_TYPE_DELETE_W:
			delete_cell_glyph(cr,0);
			break;
		case CELL_TYPE_DELETE_S:
			delete_cell_glyph(cr,1);
			break;
	}
	cairo_restore(cr);
}

void set_arrow_cb(void* v, int x, int y) {
	cairo_t *cr = ((set_arrow_cb_t*)v)->cr;
	cairo_save(cr);
	cairo_translate(cr,2*x, 2*y);
	setup_arrow(cr,((set_arrow_cb_t*)v)->arrow_dir);
	switch(((set_arrow_cb_t*)v)->arrow_type) {
		case ARROW_TYPE_NONE:
			arrow_none_glyph(cr);
			break;
		case ARROW_TYPE_X:
			arrow_x_glyph(cr);
			break;
		case ARROW_TYPE_0:
			arrow_0_glyph(cr);
			break;
		case ARROW_TYPE_1:
			arrow_1_glyph(cr);
			break;
	}
	cairo_restore(cr);
}
