INCLUDEDIR=/opt/local/include/
LINKDIR=/opt/local/lib/

all: rala_step rala_display rala_avr_load


rala_display: rala_display.c rala_glyph_cb.c rala_glyphs_day.c rala_glyphs_night.c rala_glyphs_scott.c rala_enums.c commands.c cairosdl.c int_affine.c hierarchy.o Makefile
	gcc -O3 -I$(INCLUDEDIR) -I/opt/local/include/SDL -D_THREAD_SAFE -L$(LINKDIR) -I/usr/include/cairo -lm -lSDL -lSDL_net -lcairo -lconfig++ -lstdc++ rala_display.c rala_glyph_cb.c rala_glyphs_day.c rala_glyphs_night.c rala_glyphs_scott.c rala_enums.c commands.c cairosdl.c int_affine.c hierarchy.o -o rala_display -L/opt/local/lib -lSDL

rala_step: rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c Makefile
	gcc -O3 rala_step.c rala_enums.c rala_comp.c rala_tree.c commands.c int_affine.c -o rala_step

rala_avr_load: rala_avr_load.c rala_enums.c rala_tree.c commands.c int_affine.c Makefile
	gcc -O3 rala_avr_load.c rala_enums.c rala_tree.c commands.c int_affine.c -o rala_avr_load

hierarchy.o: hierarchy.cpp hierarchy.h Makefile
	g++ -O3 -I$(INCLUDEDIR) -L$(LINKDIR) -lconfig++ -c hierarchy.cpp -o hierarchy.o

clean:
	rm rala_display rala_step rala_avr_load hierarchy.o

