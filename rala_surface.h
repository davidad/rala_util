typedef int rala_cell_t;

#define RALA_NODE_SINGLE 0
#define RALA_NODE_X 1
#define RALA_NODE_Y 2

struct rala_surface_cell {
	rala_cell_t state;
	int x;
	int y;
}

struct rala_surface_node {
	struct rala_surface_cell *single_cell;
	int node_type; //0 for single cell, 1 for X split, 2 for Y split
	int split_point; //Split is </>=, not <=/>
	struct rala_surface_node *lesser;
	struct rala_surface_node *greater;
};

struct rala_surface {
	struct rala_surface_node *head;
	int x0;
	int x1;
	int y0;
	int y1;
