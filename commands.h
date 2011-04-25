#include <stdio.h>
#include "rala_enums.h"
#include "int_affine.h"

#ifndef COMMANDS_
#define COMMANDS_

typedef struct {
	void* cl;
	cell_type_t cell_type;
	  // Extra information to be passed through
	void *extra_information;
} set_cell_cb_t;

typedef struct {
	void* cl;
	arrow_type_t arrow_type;
	arrow_dir_t arrow_dir;
} set_arrow_cb_t;

typedef void (*updater_t)(void*);
typedef void (*clear_t)(void*);

int next_command_char(char c, void* cl, affine_operator_t set_cell_cb, affine_operator_t set_arrow_cb, clear_t clear, updater_t update_screen, updater_t update_frame);

arrow_dir_t arrow_rotate(affine_t t, arrow_dir_t arrow_dir);

#endif

