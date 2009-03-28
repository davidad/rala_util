#include "ala_glyphs.h"

void xor_gate_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_translate(cr,0.04,0);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.8, 1.0);

	cairo_new_path(cr);
	cairo_move_to(cr,0,0);
	cairo_arc(cr,-0.5,0.5,sqrt(2.0)/2,-M_PI/4,M_PI/4);
	cairo_line_to(cr,0.5,1.0);
	cairo_save(cr);
	cairo_scale(cr,0.85,2.0);
	double angle = asin(3.0/4);
	cairo_arc_negative(cr,0.5,-0.5,1.0,M_PI/2,angle);
	cairo_arc_negative(cr,0.5,1.0,1.0,-angle,-M_PI/2);
	cairo_restore(cr);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,-0.12,0);
	cairo_arc(cr,-0.62,0.5,sqrt(2.0)/2,-M_PI/4,M_PI/4);
	cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void or_gate_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 1.0, 0.8);

	cairo_new_path(cr);
	cairo_move_to(cr,0,0);
	cairo_arc(cr,-0.5,0.5,sqrt(2.0)/2,-M_PI/4,M_PI/4);
	cairo_line_to(cr,0.5,1.0);
	cairo_save(cr);
	cairo_scale(cr,0.9,2.0);
	double angle = asin(3.0/4);
	cairo_arc_negative(cr,0.5,-0.5,1.0,M_PI/2,angle);
	cairo_arc_negative(cr,0.5,1.0,1.0,-angle,-M_PI/2);
	cairo_restore(cr);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void and_gate_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 1.0, 0.7, 0.7);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_arc_negative(cr,0.5,0.5,0.5,M_PI/2.0,-M_PI/2.0);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);

	cairo_restore(cr);
}

void nand_gate_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.75,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 1.0, 0.96, 0.7);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,0.5,1.0);
	cairo_arc_negative(cr,0.5,0.5,0.5,M_PI/2.0,0);
        cairo_arc(cr,1.08,0.5,0.08,M_PI,0);
        cairo_arc(cr,1.08,0.5,0.08,0,M_PI);
	cairo_arc_negative(cr,0.5,0.5,0.5,0,-M_PI/2.0);

	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void stem_cell_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.8, 0.8);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,1.0);
	cairo_line_to(cr,1.0,1.0);
	cairo_line_to(cr,1.0,0.0);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void crossover_cell_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.5, 0.8);

	cairo_new_path(cr);
	cairo_move_to(cr,0.5,0.0);
	cairo_line_to(cr,0.0,0.5);
	cairo_line_to(cr,0.5,1.0);
	cairo_line_to(cr,1.0,0.5);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void wire_cell_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.7, 0.4);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,0,M_PI*2);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void delete_cell_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.3, 0.2);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,-3*M_PI/4,3*M_PI/4);
	cairo_line_to(cr,0.5,0.5);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void copy_cell_glyph(cairo_t *cr) {
	cairo_pattern_t *pat;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.2, 0.9, 0.3);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,-5*M_PI/4,5*M_PI/4);
	cairo_line_to(cr,0.5,0.5);
	cairo_close_path(cr);

	cairo_set_source (cr, pat);
	cairo_pattern_destroy (pat);

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}
