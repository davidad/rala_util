#include <stdlib.h>

#ifndef INT_AFFINE_H
#define INT_AFFINE_H

typedef struct {
	int xx; int yx; int wx;
	int xy; int yy; int wy;
} affine_t;

struct affine_par {
	affine_t cur;
	struct affine_par* par_next;
};

struct affine_stack {
	affine_t cur;
	struct affine_par* par_next;
	struct affine_stack* prev;
};

typedef struct affine_stack affine_stack_t;

affine_t affine_compose(affine_t, affine_t);
affine_t affine_translate(int, int);
affine_t identity;
affine_t flip_x;
affine_t flip_y;
affine_t rot90;
affine_t rot180;
affine_t rot270;
affine_t flip_pos_diag;
affine_t flip_neg_diag;

int apply_x(affine_t t, int x, int y);
int apply_y(affine_t t, int x, int y);

int applyv_x(affine_t t, int x, int y);
int applyv_y(affine_t t, int x, int y);

affine_stack_t* affine_init(void);
void affine_save(affine_stack_t **p);
int affine_restore(affine_stack_t **p);
void affine_transform(affine_stack_t **p, affine_t t);

#endif
