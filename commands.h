#include <stdio.h>
#include <SDL/SDL.h>
#include "rala_glyphs.h"
#include "int_affine.h"

typedef enum {
	ARROW_DIR_W,
	ARROW_DIR_N,
	ARROW_DIR_S,
	ARROW_DIR_E
} arrow_dir_t;

int next_command_char(char c, cairo_t* cr);
