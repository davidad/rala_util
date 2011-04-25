INCLUDEDIR=/opt/local/include/
LINKDIR=/opt/local/lib/

all: rala_step rala_display rala_avr_load

rala_display: rala_display.c rala_glyph_cb.c rala_glyphs_day.c rala_glyphs_night.c rala_enums.c commands.c cairosdl.c int_affine.c Makefile
	gcc -O3 -I/usr/include/cairo -lm `sdl-config --libs --cflags` -lSDL_net -lcairo rala_display.c rala_glyph_cb.c rala_glyphs_day.c rala_glyphs_night.c rala_enums.c commands.c cairosdl.c int_affine.c -o rala_display

rala_step: rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c Makefile
	gcc -O3 rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c -o rala_step

rala_avr_load: rala_avr_load.c rala_enums.c rala_tree.c commands.c int_affine.c Makefile
	gcc -O3 rala_avr_load.c rala_enums.c rala_tree.c commands.c int_affine.c -o rala_avr_load
