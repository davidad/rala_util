#include <stdio.h>
#include <SDL/SDL.h>
#include "rala_glyphs.h"
#include "int_affine.h"

#ifndef COMMANDS_
#define COMMANDS_
typedef enum {
	ARROW_DIR_W,
	ARROW_DIR_N,
	ARROW_DIR_S,
	ARROW_DIR_E
} arrow_dir_t;

typedef enum {
	ARROW_TYPE_NONE,
	ARROW_TYPE_X,
	ARROW_TYPE_0,
	ARROW_TYPE_1
} arrow_type_t;

typedef enum {
	CELL_TYPE_BLANK,
	CELL_TYPE_STEM,
	CELL_TYPE_AND,
	CELL_TYPE_OR,
	CELL_TYPE_XOR,
	CELL_TYPE_NAND,
	CELL_TYPE_WIRE,
	CELL_TYPE_CROSSOVER,
	CELL_TYPE_COPY_E,
	CELL_TYPE_COPY_N,
	CELL_TYPE_COPY_W,
	CELL_TYPE_COPY_S,
	CELL_TYPE_DELETE_E,
	CELL_TYPE_DELETE_N,
	CELL_TYPE_DELETE_W,
	CELL_TYPE_DELETE_S
} cell_type_t;

int next_command_char(char c, cairo_t* cr);
#endif
