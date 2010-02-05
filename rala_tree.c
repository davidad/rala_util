#include "rala_tree.h"
#include <stdlib.h>
#include <stdio.h>

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

rala_cell_t* cell_insert(cell_tree_t** ct, arrow_tree_t** at, int x, int y, cell_type_t cell_type, void *extra_information) {
	return cell_insert_index(NULL, ct, at, x, y, compute_cell_index(x,y), cell_type, extra_information);
}

int arrow_insert(arrow_tree_t** at, cell_tree_t** ct, int x, int y, arrow_dir_t arrow_dir, arrow_type_t arrow_type) {
	return arrow_insert_index(NULL, at, ct, x, y, arrow_dir, compute_arrow_index(x,y,arrow_dir), arrow_type);
}

rala_cell_t* cell_lookup(cell_tree_t* t, int x, int y) {
	rala_cell_t* result = cell_lookup_index(t, compute_cell_index(x,y));
  return result;
}

rala_arrow_t* arrow_lookup(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir) {
	rala_arrow_t* result = arrow_lookup_index(t, compute_arrow_index(x, y, arrow_dir));
  return result;
}

cell_type_t cell_get(cell_tree_t* t, int x, int y) {
	return (cell_lookup(t,x,y))->state;
}

arrow_type_t arrow_get(arrow_tree_t* t, int x, int y, arrow_dir_t arrow_dir) {
	return (arrow_lookup(t,x,y,arrow_dir))->state;
}

void cell_left_rotation (cell_tree_t* old_root) {
	cell_tree_t** rootp = old_root->ptr_to_here;
	cell_tree_t* pivot = old_root->right_subtree;
	*rootp = pivot;
	old_root->right_subtree = pivot->left_subtree;
	pivot->left_subtree = old_root;
  if(old_root->right_subtree) {
    old_root->right_subtree->parent = old_root;
    old_root->right_subtree->ptr_to_here = &(old_root->right_subtree);
  }
  pivot->parent = old_root->parent;
  pivot->ptr_to_here = old_root->ptr_to_here;
  old_root->parent = pivot;
  old_root->ptr_to_here = &(pivot->left_subtree);
}

void cell_right_rotation (cell_tree_t* old_root) {
	cell_tree_t** rootp = old_root->ptr_to_here;
	cell_tree_t* pivot = old_root->left_subtree;
	*rootp = pivot;
	old_root->left_subtree = pivot->right_subtree;
	pivot->right_subtree = old_root;
  if(old_root->left_subtree) {
    old_root->left_subtree->parent = old_root;
    old_root->left_subtree->ptr_to_here = &(old_root->left_subtree);
  }
  pivot->parent = old_root->parent;
  pivot->ptr_to_here = old_root->ptr_to_here;
  old_root->parent = pivot;
  old_root->ptr_to_here = &(pivot->right_subtree);
}

void cell_print(int k, cell_tree_t* x) {
  int i;
  for(i=0; i<k; i++) {printf("  ");}
  printf("%d",x);
  if(x) {
    printf(" [%d] (%d,%d,%d)\n", x->index, x->left_subtree, x->right_subtree, x->parent);
    cell_print(k+1, x->left_subtree);
    cell_print(k+1, x->right_subtree);
  } else {
    printf("\n");
  }
}

void arrow_print(int k, arrow_tree_t* x) {
  int i;
  for(i=0; i<k; i++) {printf("  ");}
  printf("%d",x);
  if(x) {
    printf(" [%d] (%d,%d,%d)\n", x->index, x->left_subtree, x->right_subtree, x->parent);
    arrow_print(k+1, x->left_subtree);
    arrow_print(k+1, x->right_subtree);
  } else {
    printf("\n");
  }
}

