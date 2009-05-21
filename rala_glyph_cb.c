#include "rala_glyph_cb.h"
#include "rala_glyphs.h"
#include "rala_display.h"
#include "int_affine.h"
#include "commands.h"
#include <cairo/cairo.h>

void setup_arrow(cairo_t* cr, arrow_dir_t arrow_dir) {
	switch(arrow_dir) {
		case ARROW_DIR_W:
			cairo_translate(cr,-0.9,0.1);
			break;
		case ARROW_DIR_S:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,3*M_PI/2);
			cairo_translate(cr,-1.4,-0.4);
			break;
		case ARROW_DIR_E:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,M_PI);
			cairo_translate(cr,-1.4,-0.4);
			break;
		case ARROW_DIR_N:
			cairo_translate(cr,0.5,0.5);
			cairo_rotate(cr,M_PI/2);
			cairo_translate(cr,-1.4,-0.4);
			break;
	}
}

void rala_glyph_set_cell_cb(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
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

void rala_glyph_set_arrow_cb(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
	setup_arrow(cr,arrow_rotate(t, ((set_arrow_cb_t*)v)->arrow_dir));
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
