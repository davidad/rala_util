#ifndef HIERARCHY_HR_
#define HIERARCHY_H_

#include "rala_display.h"

using namespace std;
#include <iostream>
#include <string>
#include <libconfig.h++>
using namespace libconfig;

extern "C" void hierarchy_draw_boxes( cairo_t* cr, const char* hierarchy_path );

#endif

