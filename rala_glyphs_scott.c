#include "rala_glyphs_scott.h"
#include "rala_display.h"
#include <stdio.h>

// Call this before redrawing background, and the background underlay will work correctly.
// For example, if you want to clear an area to the background, simply call:
//
//     set_source_to_background(cr);
//     cairo_fill(cr);
//
// WARNING: Because of the cheesy way in which this function is implemented, rotations will mess up the source selection!
//          Please use only scaling and translation.

void set_source_to_background( cairo_t* cr ) {
	// An extremely cheap hack to make background rerendering work properly:
	// When redrawing backdrop, simply set the source surface with an offset equal to the current transform offset.
	cairo_matrix_t m;
	cairo_get_matrix( cr, &m );

	cairo_set_source_surface(cr, background_surface, m.x0, m.y0);
}

void rala_glyph_bg_clear_scott(cairo_t* cr) {
	cairo_new_path(cr);
	cairo_move_to(cr,-0.1,-0.1);
	cairo_line_to(cr,1.1,-0.1);
	cairo_line_to(cr,1.1,1.1);
	cairo_line_to(cr,-0.1,1.1);
	cairo_close_path(cr);

	set_source_to_background(cr);
	cairo_fill(cr);
}

void xor_gate_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.14,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.68, 0.68, 1.0);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_arc(cr,-0.47,0.5,sqrt(2.0)/2,-M_PI/4,M_PI/4);
	cairo_save(cr);
	cairo_scale(cr,0.85,2.0);
	double angle = asin(3.0/4);
	cairo_arc_negative(cr,0.5,-0.5,1.0,M_PI/2,angle);
	cairo_arc_negative(cr,0.5,1.0,1.0,-angle,-M_PI/2);
	cairo_restore(cr);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);

	cairo_new_path(cr);
	cairo_arc(cr,-0.62,0.5,sqrt(1.92)/2,-M_PI/4,M_PI/4);
	cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
	cairo_stroke(cr);

	cairo_restore(cr);
}

void or_gate_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.68, 1.0, 0.68);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,0,0);
	cairo_arc(cr,-0.5,0.5,sqrt(2.0)/2,-M_PI/4,M_PI/4);
	cairo_save(cr);
	cairo_scale(cr,0.9,2.0);
	double angle = asin(3.0/4);
	cairo_arc_negative(cr,0.5,-0.5,1.0,M_PI/2,angle);
	cairo_arc_negative(cr,0.5,1.0,1.0,-angle,-M_PI/2);
	cairo_restore(cr);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void and_gate_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 1.0, 0.68, 0.68);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_arc_negative(cr,0.5,0.5,0.5,M_PI/2.0,-M_PI/2.0);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);

	cairo_restore(cr);
}

void nand_gate_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);
	rala_glyph_bg_clear_scott(cr);
	cairo_restore(cr);

	cairo_scale(cr,0.68,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 1.0, 1.0, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 1.0, 1.0, 0.8);
	}


	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_arc_negative(cr,0.5,0.5,0.5,M_PI/2.0,0);
        cairo_arc(cr,1.1,0.5,0.1,M_PI,0);
        cairo_arc(cr,1.1,0.5,0.1,0,M_PI);
	cairo_arc_negative(cr,0.5,0.5,0.5,0,-M_PI/2.0);

	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void stem_cell_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.8, 0.8);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void hollow_stem_cell_glyph_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_close_path(cr);

	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	cairo_fill_preserve(cr);
	cairo_clip_preserve(cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void crossover_cell_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.5, 0.8);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
#ifdef CROSSOVER_DIAMOND
	cairo_move_to(cr,0.5,0.0);
	cairo_line_to(cr,0.0,0.5);
	cairo_line_to(cr,0.5,1.0);
	cairo_line_to(cr,1.0,0.5);
#else
	cairo_arc(cr,0.5,0.5,0.5,0,M_PI*2);
#endif
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);

#ifndef CROSSOVER_DIAMOND
	cairo_move_to(cr,0.5+sqrt(2.0)/4.0,0.5+sqrt(2.0)/4.0);
	cairo_line_to(cr,0.5-sqrt(2.0)/4.0,0.5-sqrt(2.0)/4.0);
	cairo_move_to(cr,0.5-sqrt(2.0)/4.0,0.5+sqrt(2.0)/4.0);
	cairo_line_to(cr,0.5+sqrt(2.0)/4.0,0.5-sqrt(2.0)/4.0);
	cairo_stroke(cr);
#endif
	
	cairo_restore(cr);
}

