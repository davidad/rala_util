#include "rala_glyphs.h"

void xor_gate_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_translate(cr,0.04,0);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.68, 0.68, 1.0);
	}

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
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.35, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.68, 1.0, 0.68);
	}

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

	cairo_fill_preserve (cr);

	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_set_line_width(cr,0.07);
	cairo_set_line_join(cr,CAIRO_LINE_JOIN_ROUND);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void and_gate_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 1.0, 0.68, 0.68);
	}

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

void nand_gate_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.68,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
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

void stem_cell_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.8, 0.8);
	}

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

void crossover_cell_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.8, 0.5, 0.8);
	}

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

void wire_cell_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.7, 0.4);
	}

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

void delete_cell_glyph(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.9, 0.3, 0.2);
	}

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,(rotation*2-3)*M_PI/4,(rotation*2+3)*M_PI/4);
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

void copy_cell_glyph(cairo_t *cr, int rotation) {
	static cairo_pattern_t *pat = 0;

	if(pat == 0) {
		pat = cairo_pattern_create_radial (0.27, 0.3, 0.08, 0.2, 0.35, 0.6);
		cairo_pattern_add_color_stop_rgb (pat, 0, 0.9, 0.9, 0.9);
		cairo_pattern_add_color_stop_rgb (pat, 1, 0.2, 0.9, 0.3);
	}

	cairo_save(cr);
	cairo_scale(cr,0.8,0.8);

	cairo_new_path(cr);
	cairo_arc(cr,0.5,0.5,0.5,(rotation*2-5)*M_PI/4,(rotation*2+5)*M_PI/4);
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

void arrow_none_glyph(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

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

void arrow_x_glyph(cairo_t *cr) {
	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

	cairo_new_path(cr);
	cairo_move_to(cr,ARROW_BODY_START,0.25-ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_BODY_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_END,0.25);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_BODY_RADIUS);
	cairo_close_path(cr);
	
	cairo_set_source_rgb(cr,0.0,0.0,0.0);
	cairo_fill(cr);

	cairo_restore(cr);
}

void arrow_0_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat_in, *pat_out = 0;

	if(pat_in == 0) {
		pat_in = cairo_pattern_create_radial (ARROW_HEAD_START-0.2, 0.25, 0.01, ARROW_HEAD_START-0.2, 0.25, 1.0);
		cairo_pattern_add_color_stop_rgb (pat_in, 0, 0.88, 0.9, 1.0);
		cairo_pattern_add_color_stop_rgb (pat_in, ARROW_BODY_RADIUS*(0.7), 0.35, 0.35, 1.0);
		cairo_pattern_add_color_stop_rgb (pat_in, 1, 0.0, 0.0, 0.7);
	}

	if(pat_out == 0) {
		pat_out = cairo_pattern_create_radial (0, 0, ARROW_BODY_RADIUS*(0.7), 0.03, 0, 0.3);
		cairo_pattern_add_color_stop_rgba(pat_out, 0, 0.3, 0.3, 1.0, 0.6);
		cairo_pattern_add_color_stop_rgba(pat_out, 1, 0.1, 0.1, 1.0, 0.0);
	}

	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,0.5);
	cairo_line_to(cr,1.0,0.5);
	cairo_line_to(cr,1.0,0.0);
	cairo_close_path(cr);

	cairo_save(cr);
	cairo_translate(cr,ARROW_HEAD_START-0.2, 0.25);
	cairo_scale(cr,1.7,0.9);
	cairo_set_source(cr,pat_out);
	cairo_fill(cr);
	cairo_restore(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,ARROW_BODY_START,0.25-ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_BODY_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_END,0.25);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_BODY_RADIUS);
	cairo_close_path(cr);
	
	cairo_set_source(cr,pat_in);
	cairo_fill_preserve(cr);

	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,0.03);
	cairo_clip_preserve(cr);
	cairo_stroke(cr);

	cairo_restore(cr);
}

void arrow_1_glyph(cairo_t *cr) {
	static cairo_pattern_t *pat_in, *pat_out = 0;

	if(pat_in == 0) {
		pat_in = cairo_pattern_create_radial (ARROW_HEAD_START-0.1, 0.25, 0.01, ARROW_HEAD_START-0.1, 0.25, 1.0);
		cairo_pattern_add_color_stop_rgb (pat_in, 0, 1.0, 0.9, 0.88);
		cairo_pattern_add_color_stop_rgb (pat_in, ARROW_BODY_RADIUS*(0.7), 1.0, 0.35, 0.35);
		cairo_pattern_add_color_stop_rgb (pat_in, 1, 0.7, 0.0, 0.0);
	}

	if(pat_out == 0) {
		pat_out = cairo_pattern_create_radial (0, 0, ARROW_BODY_RADIUS, 0.03, 0, 0.3);
		cairo_pattern_add_color_stop_rgba(pat_out, 0, 1.0, 0.3, 0.3, 0.6);
		cairo_pattern_add_color_stop_rgba(pat_out, 1, 1.0, 0.1, 0.1, 0.0);
	}

	cairo_save(cr);
	cairo_translate(cr,ARROW_POS_X,ARROW_POS_Y);

	cairo_new_path(cr);
	cairo_move_to(cr,0.0,0.0);
	cairo_line_to(cr,0.0,0.5);
	cairo_line_to(cr,1.0,0.5);
	cairo_line_to(cr,1.0,0.0);
	cairo_close_path(cr);

	cairo_save(cr);
	cairo_translate(cr,ARROW_HEAD_START-0.1, 0.25);
	cairo_scale(cr,1.7,0.9);
	cairo_set_source(cr,pat_out);
	cairo_fill(cr);
	cairo_restore(cr);

	cairo_new_path(cr);
	cairo_move_to(cr,ARROW_BODY_START,0.25-ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_BODY_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_BODY_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25+ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_END,0.25);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_HEAD_RADIUS);
	cairo_line_to(cr,ARROW_HEAD_START,0.25-ARROW_BODY_RADIUS);
	cairo_close_path(cr);
	
	cairo_set_source(cr,pat_in);
	cairo_fill_preserve(cr);

	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,0.03);
	cairo_clip_preserve(cr);
	cairo_stroke(cr);

	cairo_restore(cr);
}
