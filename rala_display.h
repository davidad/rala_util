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

// I wanted an underlay underneath the gates, so I created these two, to use as the source surface when clearing.
// TODO: Make these be part of the cl structure passed around to callbacks, and make rendering skins other than SCOTT use them.
extern cairo_surface_t* background_surface;
extern cairo_t* background_cr;

#endif

