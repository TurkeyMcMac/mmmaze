#ifndef RAND_H_
#define RAND_H_

#include <limits.h>

#define MAX_RAND 4294967295UL

#if UINT_MAX >= MAX_RAND
typedef unsigned RAND_TYPE;
#else
typedef unsigned long RAND_TYPE;
#endif

RAND_TYPE rand_gen(RAND_TYPE *rand);

#endif /* RAND_H_ */