void cell_splay(cell_tree_t* x) {
  //cell_print(0,x);
  if(x->parent == NULL) { //Done
    return;
  } else if(x->parent->parent == NULL) {
    if(x->parent->left_subtree == x) { //Zig
      //printf("Zig\n");
      cell_right_rotation(x->parent);
    } else {                           //Zag
      //printf("Zag\n");
      cell_left_rotation(x->parent);
    }
  } else {
    cell_tree_t *b,*c,*p,*g; //see en.wikipedia.org/wiki/Splay_tree
    p = x->parent; g = p->parent;
    //printf("x: %d;, p: %d g: %d, gg: %d\n", x, p, g, g->parent);
    x->parent = g->parent;
    *(g->ptr_to_here) = x;
    x->ptr_to_here = g->ptr_to_here;
    if(g->left_subtree == p) {
      if(p->left_subtree == x) { //Zig-zig
        //printf("Zig-zig\n");
        b = x->right_subtree;
        c = p->right_subtree;
        x->right_subtree = p; if(p) { p->parent = x; p->ptr_to_here = &(x->right_subtree); }
        p->left_subtree = b;  if(b) { b->parent = p; b->ptr_to_here = &(p->left_subtree); }
        p->right_subtree = g; if(g) { g->parent = p; g->ptr_to_here = &(p->right_subtree); }
        g->left_subtree = c;  if(c) { c->parent = g; c->ptr_to_here = &(g->left_subtree); }
      } else {                   //Zig-zag
        //printf("Zig-zag\n");
        b = x->left_subtree;
        c = x->right_subtree;
        x->right_subtree = g; if(g) { g->parent = x; g->ptr_to_here = &(x->right_subtree); }
        x->left_subtree = p;  if(p) { p->parent = x; p->ptr_to_here = &(x->left_subtree); }
        p->right_subtree = b; if(b) { b->parent = p; b->ptr_to_here = &(p->right_subtree); }
        g->left_subtree = c;  if(c) { c->parent = g; c->ptr_to_here = &(g->left_subtree); }
      }
    } else {
      if(p->right_subtree == x) { //Zig-zig
        //printf("Zig-zig 2\n");
        b = p->left_subtree;
        c = x->left_subtree;
        x->left_subtree = p;  if(p) { p->parent = x; p->ptr_to_here = &(x->left_subtree); }
        p->right_subtree = c; if(c) { c->parent = p; c->ptr_to_here = &(p->right_subtree); }
        p->left_subtree = g;  if(g) { g->parent = p; g->ptr_to_here = &(p->left_subtree); }
        g->right_subtree = b; if(b) { b->parent = g; b->ptr_to_here = &(g->right_subtree); }
      } else {                    //Zig-zag
        //printf("Zig-zag\n");
        b = x->left_subtree;
        c = x->right_subtree;
        x->left_subtree = g;  if(g) { g->parent = x; g->ptr_to_here = &(x->left_subtree); }
        x->right_subtree = p; if(p) { p->parent = x; p->ptr_to_here = &(x->right_subtree); }
        p->left_subtree = c;  if(c) { c->parent = p; c->ptr_to_here = &(p->left_subtree); }
        g->right_subtree = b; if(b) { b->parent = g; b->ptr_to_here = &(g->right_subtree); }
      }
    }
  }
  //printf("cell_splay tail call\n");
  cell_splay(x);
  //printf("cell_splay tail call returned\n");
}

void arrow_left_rotation (arrow_tree_t* old_root) {
	arrow_tree_t** rootp = old_root->ptr_to_here;
	arrow_tree_t* pivot = old_root->right_subtree;
	*rootp = pivot;
	old_root->right_subtree = pivot->left_subtree;
  if(old_root->right_subtree) {
    old_root->right_subtree->parent = old_root;
    old_root->right_subtree->ptr_to_here = &(old_root->right_subtree);
  }
	pivot->left_subtree = old_root;
  pivot->parent = old_root->parent;
  pivot->ptr_to_here = old_root->ptr_to_here;
  old_root->parent = pivot;
  old_root->ptr_to_here = &(pivot->left_subtree);
}

void arrow_right_rotation (arrow_tree_t* old_root) {
	arrow_tree_t** rootp = old_root->ptr_to_here;
	arrow_tree_t* pivot = old_root->left_subtree;
	*rootp = pivot;
	old_root->left_subtree = pivot->right_subtree;
  if(old_root->left_subtree) {
    old_root->left_subtree->parent = old_root;
    old_root->left_subtree->ptr_to_here = &(old_root->left_subtree);
  }
	pivot->right_subtree = old_root;
  pivot->parent = old_root->parent;
  pivot->ptr_to_here = old_root->ptr_to_here;
  old_root->parent = pivot;
  old_root->ptr_to_here = &(pivot->right_subtree);
}

