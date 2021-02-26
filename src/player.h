#ifndef PLAYER_H_
#define PLAYER_H_

struct maze;

int player_mark_seen(struct maze *maze, int x, int y, int dist);

int player_unmark_seen(struct maze *maze, int x, int y, int dist);

#endif /* PLAYER_H_ */
