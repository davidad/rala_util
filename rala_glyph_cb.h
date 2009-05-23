#include "commands.h"
#include "cairo/cairo.h"

#ifndef RALA_GLYPH_CB_
#define RALA_GLYPH_CB_
void rala_glyph_set_cell_cb_day (void* v, affine_t t);
void rala_glyph_set_arrow_cb_day(void* v, affine_t t);
void rala_glyph_set_cell_cb_night (void* v, affine_t t);
void rala_glyph_set_arrow_cb_night(void* v, affine_t t);
#endif
