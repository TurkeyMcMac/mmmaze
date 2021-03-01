#ifndef GROW_H_
#define GROW_H_

#include <stddef.h>

/* Appends an item to the array of the given length and allocation capacity. The
 * arguments are all rvalues and can be modified. The array can be of any known
 * type. The return value is the address of the new item, or NULL if the
 * allocation failed. */
#define GROW(arr, len, cap) \
	grow((void **)&(arr), &(len), &(cap), sizeof(*(arr)))

/* This function is the implementation of the above macro. */
void *grow(void **arrp, size_t *lenp, size_t *capp, size_t esize);

#endif /* GROW_H_ */
