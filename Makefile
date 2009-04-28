all: rala_step rala_display

rala_display: rala_display.c rala_glyph_cb.c rala_glyphs.c rala_enums.c commands.c cairosdl.c int_affine.c Makefile
	gcc -O3 -I/usr/include/cairo -lSDL -lSDL_net -lcairo rala_display.c rala_glyph_cb.c rala_glyphs.c rala_enums.c commands.c cairosdl.c int_affine.c -o rala_display

rala_step: rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c Makefile
	gcc -O3 rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c -o rala_step