void arrow_splay(arrow_tree_t* x) {
  //arrow_print(0,x);
  if(x->parent == NULL) { //Done
    return;
  } else if(x->parent->parent == NULL) {
    if(x->parent->left_subtree == x) { //Zig
      //printf("Zig\n");
      arrow_right_rotation(x->parent);
    } else {                           //Zag
      //printf("Zag\n");
      arrow_left_rotation(x->parent);
    }
  } else {
    arrow_tree_t *b,*c,*p,*g; //see en.wikipedia.org/wiki/Splay_tree
    p = x->parent; g = p->parent;
    //printf("x: %d;, p: %d g: %d, gg: %d\n", x, p, g, g->parent);
    x->parent = g->parent;
    *(g->ptr_to_here) = x;
    x->ptr_to_here = g->ptr_to_here;
    if(g->left_subtree == p) {
      if(p->left_subtree == x) { //Zig-zig
        //printf("Zig-zig\n");
        b = x->right_subtree;
        c = p->right_subtree;
        x->right_subtree = p; if(p) { p->parent = x; p->ptr_to_here = &(x->right_subtree); }
        p->left_subtree = b;  if(b) { b->parent = p; b->ptr_to_here = &(p->left_subtree); }
        p->right_subtree = g; if(g) { g->parent = p; g->ptr_to_here = &(p->right_subtree); }
        g->left_subtree = c;  if(c) { c->parent = g; c->ptr_to_here = &(g->left_subtree); }
      } else {                   //Zig-zag
        //printf("Zig-zag\n");
        b = x->left_subtree;
        c = x->right_subtree;
        x->right_subtree = g; if(g) { g->parent = x; g->ptr_to_here = &(x->right_subtree); }
        x->left_subtree = p;  if(p) { p->parent = x; p->ptr_to_here = &(x->left_subtree); }
        p->right_subtree = b; if(b) { b->parent = p; b->ptr_to_here = &(p->right_subtree); }
        g->left_subtree = c;  if(c) { c->parent = g; c->ptr_to_here = &(g->left_subtree); }
      }
    } else {
      if(p->right_subtree == x) { //Zig-zig
        //printf("Zig-zig 2\n");
        b = p->left_subtree;
        c = x->left_subtree;
        x->left_subtree = p;  if(p) { p->parent = x; p->ptr_to_here = &(x->left_subtree); }
        p->right_subtree = c; if(c) { c->parent = p; c->ptr_to_here = &(p->right_subtree); }
        p->left_subtree = g;  if(g) { g->parent = p; g->ptr_to_here = &(p->left_subtree); }
        g->right_subtree = b; if(b) { b->parent = g; b->ptr_to_here = &(g->right_subtree); }
      } else {                    //Zig-zag
        //printf("Zig-zag\n");
        b = x->left_subtree;
        c = x->right_subtree;
        x->left_subtree = g;  if(g) { g->parent = x; g->ptr_to_here = &(x->left_subtree); }
        x->right_subtree = p; if(p) { p->parent = x; p->ptr_to_here = &(x->right_subtree); }
        p->left_subtree = c;  if(c) { c->parent = p; c->ptr_to_here = &(p->left_subtree); }
        g->right_subtree = b; if(b) { b->parent = g; b->ptr_to_here = &(g->right_subtree); }
      }
    }
  }
  //printf("arrow_splay tail call\n");
  arrow_splay(x);
  //printf("arrow_splay tail call returned\n");
}

