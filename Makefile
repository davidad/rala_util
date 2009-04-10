rala_util: rala_util.c rala_glyphs.c commands.c cairosdl.c int_affine.c
	gcc -O3 -lSDL -lSDL_net -lcairo rala_util.c rala_glyphs.c commands.c cairosdl.c int_affine.c -o rala_util
