#include "rala_tree.h"
#include <stdlib.h>

index_t compute_cell_index(int x, int y) {
	if(y >= 0) {
		if(x >= 0) {
			int n = x+y;
			return (n*(n+1)/2+y)*4;
		} else {
			int n = -x+y-1;
			return (n*(n+1)/2+y)*4+1;
		}
	} else {
		if (x >= 0) {
			int n = x-y-1;
			return (n*(n+1)/2-y-1)*4+2;
		} else {
			int n = -x-y-2;
			return (n*(n+1)/2-y-1)*4+3;
		}
	}
}

index_t compute_arrow_index(int x, int y, arrow_dir_t arrow_dir) {
	index_t cell_index = compute_cell_index(x,y);
	return cell_index*4+arrow_dir;
}

rala_cell_t* cell_insert(cell_tree_t** ct, arrow_tree_t* at, int x, int y, cell_type_t cell_type, void *extra_information) {
	return cell_insert_index(ct, at, x, y, compute_cell_index(x,y), cell_type, extra_information);
}

int arrow_insert(arrow_tree_t** at, cell_tree_t* ct, int x, int y, arrow_dir_t arrow_dir, arrow_type_t arrow_type) {
	return arrow_insert_index(at, ct, x, y, arrow_dir, compute_arrow_index(x,y,arrow_dir), arrow_type);
}

cell_type_t cell_get(cell_tree_t* t, int x, int y) {
	return (cell_lookup(t,x,y))->state;
}

arrow_type_t arrow_get(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir) {
	return (arrow_lookup(t,x,y,arrow_dir))->state;
}

rala_cell_t* cell_lookup(cell_tree_t* t, int x, int y) {
	return cell_lookup_index(t, compute_cell_index(x,y));
}

rala_arrow_t* arrow_lookup(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir) {
	return arrow_lookup_index(t, compute_arrow_index(x, y, arrow_dir));
}

void cell_update_height(cell_tree_t* root) {
        int left_height = 0;
        int right_height = 0;
        if(root->left_subtree) {left_height = root->left_subtree->height;}
        if(root->right_subtree) {right_height = root->right_subtree->height;}
        if(right_height > left_height) {
                root->height = right_height + 1;
        } else {
                root->height = left_height + 1;
        }
        root->balance_factor = right_height - left_height;
}

void cell_left_rotation (cell_tree_t** rootp) {
	cell_tree_t* old_root = *rootp;
	cell_tree_t* pivot = old_root->right_subtree;
	*rootp = pivot;
	old_root->right_subtree = pivot->left_subtree;
	pivot->left_subtree = old_root;
	cell_update_height(old_root);
	cell_update_height(pivot);
}

void cell_right_rotation (cell_tree_t** rootp) {
	cell_tree_t* old_root = *rootp;
	cell_tree_t* pivot = old_root->left_subtree;
	*rootp = pivot;
	old_root->left_subtree = pivot->right_subtree;
	pivot->right_subtree = old_root;
	cell_update_height(old_root);
	cell_update_height(pivot);
}

void cell_balance(cell_tree_t** root) {
	if ((*root)->balance_factor == 2 && (*root)->right_subtree->balance_factor == -1) {
		cell_right_rotation(&(*root)->right_subtree);
		cell_left_rotation(root);
	}
	else if ((*root)->balance_factor == 2 && (*root)->right_subtree->balance_factor == 1) {
		cell_left_rotation(root);
	}
	else if ((*root)->balance_factor == -2 && (*root)->left_subtree->balance_factor == -1) {
		cell_right_rotation(root);
	}
	else if ((*root)->balance_factor == -2 && (*root)->left_subtree->balance_factor == 1) {
		cell_left_rotation(&(*root)->left_subtree);
		cell_right_rotation(root);
	}		
}

