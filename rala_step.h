#include "rala_tree.h"
#include "rala_comp.h"

#ifndef _RALA_STEP
#define _RALA_STEP

typedef struct rala_step_state {
	cell_tree_t* cell_tree;
	arrow_tree_t* arrow_tree;
	rala_queue_t* queue;
} rala_step_state_t;

#endif
