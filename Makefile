rala_util: rala_util.c rala_glyphs.c commands.c cairosdl.c
	gcc -lSDL -lSDL_net -lcairo rala_util.c rala_glyphs.c commands.c cairosdl.c -o rala_util
