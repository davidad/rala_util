#include "commands.h"
#include "int_affine.h"
#include "rala_enums.h"
#include "rala_tree.h"
#include "rala_step.h"
#include "rala_comp.h"
#include <stdio.h>
#include <unistd.h>

void rala_step_updater(void* v) {;}
void rala_step_clear(void* cl) {
	rala_step_state_t* state = (rala_step_state_t*)cl;
	state->queue = rala_queue_init();
	state->cell_tree = NULL;
	state->arrow_tree = NULL;
}
void rala_step_set_cell_cb(void* v, affine_t t) {
	set_cell_cb_t* arg = (set_cell_cb_t*)v;
	rala_step_state_t* state = (rala_step_state_t*)arg->cl;
	rala_enqueue(state->queue, cell_insert(&state->cell_tree, state->arrow_tree, t.wx, t.wy, arg->cell_type, arg->extra_information));
	printf("{%d,%d;%s}\n",t.wx,t.wy,cell_type_to_str(arg->cell_type));
	fflush(stdout);
}
void rala_step_set_arrow_cb(void* v, affine_t t) {
	set_arrow_cb_t* arg = (set_arrow_cb_t*)v;
	rala_step_state_t* state = (rala_step_state_t*)arg->cl;
	arrow_insert(&state->arrow_tree, state->cell_tree, t.wx, t.wy, arrow_rotate(t, arg->arrow_dir), arg->arrow_type);
	printf("{%d,%d;%s%s}\n",t.wx,t.wy,arrow_dir_to_str(arrow_rotate(t,arg->arrow_dir)),arrow_type_to_str(arg->arrow_type));
	fflush(stdout);
}

void rala_step_arrow_notify(rala_arrow_t* a) {
	printf("{%d,%d;%s%s}\n",a->x,a->y,arrow_dir_to_str(a->arrow_dir),arrow_type_to_str(a->state));
	fflush(stdout);
	return;
}

int main(int argc, char** argv) {
	char buf;
	int i = 1;
	long cell_updates = 0;
	long total_steps = -1; // Negative one means "infinity" here
	int delay = 0;
	int wait = 0;
	bool write_updates = true;
	rala_step_state_t state;
	state.cell_tree = NULL;
	state.arrow_tree = NULL;
	state.queue = rala_queue_init();
	rala_queue_t* back_queue = rala_queue_init();
	FILE* input = stdin;

	// Process arguments
	while (i < argc) {
		if (!strcmp(argv[i], "--help")) {
			printf("usage: rala_step [--usleep=x] [--steps=x] [--wait] [file]\n");
			printf("  Updates the rala circuit in file, and writes the results live to stdout.\n");
			printf("  If no file is given, stdin is assumed.\n");
			printf("  Options:\n");
			printf("    --usleep=x -- Delay x microseconds after each cell update.\n");
			printf("    --steps=x  -- Update x steps, then spit out the state. (no live updates)\n");
			printf("    --wait     -- Wait for a keypress before simulating. (must specify file)\n");
			exit(0);
		} else if (!strncmp(argv[i], "--usleep=", 9)) {
			delay = atoi(strchr(argv[i],'=')+1);
		} else if (!strcmp(argv[i], "--wait")) {
			wait = 1;
		} else if (!strncmp(argv[i], "--steps=", 8)) {
			total_steps = atoi(strchr(argv[i], '=')+1);
			write_updates = false;
		} else if ((input = fopen(argv[i],"r")) == NULL) {
			fprintf(stderr, "Error opening file %s\n", argv[i]);
			input = stdin;
		}
		i++;
	}

	// Read the input file
	while ((buf = fgetc(input)) != EOF) {
		next_command_char(buf, &state, rala_step_set_cell_cb, rala_step_set_arrow_cb, rala_step_clear, rala_step_updater, NULL);
	}

	// Wait for a keypress, if requested.
	if (wait==1)
		fgetc(stdin);

	// Send an initial frame marker
	if (write_updates)
		printf("%%FRAME%%\n");

	arrow_notify_t arrow_notify_function = NULL;
	if (write_updates)
		arrow_notify_function = rala_step_arrow_notify;

	total_energy = 0.0;

	while (1) {
		if (rala_cell_fire(rala_dequeue(state.queue), back_queue, arrow_notify_function, true)) {
			cell_updates++;
			if (delay > 0) {
				usleep(delay);
			}
		} else {
			if (state.queue->head == NULL) {
				if (back_queue->head == NULL) {
					fprintf(stderr, "Done! Cell updates: %li\nTotal energy: %f\n", cell_updates, total_energy);
					break;
				} else {
					if (write_updates)
						printf("%%FRAME%%\n");

					// Swap the queue buffers
					rala_queue_t* temp = state.queue;
					state.queue = back_queue;
					back_queue = temp;

					// Test if we are only supposed to run a limited number of steps
					if (total_steps > 0) {
						total_steps--;
						if (total_steps == 0)
							exit(1);
					}
				}
			}
		}
	}

	return 0;
}

