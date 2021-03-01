#ifndef RAND_H_
#define RAND_H_

#include <limits.h>

/* The maximum value returned as a random number. */
#define MAX_RAND 4294967295UL

/* The random type must hold at least 32 bits. */
#if UINT_MAX >= MAX_RAND
typedef unsigned RAND_TYPE;
#else
typedef unsigned long RAND_TYPE;
#endif

/* Generates a random number, modifying the given random state. */
RAND_TYPE rand_gen(RAND_TYPE *rand);

#endif /* RAND_H_ */
