#include "commands.h"
#include "int_affine.h"
#include "rala_enums.h"
#include "rala_tree.h"
#include "rala_cells.h"
#include "rala_avr_load.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void rala_avr_load_updater(void* v) {;}
void rala_avr_load_clear(void* cl) {
	rala_avr_load_state_t* state = (rala_avr_load_state_t*)cl;
	state->cell_tree = NULL;
	state->arrow_tree = NULL;
}
void rala_avr_load_set_cell_cb(void* v, affine_t t) {
	set_cell_cb_t* arg = (set_cell_cb_t*)v;
	rala_avr_load_state_t* state = (rala_avr_load_state_t*)arg->cl;
	cell_insert(&state->cell_tree, state->arrow_tree, t.wx, t.wy, arg->cell_type, arg->extra_information);
}
void rala_avr_load_set_arrow_cb(void* v, affine_t t) {
	set_arrow_cb_t* arg = (set_arrow_cb_t*)v;
	rala_avr_load_state_t* state = (rala_avr_load_state_t*)arg->cl;
	arrow_insert(&state->arrow_tree, state->cell_tree, t.wx, t.wy, arrow_rotate(t, arg->arrow_dir), arg->arrow_type);
}

int main(int argc, char** argv) {
	char buf;
	int i = 1;
	int x=0,y=0,h=24;
	rala_avr_load_state_t state;
	state.cell_tree = NULL;
	state.arrow_tree = NULL;
	FILE* input = stdin;
	char* tty = "/dev/ttyUSB0";
	while(i < argc) {
		if(!strncmp(argv[i], "--x=", 4)) {
			x = atoi(strchr(argv[i],'=')+1);
		} else if(!strncmp(argv[i], "--y=", 4)) {
			y = atoi(strchr(argv[i],'=')+1);
		} else if(!strncmp(argv[i], "--h=", 4)) {
			h = atoi(strchr(argv[i],'=')+1);
		} else if(!strcmp(argv[i], "--tty")) {
			tty = argv[++i];
		} else if ((input = fopen(argv[i],"r")) == NULL) {
			fprintf(stderr, "Error opening file %s\n", argv[i]);
			input = stdin;
		}
		i++;
	}
	while((buf = fgetc(input)) != EOF) {
		next_command_char(buf, &state, rala_avr_load_set_cell_cb, rala_avr_load_set_arrow_cb, rala_avr_load_clear, rala_avr_load_updater, NULL);
	}
	int dy;
	unsigned char* data=malloc(28*h);
	unsigned char* ef1=data;
	unsigned char* ef2=data+h*1;
	unsigned char* cross=data+h*2;
	unsigned char* tf_pcl=data+h*3;
	unsigned char* tf_pch=data+h*4;
	unsigned char* inv_t=data+h*5;
	unsigned char* ctrl_e=data+h*6;
	unsigned char* ctrl_n=data+h*7;
	unsigned char* ctrl_w=data+h*8;
	unsigned char* ctrl_s=data+h*9;
	unsigned char* copy_e=data+h*10;
	unsigned char* copy_n=data+h*11;
	unsigned char* copy_w=data+h*12;
	unsigned char* copy_s=data+h*13;
	unsigned char* is_cpy=data+h*14;
	unsigned char* is_del=data+h*15;
	unsigned char* out_e=data+h*16;
	unsigned char* out_n=data+h*17;
	unsigned char* out_w=data+h*18;
	unsigned char* out_s=data+h*19;
	unsigned char* ve=data+h*20;
	unsigned char* vn=data+h*21;
	unsigned char* vw=data+h*22;
	unsigned char* vs=data+h*23;
	unsigned char* te=data+h*24;
	unsigned char* tn=data+h*25;
	unsigned char* tw=data+h*26;
	unsigned char* ts=data+h*27;

	for(dy=0; dy<h; dy++) {
		ef1[dy] = ef2[dy] = cross[dy] = tf_pcl[dy] = tf_pch[dy] = inv_t[dy] = ctrl_e[dy] = ctrl_n[dy] = ctrl_w[dy] = ctrl_s[dy] = copy_e[dy] = copy_n[dy] = copy_w[dy] = copy_s[dy] = is_cpy[dy] = is_del[dy] = out_e[dy] = out_n[dy] = out_w[dy] = out_s[dy] = ve[dy] = vn[dy] = vw[dy] = vs[dy] = te[dy] = tn[dy] = tw[dy] = ts[dy] = 0;
		for(i=0; i<8; i++) {
			rala_cell_t* cell = cell_lookup(state.cell_tree,x+i,y+dy);
			unsigned char bit = (1<<i);
			int num_inputs = 0;
			if(cell != NULL) {
				if(cell->outputs[RALA_IO_EAST] != NULL && cell->outputs[RALA_IO_EAST]->state != ARROW_TYPE_NONE) {
					out_e[dy] |= bit;
				}
				if(cell->outputs[RALA_IO_NORTH] != NULL && cell->outputs[RALA_IO_NORTH]->state != ARROW_TYPE_NONE) {
					out_n[dy] |= bit;
				}
				if(cell->outputs[RALA_IO_WEST] != NULL && cell->outputs[RALA_IO_WEST]->state != ARROW_TYPE_NONE) {
					out_w[dy] |= bit;
				}
				if(cell->outputs[RALA_IO_SOUTH] != NULL && cell->outputs[RALA_IO_SOUTH]->state != ARROW_TYPE_NONE) {
					out_s[dy] |= bit;
				}
				switch(cell->state) {
					case CELL_TYPE_WIRE:
						tf_pcl[dy] |= bit;
						break;
					case CELL_TYPE_OR:
						tf_pcl[dy] |= bit;
					case CELL_TYPE_AND:
						tf_pch[dy] |= bit;
						break;
					case CELL_TYPE_XOR:
						tf_pcl[dy] |= bit;
						break;
					case CELL_TYPE_NAND:
						inv_t[dy] |= bit;
						break;
					case CELL_TYPE_CROSSOVER:
						cross[dy] |= bit;
						break;
					case CELL_TYPE_COPY_N:
						ctrl_n[dy] |= bit;
						copy_n[dy] |= bit;
						is_cpy[dy] |= bit;
						break;
					case CELL_TYPE_COPY_S:
						ctrl_s[dy] |= bit;
						copy_s[dy] |= bit;
						is_cpy[dy] |= bit;
						break;
					case CELL_TYPE_COPY_W:
						ctrl_w[dy] |= bit;
						copy_w[dy] |= bit;
						is_cpy[dy] |= bit;
						break;
					case CELL_TYPE_COPY_E:
						ctrl_e[dy] |= bit;
						copy_e[dy] |= bit;
						is_cpy[dy] |= bit;
						break;
					case CELL_TYPE_DELETE_N:
						ctrl_n[dy] |= bit;
						is_del[dy] |= bit;
						break;
					case CELL_TYPE_DELETE_S:
						ctrl_s[dy] |= bit;
						is_del[dy] |= bit;
						break;
					case CELL_TYPE_DELETE_W:
						ctrl_w[dy] |= bit;
						is_del[dy] |= bit;
						break;
					case CELL_TYPE_DELETE_E:
						ctrl_e[dy] |= bit;
						is_del[dy] |= bit;
						break;
				}
				if(cell->inputs[RALA_IO_EAST] != NULL) {
					switch(cell->inputs[RALA_IO_EAST]->state) {
						case ARROW_TYPE_1:
							te[dy] |= bit;
						case ARROW_TYPE_0:
							ve[dy] |= bit;
						case ARROW_TYPE_X:
							num_inputs++;
							if((is_del[dy]&bit) &&(!(ctrl_e[dy]&bit))) {
								ctrl_n[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								ctrl_e[dy] |= bit;
							}
							if((is_cpy[dy]&bit) &&(!(ctrl_e[dy]&bit))) {
								ctrl_n[dy] &= ~bit;
								copy_n[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								copy_s[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								copy_w[dy] &= ~bit;
								ctrl_e[dy] |= bit;
								copy_e[dy] |= bit;
							}
					}
				}
				if(cell->inputs[RALA_IO_NORTH] != NULL) {
					switch(cell->inputs[RALA_IO_NORTH]->state) {
						case ARROW_TYPE_1:
							tn[dy] |= bit;
						case ARROW_TYPE_0:
							vn[dy] |= bit;
						case ARROW_TYPE_X:
							num_inputs++;
							if((is_del[dy]&bit) &&(!(ctrl_n[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								ctrl_n[dy] |= bit;
							}
							if((is_cpy[dy]&bit) &&(!(ctrl_n[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								copy_e[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								copy_s[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								copy_w[dy] &= ~bit;
								ctrl_n[dy] |= bit;
								copy_n[dy] |= bit;
							}
					}
				}
				if(cell->inputs[RALA_IO_WEST] != NULL) {
					switch(cell->inputs[RALA_IO_WEST]->state) {
						case ARROW_TYPE_1:
							tw[dy] |= bit;
						case ARROW_TYPE_0:
							vw[dy] |= bit;
						case ARROW_TYPE_X:
							num_inputs++;
							if((is_del[dy]&bit) &&(!(ctrl_w[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								ctrl_n[dy] &= ~bit;
								ctrl_w[dy] |= bit;
							}
							if((is_cpy[dy]&bit) &&(!(ctrl_w[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								copy_e[dy] &= ~bit;
								ctrl_s[dy] &= ~bit;
								copy_s[dy] &= ~bit;
								ctrl_n[dy] &= ~bit;
								copy_n[dy] &= ~bit;
								ctrl_w[dy] |= bit;
								copy_w[dy] |= bit;
							}
					}
				}
				if(cell->inputs[RALA_IO_SOUTH] != NULL) {
					switch(cell->inputs[RALA_IO_SOUTH]->state) {
						case ARROW_TYPE_1:
							ts[dy] |= bit;
						case ARROW_TYPE_0:
							vs[dy] |= bit;
						case ARROW_TYPE_X:
							num_inputs++;
							if((is_del[dy]&bit) &&(!(ctrl_s[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								ctrl_n[dy] &= ~bit;
								ctrl_s[dy] |= bit;
							}
							if((is_cpy[dy]&bit) &&(!(ctrl_s[dy]&bit))) {
								ctrl_e[dy] &= ~bit;
								copy_e[dy] &= ~bit;
								ctrl_w[dy] &= ~bit;
								copy_w[dy] &= ~bit;
								ctrl_n[dy] &= ~bit;
								copy_n[dy] &= ~bit;
								ctrl_s[dy] |= bit;
								copy_s[dy] |= bit;
							}
					}
				}
				if(num_inputs <= 1 && (!(cross[dy] &bit))) {
					if(inv_t[dy] &bit) {
						tf_pcl[dy] |= bit;
					}
					ef1[dy] |= bit;
				} else {
					if(inv_t[dy] &bit) {
						tf_pch[dy] |= bit;
					}
					ef2[dy] |= bit;
				}
			}
		}
	}
	for(i = 0; i<28*h; i++) {
		printf("y:%d,s:%d - 0x%2x\n", i%h, i/h, data[i]);
		fflush(stdout);
	}
	return 0;
}
