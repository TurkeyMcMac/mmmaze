#include "rand.h"

RAND_TYPE rand_gen(RAND_TYPE *rand)
{
        *rand = (MAX_RAND & (1103515245UL * *rand)) + 12345;
	*rand = (MAX_RAND & (*rand << 4)) | ((0xF0000000UL & *rand) >> 28);
	return *rand;
}
