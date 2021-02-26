#ifndef OTHER_H_
#define OTHER_H_

#include "maze.h"

struct other {
	short x, y;
	TILE_TYPE back_dir;
};

void other_move_random(struct other *other, const struct maze *maze,
	RAND_TYPE *rand);

#endif /* OTHER_H_ */
