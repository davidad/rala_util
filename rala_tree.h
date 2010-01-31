#include "rala_enums.h"
#include "rala_cells.h"

#ifndef _RALA_TREE
#define _RALA_TREE

typedef long long int index_t;

typedef struct cell_tree_node {
	index_t index;

	rala_cell_t data;

	struct cell_tree_node* left_subtree;
	struct cell_tree_node* right_subtree;
} cell_tree_t;

typedef struct arrow_tree_node {
	index_t index;

	rala_arrow_t data;

	struct arrow_tree_node* left_subtree;
	struct arrow_tree_node* right_subtree;
} arrow_tree_t;

index_t compute_cell_index(int x, int y);
index_t compute_arrow_index(int x, int y, arrow_dir_t arrow_dir);

rala_cell_t* cell_insert(cell_tree_t** ct, arrow_tree_t* at, int x, int y, cell_type_t cell_type, void *extra_information);
int arrow_insert(arrow_tree_t** at, cell_tree_t* ct, int x, int y, arrow_dir_t arrow_dir, arrow_type_t arrow_type);
cell_type_t cell_get(cell_tree_t* t, int x, int y);
arrow_type_t arrow_get(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir);
rala_cell_t* cell_lookup(cell_tree_t* t, int x, int y);
rala_arrow_t* arrow_lookup(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir);

rala_cell_t* cell_insert_index(cell_tree_t** ct, arrow_tree_t* at, int x, int y, index_t index, cell_type_t cell_type, void *extra_information);
int arrow_insert_index(arrow_tree_t** at, cell_tree_t* ct, int x, int y, arrow_dir_t arrow_dir, index_t index, arrow_type_t arrow_type);
rala_cell_t* cell_lookup_index(cell_tree_t* t, index_t index);
rala_arrow_t* arrow_lookup_index(arrow_tree_t* t, index_t index);

#endif