void arrow_update_height(arrow_tree_t* root) {
        int left_height = 0;
        int right_height = 0;
        if(root->left_subtree) {left_height = root->left_subtree->height;}
        if(root->right_subtree) {right_height = root->right_subtree->height;}
        if(right_height > left_height) {
                root->height = right_height + 1;
        } else {
                root->height = left_height + 1;
        }
        root->balance_factor = right_height - left_height;
}

void arrow_left_rotation (arrow_tree_t** rootp) {
	arrow_tree_t* old_root = *rootp;
	arrow_tree_t* pivot = old_root->right_subtree;
	*rootp = pivot;
	old_root->right_subtree = pivot->left_subtree;
	pivot->left_subtree = old_root;
	arrow_update_height(old_root);
	arrow_update_height(pivot);
}

void arrow_right_rotation (arrow_tree_t** rootp) {
	arrow_tree_t* old_root = *rootp;
	arrow_tree_t* pivot = old_root->left_subtree;
	*rootp = pivot;
	old_root->left_subtree = pivot->right_subtree;
	pivot->right_subtree = old_root;
	arrow_update_height(old_root);
	arrow_update_height(pivot);
}

void arrow_balance(arrow_tree_t** root) {
	if ((*root)->balance_factor == 2 && (*root)->right_subtree->balance_factor == -1) {
		arrow_right_rotation(&(*root)->right_subtree);
		arrow_left_rotation(root);
	}
	else if ((*root)->balance_factor == 2 && (*root)->right_subtree->balance_factor == 1) {
		arrow_left_rotation(root);
	}
	else if ((*root)->balance_factor == -2 && (*root)->left_subtree->balance_factor == -1) {
		arrow_right_rotation(root);
	}
	else if ((*root)->balance_factor == -2 && (*root)->left_subtree->balance_factor == 1) {
		arrow_left_rotation(&(*root)->left_subtree);
		arrow_right_rotation(root);
	}		
}

rala_cell_t* cell_insert_index(cell_tree_t** ct, arrow_tree_t* at, int x, int y, index_t index, cell_type_t cell_type, void *extra_information) {
	rala_cell_t* result = NULL;
	if(*ct == NULL) {
		*ct = malloc(sizeof(cell_tree_t));
		(*ct)->index = index;
		(*ct)->data.x = x;
		(*ct)->data.y = y;
		(*ct)->data.state = cell_type;
		(*ct)->data.extra_information = extra_information;
		(*ct)->left_subtree = NULL;
		(*ct)->right_subtree = NULL;
    (*ct)->height=1;
    (*ct)->balance_factor=0;
		(*ct)->data.inputs[RALA_IO_NORTH] = arrow_lookup(at, x, y, ARROW_DIR_N);
		if((*ct)->data.inputs[RALA_IO_NORTH]) {
			(*ct)->data.inputs[RALA_IO_NORTH]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_SOUTH] = arrow_lookup(at, x, y, ARROW_DIR_S);
		if((*ct)->data.inputs[RALA_IO_SOUTH]) {
			(*ct)->data.inputs[RALA_IO_SOUTH]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_WEST] = arrow_lookup(at, x, y, ARROW_DIR_W);
		if((*ct)->data.inputs[RALA_IO_WEST]) {
			(*ct)->data.inputs[RALA_IO_WEST]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_EAST] = arrow_lookup(at, x, y, ARROW_DIR_E);
		if((*ct)->data.inputs[RALA_IO_EAST]) {
			(*ct)->data.inputs[RALA_IO_EAST]->to = &((*ct)->data);
		}

		(*ct)->data.outputs[RALA_IO_NORTH] = arrow_lookup(at, x, y+1, ARROW_DIR_S);
		if((*ct)->data.outputs[RALA_IO_NORTH]) {
			(*ct)->data.outputs[RALA_IO_NORTH]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_SOUTH] = arrow_lookup(at, x, y-1, ARROW_DIR_N);
		if((*ct)->data.outputs[RALA_IO_SOUTH]) {
			(*ct)->data.outputs[RALA_IO_SOUTH]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_WEST] = arrow_lookup(at, x-1, y, ARROW_DIR_E);
		if((*ct)->data.outputs[RALA_IO_WEST]) {
			(*ct)->data.outputs[RALA_IO_WEST]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_EAST] = arrow_lookup(at, x+1, y, ARROW_DIR_W);
		if((*ct)->data.outputs[RALA_IO_EAST]) {
			(*ct)->data.outputs[RALA_IO_EAST]->from = &((*ct)->data);
		}
		result = &((*ct)->data);
	} else if ((*ct)->index == index) {
		(*ct)->data.state = cell_type;
		(*ct)->data.extra_information = extra_information;
		return &((*ct)->data);
	} else if ((*ct)->index < index) {
		result = cell_insert_index(&((*ct)->right_subtree), at, x, y, index, cell_type, extra_information);
    cell_update_height(*ct);
    cell_balance(ct);
	} else if ((*ct)->index > index) {
		result = cell_insert_index(&((*ct)->left_subtree), at, x, y, index, cell_type, extra_information);
    cell_update_height(*ct);
    cell_balance(ct);
	}
	return result;
}

