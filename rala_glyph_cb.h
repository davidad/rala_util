#include "commands.h"
#include "cairo/cairo.h"

#ifndef RALA_GLYPH_CB_
#define RALA_GLYPH_CB_
typedef struct {
	cell_type_t cell_type;
	cairo_t* cr;
} set_cell_cb_t;

typedef struct {
	cairo_t* cr;
	arrow_type_t arrow_type;
	arrow_dir_t arrow_dir;
} set_arrow_cb_t;

void set_cell_cb (void* v, affine_t t);
void set_arrow_cb(void* v, affine_t t);
#endif
