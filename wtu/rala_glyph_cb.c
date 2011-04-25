#include "rala_glyph_cb.h"
#include "rala_glyphs.h"
#include "rala_display.h"
#include "rala_enums.h"
#include "int_affine.h"
#include "commands.h"
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>

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

void rala_glyph_set_cell_cb_day(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;
  static cairo_pattern_t* memoized_pattern[CELL_TYPE_MAX];
  static int cell_width, cell_height; //in pixels
  static cairo_matrix_t scale_matrix;

  //Make sure width and height are the same as before
  double temp_width = 1.0, temp_height = 1.0;
  cairo_user_to_device_distance(cr, &temp_width, &temp_height);
  if((int)temp_width != cell_width || (int)temp_height != cell_height) {
    cell_width = (int)temp_width;
    cell_height = (int)temp_height;
    memset(memoized_pattern, 0, sizeof(cairo_pattern_t*)*CELL_TYPE_MAX);
    cairo_matrix_init_scale(&scale_matrix, temp_width, temp_height);
  }

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	} else {
		cl->w = -1;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
  cell_type_t cell_type = ((set_cell_cb_t*)v)->cell_type;
  if(!memoized_pattern[cell_type]) {
    cairo_surface_t* memoizer_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, cell_width, cell_height);
    cairo_t* m_cr = cairo_create(memoizer_surface);
    cairo_scale(m_cr, cell_width, cell_height);
    switch(cell_type) {
      case CELL_TYPE_BLANK:
        break;
      case CELL_TYPE_STEM:
        stem_cell_glyph_day(m_cr);
        break;
      case CELL_TYPE_AND:
        and_gate_glyph_day(m_cr);
        break;
      case CELL_TYPE_OR:
        or_gate_glyph_day(m_cr);
        break;
      case CELL_TYPE_XOR:
        xor_gate_glyph_day(m_cr);
        break;
      case CELL_TYPE_NAND:
        nand_gate_glyph_day(m_cr);
        break;
      case CELL_TYPE_WIRE:
        wire_cell_glyph_day(m_cr);
        break;
      case CELL_TYPE_CROSSOVER:
        crossover_cell_glyph_day(m_cr);
        break;
      case CELL_TYPE_COPY_E:
        copy_cell_glyph_day(m_cr,2);
        break;
      case CELL_TYPE_COPY_N:
        copy_cell_glyph_day(m_cr,3);
        break;
      case CELL_TYPE_COPY_W:
        copy_cell_glyph_day(m_cr,0);
        break;
      case CELL_TYPE_COPY_S:
        copy_cell_glyph_day(m_cr,1);
        break;
      case CELL_TYPE_DELETE_E:
        delete_cell_glyph_day(m_cr,2);
        break;
      case CELL_TYPE_DELETE_N:
        delete_cell_glyph_day(m_cr,3);
        break;
      case CELL_TYPE_DELETE_W:
        delete_cell_glyph_day(m_cr,0);
        break;
      case CELL_TYPE_DELETE_S:
        delete_cell_glyph_day(m_cr,1);
        break;
    }
    memoized_pattern[cell_type] = cairo_pattern_create_for_surface(memoizer_surface);
    cairo_pattern_set_matrix(memoized_pattern[cell_type], &scale_matrix);
  }
  cairo_set_source(cr, memoized_pattern[cell_type]);
  cairo_paint(cr);
	cairo_restore(cr);
}

void rala_glyph_set_arrow_cb_day(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;
  static cairo_pattern_t* memoized_pattern[ARROW_TYPE_MAX];
  static int cell_width, cell_height; //in pixels
  static cairo_matrix_t scale_matrix;

  //Make sure width and height are the same as before
  double temp_width = 1.0, temp_height = 1.0;
  cairo_user_to_device_distance(cr, &temp_width, &temp_height);
  if((int)temp_width != cell_width || (int)temp_height != cell_height) {
    cell_width = (int)temp_width;
    cell_height = (int)temp_height;
    memset(memoized_pattern, 0, sizeof(cairo_pattern_t*)*ARROW_TYPE_MAX);
    cairo_matrix_init_scale(&scale_matrix, temp_width, temp_height);
  }

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	} else {
		cl->w = -1;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
	setup_arrow(cr,arrow_rotate(t, ((set_arrow_cb_t*)v)->arrow_dir));
  arrow_type_t arrow_type = ((set_arrow_cb_t*)v)->arrow_type;
  if(!memoized_pattern[arrow_type]) {
    cairo_surface_t* memoizer_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, cell_width, cell_height);
    cairo_t* m_cr = cairo_create(memoizer_surface);
    cairo_scale(m_cr, cell_width, cell_height);
    switch(arrow_type) {
      case ARROW_TYPE_NONE:
        arrow_none_glyph_day(m_cr);
        break;
      case ARROW_TYPE_X:
        arrow_x_glyph_day(m_cr);
        break;
      case ARROW_TYPE_0:
        arrow_0_glyph_day(m_cr);
        break;
      case ARROW_TYPE_1:
        arrow_1_glyph_day(m_cr);
        break;
    }
    memoized_pattern[arrow_type] = cairo_pattern_create_for_surface(memoizer_surface);
    cairo_pattern_set_matrix(memoized_pattern[arrow_type], &scale_matrix);
  }
  cairo_set_source(cr, memoized_pattern[arrow_type]);
  cairo_paint(cr);
	cairo_restore(cr);
}