void wire_cell_glyph_scott(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.7, 0.4);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,0,M_PI*2);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void delete_cell_glyph_scott(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.3, 0.2);
	}

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,((-rotation)*2-3)*M_PI/4,((-rotation)*2+3)*M_PI/4);
	cairo_line_to(cr,0.5,0.5);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void copy_cell_glyph_scott(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.2, 0.9, 0.3);
	}

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	rala_glyph_bg_clear_scott(cr);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,((-rotation)*2-5)*M_PI/4,((-rotation)*2+5)*M_PI/4);
	cairo_line_to(cr,0.5,0.5);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void get_cell_glyph_scott(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);
  

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.98, 0.75, 0.02);
	}

	rala_glyph_bg_clear_scott(cr);

  double d = 1.0;
  double a = d/4;
  double b = sqrt(3) * a;

  cairo_save(cr);
  cairo_translate(cr,0.5,0.5);
  cairo_rotate(cr,rotation*(M_PI/2));
	cairo_new_path(cr);
  cairo_move_to(cr,0.1,2*a);
  cairo_line_to(cr,b,a);
  cairo_line_to(cr,b,-a);
  cairo_line_to(cr,0.1,-2*a);
  cairo_line_to(cr,-b,-a);
  cairo_line_to(cr,-b,a);
	cairo_close_path(cr);
  cairo_restore(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void put_cell_glyph_scott(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.1,0.1);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.0, 0.48, 0.57);
	}

	rala_glyph_bg_clear_scott(cr);

  double t = 1.0/3.0;
  double s = 1.0/6.0;
	cairo_new_path(cr);
  cairo_move_to(cr,t,1.0-s);
  cairo_line_to(cr,t+s,1.0);
  cairo_line_to(cr,t*2,1.0-s);
  cairo_line_to(cr,t*2,t*2);
  cairo_line_to(cr,t*2+s,t*2);
  cairo_line_to(cr,1.0,t+s);
  cairo_line_to(cr,t*2+s,t);
  cairo_line_to(cr,t*2,t);
  cairo_line_to(cr,t*2,0.0);
  cairo_line_to(cr,t+s,s);
  cairo_line_to(cr,t,0.0);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void arrow_none_glyph_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

	cairo_new_path(cr);
	cairo_move_to(cr,0.5,0.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_close_path(cr);

	//cairo_set_source_rgb(cr,1.0,1.0,1.0);
	set_source_to_background(cr);
	cairo_fill(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,ARROW_BODY_START,0.25-ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_BODY_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_END,0.25);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_BODY_RADIUS);
	cairo_close_path(cr);
	
	cairo_set_source_rgb(cr,0.8,0.8,0.8);
	cairo_fill(cr);

	cairo_restore(cr);
}

void arrow_x_glyph_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

#ifdef CLEAR_BEFORE_DRAW
	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_close_path(cr);
#endif

	//cairo_set_source_rgb(cr,1.0,1.0,1.0);
	set_source_to_background(cr);
	cairo_fill(cr);

	double s = 0.1;

	cairo_new_path(cr);
	cairo_move_to(cr, 0.5, 0.5-ARROW_THICKNESS);
	//cairo_rel_line_to(cr, 0.0, -ARROW_THICKNESS);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_TAIL, 0.0);
	cairo_rel_line_to(cr, 0.0, -2*ARROW_THICKNESS);
	// Final movement unnecessary
	//cairo_rel_line_to(cr, ARROW_TAIL, 0.0);
	cairo_close_path(cr);
	cairo_set_source_rgb(cr,0.5,0.5,0.5);
	cairo_fill(cr);

	cairo_restore(cr);
}

void arrow_0_glyph_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

#ifdef CLEAR_BEFORE_DRAW
	cairo_new_path(cr);
	cairo_move_to(cr,0.5,0.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_close_path(cr);
#endif

	//cairo_set_source_rgb(cr,1.0,1.0,1.0);
	set_source_to_background(cr);
	cairo_fill(cr);

#ifdef CLEAR_BEHIND_TAIL
	cairo_new_path(cr);
	cairo_move_to(cr,0.5-ARROW_TAIL,0.0);
	cairo_line_to(cr,0.4-ARROW_TAIL,0.0);
	cairo_line_to(cr,0.4-ARROW_TAIL,1.0);
	cairo_line_to(cr,0.5-ARROW_TAIL,1.0);
	cairo_close_path(cr);
	cairo_fill(cr);
#endif

	cairo_new_path(cr);
	cairo_move_to(cr, 0.5, 0.5-ARROW_THICKNESS);
	//cairo_rel_line_to(cr, 0.0, -ARROW_THICKNESS);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_TAIL, 0.0);
	cairo_rel_line_to(cr, 0.0, -2*ARROW_THICKNESS);
	// Final movement unnecessary
	//cairo_rel_line_to(cr, ARROW_TAIL, 0.0);
	cairo_close_path(cr);
	cairo_set_source_rgb(cr,0.0,0.0,1.0);
	cairo_clip_preserve(cr);

#ifdef ARROW_OUTLINES
	cairo_fill_preserve(cr);
	cairo_set_line_width(cr,ARROW_OUTLINE_THICKNESS);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_stroke(cr);
#else
	cairo_fill(cr);
#endif

	cairo_restore(cr);
}

