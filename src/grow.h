#ifndef GROW_H_
#define GROW_H_

#include <stddef.h>

#define GROW(arr, len, cap) \
	grow((void **)&(arr), &(len), &(cap), sizeof(*(arr)))

void *grow(void **arrp, size_t *lenp, size_t *capp, size_t esize);

#endif /* GROW_H_ */