rala_cell_t* cell_insert_index(cell_tree_t* parent, cell_tree_t** ct, arrow_tree_t** at, int x, int y, index_t index, cell_type_t cell_type, void *extra_information) {
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
    (*ct)->parent = parent;
    (*ct)->ptr_to_here = ct;

		(*ct)->data.inputs[RALA_IO_NORTH] = arrow_lookup(*at, x, y, ARROW_DIR_N);
		if((*ct)->data.inputs[RALA_IO_NORTH]) {
			(*ct)->data.inputs[RALA_IO_NORTH]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_SOUTH] = arrow_lookup(*at, x, y, ARROW_DIR_S);
		if((*ct)->data.inputs[RALA_IO_SOUTH]) {
			(*ct)->data.inputs[RALA_IO_SOUTH]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_WEST] = arrow_lookup(*at, x, y, ARROW_DIR_W);
		if((*ct)->data.inputs[RALA_IO_WEST]) {
			(*ct)->data.inputs[RALA_IO_WEST]->to = &((*ct)->data);
		}
		(*ct)->data.inputs[RALA_IO_EAST] = arrow_lookup(*at, x, y, ARROW_DIR_E);
		if((*ct)->data.inputs[RALA_IO_EAST]) {
			(*ct)->data.inputs[RALA_IO_EAST]->to = &((*ct)->data);
		}

		(*ct)->data.outputs[RALA_IO_NORTH] = arrow_lookup(*at, x, y+1, ARROW_DIR_S);
		if((*ct)->data.outputs[RALA_IO_NORTH]) {
			(*ct)->data.outputs[RALA_IO_NORTH]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_SOUTH] = arrow_lookup(*at, x, y-1, ARROW_DIR_N);
		if((*ct)->data.outputs[RALA_IO_SOUTH]) {
			(*ct)->data.outputs[RALA_IO_SOUTH]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_WEST] = arrow_lookup(*at, x-1, y, ARROW_DIR_E);
		if((*ct)->data.outputs[RALA_IO_WEST]) {
			(*ct)->data.outputs[RALA_IO_WEST]->from = &((*ct)->data);
		}
		(*ct)->data.outputs[RALA_IO_EAST] = arrow_lookup(*at, x+1, y, ARROW_DIR_W);
		if((*ct)->data.outputs[RALA_IO_EAST]) {
			(*ct)->data.outputs[RALA_IO_EAST]->from = &((*ct)->data);
		}
		result = &((*ct)->data);
	} else if ((*ct)->index == index) {
		(*ct)->data.state = cell_type;
		(*ct)->data.extra_information = extra_information;
		result = &((*ct)->data);
    cell_splay(*ct);
	} else if ((*ct)->index < index) {
		result = cell_insert_index(*ct, &((*ct)->right_subtree), at, x, y, index, cell_type, extra_information);
	} else if ((*ct)->index > index) {
		result = cell_insert_index(*ct, &((*ct)->left_subtree), at, x, y, index, cell_type, extra_information);
	}
	return result;
}

int arrow_insert_index(arrow_tree_t* parent, arrow_tree_t** at, cell_tree_t** ct, int x, int y, arrow_dir_t arrow_dir, index_t index, arrow_type_t arrow_type) {
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
    (*at)->parent = parent;
    (*at)->ptr_to_here = at;

		(*at)->data.to = cell_lookup(*ct, x, y);

		switch(arrow_dir) {
			case ARROW_DIR_N:
				(*at)->data.from = cell_lookup(*ct, x, y+1);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_SOUTH] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_NORTH] = &((*at)->data);
				}
				break;
			case ARROW_DIR_S:
				(*at)->data.from = cell_lookup(*ct, x, y-1);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_NORTH] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_SOUTH] = &((*at)->data);
				}
				break;
			case ARROW_DIR_E:
				(*at)->data.from = cell_lookup(*ct, x+1, y);
				if((*at)->data.from) {
					(*at)->data.from->outputs[RALA_IO_WEST] = &((*at)->data);
				}
				if((*at)->data.to) {
					(*at)->data.to->inputs[RALA_IO_EAST] = &((*at)->data);
				}
				break;
			case ARROW_DIR_W:
				(*at)->data.from = cell_lookup(*ct, x-1, y);
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
    arrow_splay(*at);
		return 1;
	} else if ((*at)->index < index) {
		result = arrow_insert_index(*at, &((*at)->right_subtree), ct, x, y, arrow_dir, index, arrow_type);
	} else if ((*at)->index > index) {
		result = arrow_insert_index(*at, &((*at)->left_subtree), ct, x, y, arrow_dir, index, arrow_type);
	}
	return result;
}

rala_cell_t* cell_lookup_index(cell_tree_t* t, index_t index) {
  //printf("Looking up %d\n", index);
  //cell_print(0,t);
	if(t == NULL) {return NULL;}
	if(t->index == index) {
    cell_splay(t);
    return &(t->data);}
	if(t->index < index) {return cell_lookup_index(t->right_subtree, index);}
	return cell_lookup_index(t->left_subtree,  index);
}

rala_arrow_t* arrow_lookup_index(arrow_tree_t* t, index_t index) {
	if(t == NULL) {return NULL;}
	if(t->index == index) {
    // arrow_splay(t);
    return &(t->data);}
	if(t->index < index) {return arrow_lookup_index(t->right_subtree, index);}
	return arrow_lookup_index(t->left_subtree,  index);
}

