#include "rala_comp.h"
#include <stdlib.h>
#include <stdio.h>
//#include <errno.h>
//#include <unistd.h>

rala_queue_t* rala_queue_init(void) {
	rala_queue_t* q = malloc(sizeof(rala_queue_t));
	q->head = NULL;
	q->tail = NULL;
	return q;
}

void rala_enqueue(rala_queue_t* q, rala_cell_t* cell) {
	if(q != NULL) {
		if(q->tail == NULL) {
			q->head = q->tail = malloc(sizeof(rala_queue_node_t));
		} else {
			rala_queue_node_t* n = malloc(sizeof(rala_queue_node_t));
			q->tail->next = n;
			q->tail = n;
		}
		q->tail->next = NULL;
		q->tail->cell = cell;
	}
}

rala_cell_t* rala_dequeue(rala_queue_t* q) {
	if(q == NULL || q->head == NULL) {return NULL;}
	rala_queue_node_t* dequeued = q->head;
	q->head = dequeued->next;
	if(q->head == NULL) q->tail = NULL;
	rala_cell_t* cell = dequeued->cell;
	free(dequeued);
	return cell;
}

bool rala_cell_fire(rala_cell_t* cell, rala_queue_t* q, arrow_notify_t arrow_notify, bool side_effects_on) {
	int i, result, dir, control_dir, control, data;
	int return_v;
	bool touch_inputs = true;
	bool remove_dir = true;
	bool do_fire = false;
	bool morefiledata = false;
	int file_size;
	int previous_file_location;
	char byte_buffer;
	//printf("Random Cell FD: %i\n", cell->extra_information);
	if(cell == NULL) {return false;}
	switch(cell->state) {
		case CELL_TYPE_AND:
			result = 1;
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state == ARROW_TYPE_X) {
						return false;
					} else if (cell->inputs[i]->state >= ARROW_TYPE_0) {
						result &= cell->inputs[i]->state-ARROW_TYPE_0;
					}
				}
			}
			break;
		case CELL_TYPE_OR:
			result = 0;
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state == ARROW_TYPE_X) {
						return false;
					} else if (cell->inputs[i]->state >= ARROW_TYPE_0) {
						result |= cell->inputs[i]->state-ARROW_TYPE_0;
					}
				}
			}
			break;
		case CELL_TYPE_XOR:
			result = 0;
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state == ARROW_TYPE_X) {
						return false;
					} else if (cell->inputs[i]->state >= ARROW_TYPE_0) {
						result ^= cell->inputs[i]->state-ARROW_TYPE_0;
					}
				}
			}
			break;
		case CELL_TYPE_SINK:
			result = 0;
			break;
		case CELL_TYPE_FILE:
		      // For the file cell, extra_information is used to hold the filedescriptor of the file to stream from
		    //printf("Cell FD: %i\n", cell->extra_information);
            //errno = 0;
            //return_v = fread(&byte_buffer, sizeof(char), 1, cell->extra_information);

            if (cell->extra_information == NULL) {
                return false;
            }

              // Warning: Don't do this unless side effects are enabled
            if (side_effects_on) {
                return_v = fgetc(cell->extra_information);
            } else {
                return false;
            }

            //previous_file_location = ftell(cell->extra_information);
            //fseek(cell->extra_information, 0, SEEK_END); // seek to end of file
            //file_size = ftell(cell->extra_information); // get current file pointer
            //fseek(cell->extra_information, previous_file_location, SEEK_SET); // seek back to where we were in the file

            //morefiledata = false;
            do_fire = true;
            //if (file_size > previous_file_location) {
            if (return_v == EOF || return_v == 13 || return_v == 10) {
                return false;
                //morefiledata = true;
                //do_fire = true;
            }

		    //printf( "Return value: %i\n", retval );
		    //printf( "Errno: %i\n", errno );
		    //fprintf( stderr, "Result: %i\n", byte_buffer );
		    //fprintf( stderr, "Result: %i\n", byte_buffer );
			result = (return_v-48)&1;
			//fprintf( stderr, "%i ", return_v );
			//fflush( stderr );
			break;
		case CELL_TYPE_NAND:
			result = 1;
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state == ARROW_TYPE_X) {
						return false;
					} else if (cell->inputs[i]->state >= ARROW_TYPE_0) {
						result &= cell->inputs[i]->state-ARROW_TYPE_0;
					}
				}
			}
			result = 1 - result;
			break;
		case CELL_TYPE_WIRE:
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state >= ARROW_TYPE_0) {
						result = cell->inputs[i]->state-ARROW_TYPE_0;
						do_fire = true;
					}
				}
			}
			if(!do_fire) {
				return false;
			} else {do_fire = false;}
			break;
		case CELL_TYPE_CROSSOVER:
			for(i=0; i<4; i++) {
				if(cell->inputs[i] != NULL) {
					if(cell->inputs[i]->state >= ARROW_TYPE_0) {
						int j = i ^ 1;
						if(cell->outputs[j] != NULL && cell->outputs[j]->state == ARROW_TYPE_X) {
							cell->outputs[j]->state = cell->inputs[i]->state;
							cell->inputs[i]->state = ARROW_TYPE_X;
							do_fire = true;
							if(q != NULL && cell->inputs[i]->from != NULL) {
								rala_enqueue(q,cell->inputs[i]->from);
							}
							if(arrow_notify) arrow_notify(cell->inputs[i]);
							if(q != NULL && cell->outputs[j]->to != NULL) {
								rala_enqueue(q,cell->outputs[j]->to);
							}
							if(arrow_notify) arrow_notify(cell->outputs[j]);
						}
					}
				}
			}
			return do_fire;
		case CELL_TYPE_COPY_N:
		case CELL_TYPE_COPY_S:
		case CELL_TYPE_COPY_W:
		case CELL_TYPE_COPY_E:
			dir = cell->state-CELL_TYPE_COPY_N;
			if(cell->inputs[dir] == NULL) { return false; }
			if((data=(cell->inputs[dir]->state-ARROW_TYPE_0))>=0) {
				for(i=0; i<4; i++) {
					if(cell->inputs[i] != NULL) {
						if(i != dir && (control = (cell->inputs[i]->state - ARROW_TYPE_0))>=0) {
							result = data;
							do_fire = true;
							if(control) {
								remove_dir = false;
							}
						}
					}
				}
			}
			if(!do_fire) {return false;}
			break;
		case CELL_TYPE_DELETE_N:
		case CELL_TYPE_DELETE_S:
		case CELL_TYPE_DELETE_W:
		case CELL_TYPE_DELETE_E:
			dir = cell->state-CELL_TYPE_DELETE_N;
			if(cell->inputs[dir] == NULL) { return false; }
			if((data=(cell->inputs[dir]->state-ARROW_TYPE_0))>=0) {
				for(i=0; i<4; i++) {
					if(i != dir && cell->inputs[i] != NULL && (control = cell->inputs[i]->state - ARROW_TYPE_0)>=0) {
						control_dir = i;
						do_fire = true;
						if(!control) {
							result = data;
						}
					}
				}
			}
			if(!do_fire) {return false;}
			if(control) {
				cell->inputs[dir]->state = ARROW_TYPE_X;
				cell->inputs[control_dir]->state = ARROW_TYPE_X;
				if(q != NULL && cell->inputs[dir]->from != NULL) {
					rala_enqueue(q,cell->inputs[dir]->from);
				}
				if(q != NULL && cell->inputs[control_dir]->from != NULL) {
					rala_enqueue(q,cell->inputs[control_dir]->from);
				}
				if(arrow_notify) {
					arrow_notify(cell->inputs[dir]);
				  arrow_notify(cell->inputs[control_dir]);
				}
				return true;
			}
			break;
	}
	for(i = 0; i<4; i++) {
		if(cell->outputs[i] != NULL && cell->outputs[i]->state >= ARROW_TYPE_0) {
			return false;
		}
	}
	for(i = 0; i<4; i++) {
		if(cell->outputs[i] != NULL && cell->outputs[i]->state == ARROW_TYPE_X) {
			cell->outputs[i]->state = ARROW_TYPE_0+result;
			if(q != NULL && cell->outputs[i]->to != NULL) {
				rala_enqueue(q,cell->outputs[i]->to);
			}
			if(arrow_notify) arrow_notify(cell->outputs[i]);
			do_fire = true;
		}
	}

      //Special handling for the sink cell to endlessly clear inputs
	if (cell->state == CELL_TYPE_SINK) do_fire = true;
    //if (cell->state == CELL_TYPE_FILE && morefiledata) {
        //rala_enqueue(q,cell);
        //return false;
    //}

	if(do_fire) {
		for(i = 0; i<4; i++) {
			if(cell->inputs[i] != NULL) {
				if(cell->inputs[i]->state >= ARROW_TYPE_0) {
					if(remove_dir || i != dir) {
						cell->inputs[i]->state = ARROW_TYPE_X;
						if(q != NULL && cell->inputs[i]->from != NULL) {
							rala_enqueue(q,cell->inputs[i]->from);
						}
						if(arrow_notify) arrow_notify(cell->inputs[i]);
					}
				}
			}
		}
	}
	return do_fire;
}
