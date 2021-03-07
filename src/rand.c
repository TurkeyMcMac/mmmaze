#include "rand.h"

RAND_TYPE rand_gen(RAND_TYPE *rand)
{
	/* LCG: */
        *rand = (MAX_RAND & (1103515245UL * *rand)) + 12345;
	return *rand;
}
