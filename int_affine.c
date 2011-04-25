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
	return t.xx*x + t.yx*y + t.wx;
}

int apply_y(affine_t t, int x, int y) {
	return t.xy*x + t.yy*y + t.wy;
}

int applyv_x(affine_t t, int x, int y) {
	return t.xx*x + t.yx*y;
}

int applyv_y(affine_t t, int x, int y) {
	return t.xy*x + t.yy*y;
}

int applyd_x(affine_t t, char d) {
	switch(d) {
		case 'N':
		case 'n':
			return applyv_x(t,0,1);
		case 'E':
		case 'e':
			return applyv_x(t,1,0);
		case 'S':
		case 's':
			return applyv_x(t,0,-1);
		case 'W':
		case 'w':
			return applyv_x(t,-1,0);
	}
}

int applyd_y(affine_t t, char d) {
	switch(d) {
		case 'N':
		case 'n':
			return applyv_y(t,0,1);
		case 'E':
		case 'e':
			return applyv_y(t,1,0);
		case 'S':
		case 's':
			return applyv_y(t,0,-1);
		case 'W':
		case 'w':
			return applyv_y(t,-1,0);
	}
}

affine_stack_t* affine_init(void) {
	affine_stack_t *p = malloc(sizeof(affine_stack_t));
	p->prev = NULL;
	p->par_next = NULL;
	p->par_cur = NULL;
	p->cur.xx = p->cur.yy = 1;
	p->cur.xy = p->cur.yx = p->cur.wx = p->cur.wy = 0;
	return p;
}

void init_par(affine_par_t *p) {
	p->x_offset = 0;
	p->y_offset = 0;
	p->x_range_min = 0;
	p->y_range_min = 0;
	p->x_range_max = 0;
	p->y_range_max = 0;
	p->par_next = NULL;
	p->cur = identity;
}

void free_pars(affine_stack_t *p) {
	affine_par_t* par_iter = p->par_next;
	while(par_iter!=NULL) {
		affine_par_t* to_free = par_iter;
		par_iter=par_iter->par_next;
		free(to_free);
	}
}

void affine_save(affine_stack_t **p) {
	affine_stack_t *old = *p;
	*p = malloc(sizeof(affine_stack_t));
	(*p)->cur = old->cur;
	(*p)->prev = old;
	(*p)->par_cur = NULL;
	if(old->par_next != NULL) {
		(*p)->par_next = malloc(sizeof(affine_par_t));
		init_par((*p)->par_next);
		(*p)->trivial_par = true;
	} else {
		(*p)->par_next = NULL;
		(*p)->trivial_par = false;
	}
}

int affine_restore(affine_stack_t **p) {
	if((*p)->prev == NULL) {
		return 1;
	}
	affine_stack_t *old = *p;
	*p = (*p)->prev;
	free_pars(old);
	free(old);
	return 0;
}

void affine_start_split(affine_stack_t **p) {
	if((*p)->trivial_par) {
		free_pars(*p);
		(*p)->par_next = malloc(sizeof(affine_par_t));
		init_par((*p)->par_next);
		(*p)->par_cur = (*p)->par_next;
		(*p)->trivial_par =false;
	} else {
		affine_par_t** par_iter = &((*p)->par_next);
		while((*par_iter)!=NULL) par_iter=&((*par_iter)->par_next);
		(*par_iter) = malloc(sizeof(affine_par_t));
		(*p)->par_cur = (*par_iter);
		init_par(*par_iter);
		if((*p)->prev == NULL || (*p)->prev->par_next == NULL) {
			(*par_iter)->cur = (*p)->cur;
		}
	}
}

int affine_end_split(affine_stack_t **p) {
	(*p)->par_cur = NULL;
}

void affine_transform(affine_stack_t **p, affine_t t) {
	if((*p)->par_cur) {
		(*p)->par_cur->cur = affine_compose((*p)->par_cur->cur, t);
	}
	else if ((*p)->par_next) {
		affine_par_t* par_iter = (*p)->par_next;
		while(par_iter!=NULL) {
			par_iter->cur = affine_compose(par_iter->cur, t);
			par_iter=par_iter->par_next;
		}
	}
	(*p)->cur = affine_compose((*p)->cur, t);
}

void affine_operate(affine_stack_t *transforms, affine_operator_t op, void* data) {
	affine_par_t* par_iter;
	int x, y;
	if(transforms->par_next == NULL) {
		op(data, transforms->cur);
	} else {
		affine_stack_t* j = transforms;
		while(j != NULL && j->par_next != NULL) {
			j->par_cur = j->par_next;
			j->par_cur->x_cur = j->par_cur->x_range_min;
			j->par_cur->y_cur = j->par_cur->y_range_min;
			j = j->prev;
		}
		while(1) {
			affine_t composite = identity;
			j = transforms;
			while(j != NULL && j->par_cur != NULL) {
				composite = affine_compose(j->par_cur->cur, composite);
				composite = affine_compose(affine_translate(j->par_cur->x_cur*j->par_cur->x_offset,j->par_cur->y_cur*j->par_cur->y_offset), composite);
				j = j->prev;
			}
			op(data, composite);
			j = transforms;
			while(j != NULL && j->par_cur != NULL && j->par_cur->par_next == NULL && j->par_cur->x_cur >= j->par_cur->x_range_max && j->par_cur->y_cur >= j->par_cur->y_range_max) {
				j->par_cur = j->par_next;
				j->par_cur->x_cur = j->par_cur->x_range_min;
				j->par_cur->y_cur = j->par_cur->y_range_min;
				j = j->prev;
			}
			if(j == NULL || j->par_next == NULL ) {
				j = transforms;
				while(j != NULL && j->par_cur != NULL) {
					j->par_cur = NULL;
					j = j->prev;
				}
				break;
			}
			if(j->par_cur->x_cur < j->par_cur->x_range_max) {
				j->par_cur->x_cur++;
			} else if (j->par_cur->y_cur < j->par_cur->y_range_max) {
				j->par_cur->x_cur = j->par_cur->x_range_min;
				j->par_cur->y_cur++;
			} else {
				j->par_cur = j->par_cur->par_next;
				j->par_cur->x_cur = j->par_cur->x_range_min;
				j->par_cur->y_cur = j->par_cur->y_range_min;
			}
		}
		j = transforms;
		while(j != NULL && j->par_cur != NULL) {
			j->par_cur = NULL;
			j = j->prev;
		}
	}
}


affine_t identity = {.xx = 1, .yy = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t flip_x = {.xx = -1, .yy = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t flip_y = {.yy = -1, .xx = 1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t rot90 = {.xy = 1, .yx = -1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t rot180 = {.xx = -1, .yy = -1, .xy = 0, .yx = 0, .wx = 0, .wy = 0};
affine_t rot270 = {.xy = -1, .yx = 1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t flip_pos_diag = {.xy = 1, .yx = 1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
affine_t flip_neg_diag = {.xy = -1, .yx = -1, .xx = 0, .yy = 0, .wx = 0, .wy = 0};
