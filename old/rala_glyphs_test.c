#include "rala_glyphs.h"

int
main (int argc, char *argv[])
{
        cairo_surface_t *surface;
        cairo_t *cr;

        surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 500, 250);
        cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_translate(cr, 10,10);
	cairo_scale (cr, 100, 100);

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
	delete_cell_glyph(cr,3);
	cairo_translate(cr,-1.0,0);
	copy_cell_glyph(cr,3);
	cairo_translate(cr,0,1.0);
	arrow_none_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_x_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_0_glyph(cr);
	cairo_translate(cr,1.0,0);
	arrow_1_glyph(cr);

	/* Write output and clean up */
        cairo_surface_write_to_png (surface, "ala_glyphs.png");
        cairo_destroy (cr);
        cairo_surface_destroy (surface);

        return 0;
}
