#include "rala_enums.h"

char* arrow_dir_to_str(arrow_dir_t d) {
	static char* strs[] = {"N", "S", "W", "E"};
	return strs[d];
}

char* arrow_type_to_str(arrow_type_t t) {
	static char* strs[] = {"_", "X", "0", "1"};
	return strs[t];
}

char* cell_type_to_str(cell_type_t t) {
	static char* strs[] = {"", "s", "a", "o", "x", "n", "w", "C", "cN", "cS", "cW", "cE", "dN", "dS", "dW", "dE"};
	return strs[t];
}
