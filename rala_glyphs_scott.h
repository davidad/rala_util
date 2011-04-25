#include <cairo/cairo.h>
#include <math.h>
#include "rala_glyphs.h"

#ifndef RALA_GLYPHS_scott_H
#define RALA_GLYPHS_scott_H

#define ARROW_POS_X -0.1
#define ARROW_POS_Y -0.1

#define ARROW_BODY_RADIUS 0.095
#define ARROW_HEAD_RADIUS 0.2
#define ARROW_BODY_START 0.1
#define ARROW_HEAD_START 0.55
#define ARROW_HEAD_END 0.9

// New style constants

/*
#define ARROW_THICKNESS 0.16
#define ARROW_WIDTH     0.4
#define ARROW_SLOPE     1.0
#define ARROW_TAIL      0.3
*/

#define ARROW_THICKNESS 0.16*0.95
#define ARROW_WIDTH     0.4*0.95
#define ARROW_SLOPE     1.0
#define ARROW_TAIL      0.33 //0.35

#define ARROW_BULGE     (ARROW_WIDTH-ARROW_THICKNESS)
#define ARROW_FORWARD_EXTENT ((ARROW_THICKNESS+ARROW_BULGE)*ARROW_SLOPE)

#define ARROW_OUTLINES
#define ARROW_OUTLINE_THICKNESS 0.08
#define CLEAR_BEFORE_DRAW
#define CLEAR_BEHIND_TAIL

//#define CROSSOVER_DIAMOND

void and_gate_glyph_scott(cairo_t *cr);
void nand_gate_glyph_scott(cairo_t *cr);
void or_gate_glyph_scott(cairo_t *cr);
void xor_gate_glyph_scott(cairo_t *cr);

void copy_cell_glyph_scott(cairo_t *cr, int rotation);
void delete_cell_glyph_scott(cairo_t *cr, int rotation);
void stem_cell_glyph_scott(cairo_t *cr);
void wire_cell_glyph_scott(cairo_t *cr);
void crossover_cell_glyph_scott(cairo_t *cr);

void arrow_none_glyph_scott(cairo_t *cr);
void arrow_x_glyph_scott(cairo_t *cr);
void arrow_0_glyph_scott(cairo_t *cr);
void arrow_1_glyph_scott(cairo_t *cr);

void blank_cell_scott(cairo_t *cr);
void hollow_stem_cell_glyph_scott(cairo_t *cr);

void clear_scott(void *cl);
#endif
