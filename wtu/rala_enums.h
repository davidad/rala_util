#ifndef _RALA_ENUMS
#define _RALA_ENUMS

typedef enum {
	ARROW_DIR_N,
	ARROW_DIR_S,
	ARROW_DIR_W,
	ARROW_DIR_E,
  ARROW_DIR_MAX
} arrow_dir_t;

typedef enum {
	ARROW_TYPE_NONE,
	ARROW_TYPE_X,
	ARROW_TYPE_0,
	ARROW_TYPE_1,
  ARROW_TYPE_MAX
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
	CELL_TYPE_COPY_N,
	CELL_TYPE_COPY_S,
	CELL_TYPE_COPY_W,
	CELL_TYPE_COPY_E,
	CELL_TYPE_DELETE_N,
	CELL_TYPE_DELETE_S,
	CELL_TYPE_DELETE_W,
	CELL_TYPE_DELETE_E,
	CELL_TYPE_SINK,
	CELL_TYPE_FILE,
  CELL_TYPE_MAX
} cell_type_t;

char* arrow_dir_to_str(arrow_dir_t);
char* arrow_type_to_str(arrow_type_t);
char* cell_type_to_str(cell_type_t);

#endif
