#include "rand.h"

RAND_TYPE rand_gen(RAND_TYPE *rand)
{
	RAND_TYPE upper_4;
        *rand = (MAX_RAND & (1103515245 * *rand)) + 12345;
	upper_4 = *rand & 0xF0000000;
	*rand = (MAX_RAND & (*rand << 4)) | (upper_4 >> 28);
	return *rand;
}

RAND_TYPE rand_gen_bounded(RAND_TYPE bound, RAND_TYPE *rand)
{
	RAND_TYPE unbounded = rand_gen(rand);
	return unbounded / (MAX_RAND / bound);
}
