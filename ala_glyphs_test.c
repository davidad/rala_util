#include "ala_glyphs.h"

int
main (int argc, char *argv[])
{
        cairo_surface_t *surface;
        cairo_t *cr;

        surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 750, 300);
        cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_translate(cr, 15,15);
	cairo_scale (cr, 150, 150);

	/* Drawing code goes here */
	and_gate_glyph(cr);
	cairo_translate(cr,1.0,0);
	nand_gate_glyph(cr);
	cairo_translate(cr,1.0,0);
	xor_gate_glyph(cr);
	cairo_translate(cr,1.0,0);
	or_gate_glyph(cr);
	cairo_translate(cr,1.0,0.5);
	stem_cell_glyph(cr);
	cairo_translate(cr,-1.0,0.5);
	crossover_cell_glyph(cr);
	cairo_translate(cr,-1.0,0);
	wire_cell_glyph(cr);
	cairo_translate(cr,-1.0,0);
	delete_cell_glyph(cr);
	cairo_translate(cr,-1.0,0);
	copy_cell_glyph(cr);

	/* Write output and clean up */
        cairo_surface_write_to_png (surface, "ala_glyphs.png");
        cairo_destroy (cr);
        cairo_surface_destroy (surface);

        return 0;
}
