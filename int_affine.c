#include "int_affine.h"

affine_t affine_compose(affine_t a, affine_t b) {
	affine_t r;
	r.xx = b.xx*a.xx + b.xy*a.yx;
	r.xy = b.xx*a.xy + b.xy*a.yy;
	r.yx = b.yx*a.xx + b.yy*a.yx;
	r.yy = b.yx*a.xy + b.yy*a.yy;
	r.wx = b.wx*a.xx + b.wy*a.yx + a.wx;
	r.wy = b.wy*a.yy + b.wx*a.xy + a.wy;
	return r;
}

affine_t affine_translate(int x, int y) {
	affine_t r;
	r.xx = r.yy = 1;
	r.xy = r.yx = 0;
	r.wx = x;
	r.wy = y;
	return r;
}

int apply_x(affine_t t, int x, int y) {
	return t.xx*x + t.xy*y + t.wx;
}

int apply_y(affine_t t, int x, int y) {
	return t.yx*x + t.yy*y + t.wy;
}

int applyv_x(affine_t t, int x, int y) {
	return t.xx*x + t.xy*y;
}

int applyv_y(affine_t t, int x, int y) {
	return t.yx*x + t.yy*y;
}

affine_stack_t* affine_init(void) {
	affine_stack_t *p = malloc(sizeof(affine_stack_t));
	p->prev = NULL;
	p->cur.xx = p->cur.yy = 1;
	p->cur.xy = p->cur.yx = p->cur.wx = p->cur.wy = 0;
	return p;
}

void affine_save(affine_stack_t **p) {
	affine_stack_t *old = *p;
	*p = malloc(sizeof(affine_stack_t));
	(*p)->cur = old->cur;
	(*p)->prev = old;
}

int affine_restore(affine_stack_t **p) {
	if((*p)->prev == NULL) {
		return 1;
	}
	affine_stack_t *old = *p;
	*p = (*p)->prev;
	free(old);
	return 0;
}

void affine_transform(affine_stack_t **p, affine_t t) {
	(*p)->cur = affine_compose((*p)->cur, t);
}

affine_t identity = {.xx = 1, .yy = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t flip_x = {.xx = -1, .yy = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t flip_y = {.yy = -1, .xx = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t rot90 = {.xy = 1, .yx = -1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t rot180 = {.xx = -1, .yy = -1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t rot270 = {.xy = -1, .yx = 1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t flip_pos_diag = {.xy = 1, .yx = 1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t flip_neg_diag = {.xy = -1, .yx = -1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
