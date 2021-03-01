#ifndef PLAYER_H_
#define PLAYER_H_

/* There is one player at some position on the maze. */

struct maze;

/* Marks a part of the maze with BIT_PLAYER_SEEN. The player can see all tiles
 * up to the given distance from their position. 0 is returned unless
 * allocation fails internally, in which case -1 is returned and the marking is
 * incomplete. */
int player_mark_seen(struct maze *maze, int x, int y, int dist);

/* Does the same as above, but removes BIT_PLAYER_SEEN instead. */
int player_unmark_seen(struct maze *maze, int x, int y, int dist);

#endif /* PLAYER_H_ */
