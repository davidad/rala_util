#include "commands.h"
#include "cairo/cairo.h"

#ifndef RALA_GLYPH_CB_
#define RALA_GLYPH_CB_
void rala_glyph_set_cell_cb (void* v, affine_t t);
void rala_glyph_set_arrow_cb(void* v, affine_t t);
#endif
