#include "rala_enums.h"
#include "rala_cells.h"
#include <stdbool.h>

#ifndef _RALA_COMP
#define _RALA_COMP

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
bool rala_cell_fire(rala_cell_t* cell, rala_queue_t* q, arrow_notify_t arrow_notify, bool side_effects_on);

extern double total_energy;
extern double energy_table[CELL_TYPE_MAX][4];

#endif
