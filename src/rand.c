#include "rand.h"

RAND_TYPE rand_gen(RAND_TYPE *rand)
{
	/* LCG: */
        *rand = (*rand * 1103515245UL + 12345) & MAX_RAND;
	return *rand;
}
