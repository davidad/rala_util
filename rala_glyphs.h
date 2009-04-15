#include <cairo/cairo.h>
#include <math.h>

#ifndef RALA_GLYPHS_
#define RALA_GLYPHS_

#define ARROW_POS_X -0.1
#define ARROW_POS_Y -0.1

#define ARROW_BODY_RADIUS 0.095
#define ARROW_HEAD_RADIUS 0.2
#define ARROW_BODY_START 0.1
#define ARROW_HEAD_START 0.55
#define ARROW_HEAD_END 0.9

//#define CROSSOVER_DIAMOND

void and_gate_glyph(cairo_t *cr);
void nand_gate_glyph(cairo_t *cr);
void or_gate_glyph(cairo_t *cr);
void xor_gate_glyph(cairo_t *cr);

void copy_cell_glyph(cairo_t *cr, int rotation);
void delete_cell_glyph(cairo_t *cr, int rotation);
void stem_cell_glyph(cairo_t *cr);
void wire_cell_glyph(cairo_t *cr);
void crossover_cell_glyph(cairo_t *cr);

void arrow_none_glyph(cairo_t *cr);
void arrow_x_glyph(cairo_t *cr);
void arrow_0_glyph(cairo_t *cr);
void arrow_1_glyph(cairo_t *cr);

void blank_cell(cairo_t *cr);
void hollow_stem_cell_glyph(cairo_t *cr);

typedef enum {NONE, HOLLOW_STEM, STEM} blank_cell_style_t;
blank_cell_style_t blank_cell_style;
void clear(void *cl);
#endif
