#include "rala_enums.h"

#ifndef _RALA_CELLS
#define _RALA_CELLS

#define RALA_IO_NORTH 0
#define RALA_IO_SOUTH 1
#define RALA_IO_WEST  2
#define RALA_IO_EAST  3

typedef struct rala_cell {
	cell_type_t state;

	int x;
	int y;

      // Extra information for whatever cells need it
      // File cells use extra_information to store the file descriptor they read from
    void *extra_information;

	struct rala_arrow* inputs[4];
	struct rala_arrow* outputs[4];
} rala_cell_t;

typedef struct rala_arrow {
	arrow_type_t state;

	int x;
	int y;
	arrow_dir_t arrow_dir;

	struct rala_cell *from;
	struct rala_cell *to;
} rala_arrow_t;

#endif
