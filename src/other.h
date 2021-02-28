#ifndef OTHER_H_
#define OTHER_H_

#include "maze.h"

struct other {
	short x, y;
	TILE_TYPE back_dir;
	TILE_TYPE next_move;
};

void other_init(struct other *other, int x, int y, struct maze *maze);

void other_start_move_random(struct other *other, const struct maze *maze,
	RAND_TYPE *rand);

void other_make_move(struct other *other, struct maze *maze);

#endif /* OTHER_H_ */
