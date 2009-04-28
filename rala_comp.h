#include "rala_enums.h"
#include <stdbool.h>

#ifndef _RALA_COMP
#define _RALA_COMP

#define RALA_IO_NORTH 0
#define RALA_IO_SOUTH 1
#define RALA_IO_WEST  2
#define RALA_IO_EAST  3

typedef struct rala_cell {
	cell_type_t state;

	int x;
	int y;

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

typedef struct rala_queue_node {
	rala_cell_t* cell;
	struct rala_queue_node* next;
} rala_queue_node_t;

typedef struct rala_queue {
	rala_queue_node_t* head;
	rala_queue_node_t* tail;
} rala_queue_t;

rala_queue_t* rala_queue_init(void);
void rala_enqueue(rala_queue_t* q, rala_cell_t* cell);
rala_cell_t* rala_dequeue(rala_queue_t* q);

//bool rala_cell_ready(rala_cell_t* cell);

typedef void (*arrow_notify_t)(rala_arrow_t*);
bool rala_cell_fire(rala_cell_t* cell, rala_queue_t* q, arrow_notify_t arrow_notify);

#endif