void arrow_1_glyph_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

#ifdef CLEAR_BEFORE_DRAW
	cairo_new_path(cr);
	cairo_move_to(cr,0.5,0.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_close_path(cr);
#endif

	//cairo_set_source_rgb(cr,1.0,1.0,1.0);
	set_source_to_background(cr);
	cairo_fill(cr);

#ifdef CLEAR_BEHIND_TAIL
	cairo_new_path(cr);
	cairo_move_to(cr,0.5-ARROW_TAIL,0.0);
	cairo_line_to(cr,0.4-ARROW_TAIL,0.0);
	cairo_line_to(cr,0.4-ARROW_TAIL,1.0);
	cairo_line_to(cr,0.5-ARROW_TAIL,1.0);
	cairo_close_path(cr);
	cairo_fill(cr);
#endif

	cairo_new_path(cr);
	cairo_move_to(cr, 0.5, 0.5-ARROW_THICKNESS);
	//cairo_rel_line_to(cr, 0.0, -ARROW_THICKNESS);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_FORWARD_EXTENT, ARROW_THICKNESS+ARROW_BULGE);
	cairo_rel_line_to(cr, 0.0, -ARROW_BULGE);
	cairo_rel_line_to(cr, -ARROW_TAIL, 0.0);
	cairo_rel_line_to(cr, 0.0, -2*ARROW_THICKNESS);
	// Final movement unnecessary
	//cairo_rel_line_to(cr, ARROW_TAIL, 0.0);
	cairo_close_path(cr);
	cairo_set_source_rgb(cr,1.0,0.0,0.0);
	cairo_clip_preserve(cr);

#ifdef ARROW_OUTLINES
	cairo_fill_preserve(cr);
	cairo_set_line_width(cr,ARROW_OUTLINE_THICKNESS);
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_stroke(cr);
#else
	cairo_fill(cr);
#endif
	cairo_restore(cr);
}

void blank_cell_scott(cairo_t *cr) {
	cairo_save(cr);
	cairo_new_path(cr);
	cairo_move_to(cr,-1.0,0.0);
	cairo_line_to(cr,-1.0,0.5);
	cairo_line_to(cr,0.0,0.5);
	cairo_line_to(cr,0.0,2.0);
	cairo_line_to(cr,0.5,2.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_line_to(cr,2.0,1.0);
	cairo_line_to(cr,2.0,0.5);
	cairo_line_to(cr,1.0,0.5);
	cairo_line_to(cr,1.0,-1.0);
	cairo_line_to(cr,0.5,-1.0);
	cairo_line_to(cr,0.5,0.0);
	cairo_close_path(cr);

	cairo_set_source_rgb(cr,1,1,1);
	cairo_fill(cr);

	if(blank_cell_style > NONE) {
		cairo_translate(cr,0.5,0.5);
		cairo_save(cr);
		cairo_translate(cr,-1.4,-0.4);
		arrow_none_glyph_scott(cr);
		cairo_restore(cr);
		cairo_save(cr);
		cairo_rotate(cr,3*M_PI/2);
		cairo_translate(cr,-1.4,-0.4);
		arrow_none_glyph_scott(cr);
		cairo_restore(cr);
		cairo_save(cr);
		cairo_rotate(cr,M_PI);
		cairo_translate(cr,-1.4,-0.4);
		arrow_none_glyph_scott(cr);
		cairo_restore(cr);
		cairo_save(cr);
		cairo_rotate(cr,M_PI/2);
		cairo_translate(cr,-1.4,-0.4);
		arrow_none_glyph_scott(cr);
		cairo_restore(cr);
	}
	cairo_restore(cr);
}

void clear_scott(void *cl) {
	cairo_t* cr = (cairo_t*)cl;
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
		for(y=user_y_int; y<user_y_int+user_h_int; y+=2) {
			for(x=user_x_int; x<user_x_int+user_w_int; x+=2) {
				cairo_save(cr);
				cairo_translate(cr,x,y);
				blank_cell_scott(cr);
				if(blank_cell_style==HOLLOW_STEM) {
					hollow_stem_cell_glyph_scott(cr);
				} else if (blank_cell_style==STEM) {
					stem_cell_glyph_scott(cr);
				}
				cairo_restore(cr);
			}
		}
	}
}
