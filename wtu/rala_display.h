#ifndef RALA_DISPLAY_H_
#define RALA_DISPLAY_H_

#include <cairo/cairo.h>

struct cl {
	cairo_t* cr;
	int x;
	int y;
	int w;
	int h;
};

#endif