void rala_glyph_set_cell_cb_night(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;
  static cairo_pattern_t* memoized_pattern[CELL_TYPE_MAX];
  static int cell_width, cell_height; //in pixels
  static cairo_matrix_t scale_matrix;

  //Make sure width and height are the same as before
  double temp_width = 1.0, temp_height = 1.0;
  cairo_user_to_device_distance(cr, &temp_width, &temp_height);
  if((int)temp_width != cell_width || (int)temp_height != cell_height) {
    cell_width = (int)temp_width;
    cell_height = (int)temp_height;
    memset(memoized_pattern, 0, sizeof(cairo_pattern_t*)*CELL_TYPE_MAX);
    cairo_matrix_init_scale(&scale_matrix, temp_width, temp_height);
  }

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	} else {
		cl->w = -1;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
  cell_type_t cell_type = ((set_cell_cb_t*)v)->cell_type;
  if(!memoized_pattern[cell_type]) {
    cairo_surface_t* memoizer_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, cell_width, cell_height);
    cairo_t* m_cr = cairo_create(memoizer_surface);
    cairo_scale(m_cr, cell_width, cell_height);
    switch(cell_type) {
      case CELL_TYPE_BLANK:
        break;
      case CELL_TYPE_STEM:
        stem_cell_glyph_night(m_cr);
        break;
      case CELL_TYPE_AND:
        and_gate_glyph_night(m_cr);
        break;
      case CELL_TYPE_OR:
        or_gate_glyph_night(m_cr);
        break;
      case CELL_TYPE_XOR:
        xor_gate_glyph_night(m_cr);
        break;
      case CELL_TYPE_NAND:
        nand_gate_glyph_night(m_cr);
        break;
      case CELL_TYPE_WIRE:
        wire_cell_glyph_night(m_cr);
        break;
      case CELL_TYPE_CROSSOVER:
        crossover_cell_glyph_night(m_cr);
        break;
      case CELL_TYPE_COPY_E:
        copy_cell_glyph_night(m_cr,2);
        break;
      case CELL_TYPE_COPY_N:
        copy_cell_glyph_night(m_cr,3);
        break;
      case CELL_TYPE_COPY_W:
        copy_cell_glyph_night(m_cr,0);
        break;
      case CELL_TYPE_COPY_S:
        copy_cell_glyph_night(m_cr,1);
        break;
      case CELL_TYPE_DELETE_E:
        delete_cell_glyph_night(m_cr,2);
        break;
      case CELL_TYPE_DELETE_N:
        delete_cell_glyph_night(m_cr,3);
        break;
      case CELL_TYPE_DELETE_W:
        delete_cell_glyph_night(m_cr,0);
        break;
      case CELL_TYPE_DELETE_S:
        delete_cell_glyph_night(m_cr,1);
        break;
    }
    memoized_pattern[cell_type] = cairo_pattern_create_for_surface(memoizer_surface);
    cairo_pattern_set_matrix(memoized_pattern[cell_type], &scale_matrix);
  }
  cairo_set_source(cr, memoized_pattern[cell_type]);
  cairo_paint(cr);
	cairo_restore(cr);
}

void rala_glyph_set_arrow_cb_night(void* v, affine_t t) {
	struct cl* cl = (struct cl*)(((set_cell_cb_t*)v)->cl);
	cairo_t *cr = cl->cr;
  static cairo_pattern_t* memoized_pattern[ARROW_TYPE_MAX];
  static int cell_width, cell_height; //in pixels
  static cairo_matrix_t scale_matrix;

  //Make sure width and height are the same as before
  double temp_width = 1.0, temp_height = 1.0;
  cairo_user_to_device_distance(cr, &temp_width, &temp_height);
  if((int)temp_width != cell_width || (int)temp_height != cell_height) {
    cell_width = (int)temp_width;
    cell_height = (int)temp_height;
    memset(memoized_pattern, 0, sizeof(cairo_pattern_t*)*ARROW_TYPE_MAX);
    cairo_matrix_init_scale(&scale_matrix, temp_width, temp_height);
  }

	//Mark dirty
	if(cl->w == 0) {
		cl->x = 2*t.wx-1;
		cl->y = -(2*t.wy)-1;
		cl->w = 3;
		cl->h = 3;
	} else {
		cl->w = -1;
	}

	cairo_save(cr);
	cairo_translate(cr,2*t.wx, -2*t.wy);
	setup_arrow(cr,arrow_rotate(t, ((set_arrow_cb_t*)v)->arrow_dir));
  arrow_type_t arrow_type = ((set_arrow_cb_t*)v)->arrow_type;
  if(!memoized_pattern[arrow_type]) {
    cairo_surface_t* memoizer_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, cell_width, cell_height);
    cairo_t* m_cr = cairo_create(memoizer_surface);
    cairo_scale(m_cr, cell_width, cell_height);
    switch(arrow_type) {
      case ARROW_TYPE_NONE:
        arrow_none_glyph_night(m_cr);
        break;
      case ARROW_TYPE_X:
        arrow_x_glyph_night(m_cr);
        break;
      case ARROW_TYPE_0:
        arrow_0_glyph_night(m_cr);
        break;
      case ARROW_TYPE_1:
        arrow_1_glyph_night(m_cr);
        break;
    }
    memoized_pattern[arrow_type] = cairo_pattern_create_for_surface(memoizer_surface);
    cairo_pattern_set_matrix(memoized_pattern[arrow_type], &scale_matrix);
  }
  cairo_set_source(cr, memoized_pattern[arrow_type]);
  cairo_paint(cr);
	cairo_restore(cr);
}
