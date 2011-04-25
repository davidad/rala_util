/*
 * Hierarchy rendering code
 */

#include "hierarchy.h"

void render_box( cairo_t* cr, Setting& box ) {
	if ( not box.isGroup() ) {
		cerr << "Each entry in field `boxes' must be a group." << endl;
		return;
	}

	string name;
	int x, y, w, h;
	double col_r, col_g, col_b;
	string color_name;
	string label_location;

	box.lookupValue("name", name);
	box.lookupValue("x", x);
	box.lookupValue("y", y);
	box.lookupValue("w", w);
	box.lookupValue("h", h);
	box.lookupValue("red", col_r);
	box.lookupValue("green", col_g);
	box.lookupValue("blue", col_b);

	//cout << "{ x: " << x << " y: " << y << " w: " << w << " h: " << h << " }" << endl;

	cairo_save( cr );
	cairo_set_source_rgb( cr, col_r, col_g, col_b );
	cairo_move_to( cr, x*2-0.5, -y*2+1.5 );
	cairo_rel_line_to( cr, w*2, 0.0 );
	cairo_rel_line_to( cr, 0.0, -h*2 );
	cairo_rel_line_to( cr, -w*2, 0.0 );
	cairo_close_path( cr );
	//cairo_rel_line_to( cr, 0.0, -h );
	//cairo_rel_line_to( cr, w, 0.0 );
	cairo_fill( cr );
	cairo_restore( cr );
	//cairo_stroke( cr );

}

extern "C" void hierarchy_draw_boxes( cairo_t* cr, const char* hierarchy_path ) {
	Config conf;
	try {
		conf.readFile( hierarchy_path );
	} catch (libconfig::ConfigException msg) {
		cerr << "Exception when attempting to read and parse file: " << hierarchy_path << endl;
		return;
	}

	if ( not conf.exists("boxes") ) {
		cerr << "No field `boxes' in configuration file." << endl;
		return;
	}

	Setting& boxes = conf.lookup("boxes");

	if ( not boxes.isList() ) {
		cerr << "Field `boxes' must be a list." << endl;
		return;
	}

	for (int ii=0; ii<boxes.getLength(); ii++) {
		render_box(cr, boxes[ii]);
	}

}

