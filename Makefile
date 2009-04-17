rala_util: rala_util.c rala_glyph_cb.c rala_glyphs.c commands.c cairosdl.c int_affine.c Makefile
	gcc -O3 -I/usr/include/cairo -lSDL -lSDL_net -lcairo rala_util.c rala_glyph_cb.c rala_glyphs.c commands.c cairosdl.c int_affine.c -o rala_util

rala_step: rala_step.c commands.c int_affine.c Makefile
	gcc -O3 rala_step.c commands.c int_affine.c -o rala_step
