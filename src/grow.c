#include "grow.h"
#include <stdlib.h>

void *grow(void **arrp, size_t *lenp, size_t *capp, size_t esize)
{
	size_t old_len = *lenp;
	if (old_len >= *capp) {
		void *new_arr;
		size_t new_cap, new_size;
		new_cap = 16 + old_len + old_len / 2;
		if (new_cap <= old_len) {
			new_cap = old_len + 1;
			if (new_cap <= old_len) return NULL;
		}
		new_size = new_cap * esize;
		if (new_size / esize != new_cap) return NULL;
		new_arr = realloc(*arrp, new_size);
		if (!new_arr) return NULL;
		*capp = new_cap;
		*arrp = new_arr;
	}
	++*lenp;
	return (char *)*arrp + old_len * esize;
}