int arrow_insert_index(arrow_tree_t** at, cell_tree_t* ct, int x, int y, arrow_dir_t arrow_dir, index_t index, arrow_type_t arrow_type) {
	int result = -1;
	if(*at == NULL) {
		*at = malloc(sizeof(arrow_tree_t));
		(*at)->index = index;
		(*at)->data.x = x;
		(*at)->data.y = y;
		(*at)->data.arrow_dir = arrow_dir;
		(*at)->data.state = arrow_type;
		(*at)->left_subtree = NULL;
		(*at)->right_subtree = NULL;
    (*at)->height=1;
    (*at)->balance_factor=0;

		(*at)->data.to = cell_lookup(ct, x, y);
		switch(arrow_dir) {
			case ARROW_DIR_N:
				(*at)->data.from = cell_lookup(ct, x, y+1);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_SOUTH] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_NORTH] = &((*at)->data);
				}
				break;
			case ARROW_DIR_S:
				(*at)->data.from = cell_lookup(ct, x, y-1);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_NORTH] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_SOUTH] = &((*at)->data);
				}
				break;
			case ARROW_DIR_E:
				(*at)->data.from = cell_lookup(ct, x+1, y);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_WEST] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_EAST] = &((*at)->data);
				}
				break;
			case ARROW_DIR_W:
				(*at)->data.from = cell_lookup(ct, x-1, y);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_EAST] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_WEST] = &((*at)->data);
				}
				break;
		}
		return 0;
	} else if ((*at)->index == index) {
		(*at)->data.state=arrow_type;
		return 1;
	} else if ((*at)->index < index) {
		result = arrow_insert_index(&((*at)->right_subtree), ct, x, y, arrow_dir, index, arrow_type);
    arrow_update_height(*at);
    arrow_balance(at);
	} else if ((*at)->index > index) {
		result = arrow_insert_index(&((*at)->left_subtree), ct, x, y, arrow_dir, index, arrow_type);
    arrow_update_height(*at);
    arrow_balance(at);
	}
	return result;
}

rala_cell_t* cell_lookup_index(cell_tree_t* t, index_t index) {
	if(t == NULL) {return NULL;}
	if(t->index == index) {return &(t->data);}
	if(t->index < index) {return cell_lookup_index(t->right_subtree, index);}
	return cell_lookup_index(t->left_subtree,  index);
}

rala_arrow_t* arrow_lookup_index(arrow_tree_t* t, index_t index) {
	if(t == NULL) {return NULL;}
	if(t->index == index) {return &(t->data);}
	if(t->index < index) {return arrow_lookup_index(t->right_subtree, index);}
	return arrow_lookup_index(t->left_subtree,  index);
}

