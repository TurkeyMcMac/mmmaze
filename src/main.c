#include "bits.h"
#include "maze.h"
#include "other.h"
#include "player.h"
#include "rand.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define VIEW_DIST 5

static chtype get_path_char(TILE_TYPE tile)
{
	int r = tile & BIT_RIGHT;
	int u = tile & BIT_UP;
	int l = tile & BIT_LEFT;
	int d = tile & BIT_DOWN;
	chtype ch = ACS_BULLET; /* No connections. */
	if (!r && !u && !l &&  d) ch = ACS_VLINE;
	if (!r && !u &&  l && !d) ch = ACS_HLINE;
	if (!r && !u &&  l &&  d) ch = ACS_URCORNER;
	if (!r &&  u && !l && !d) ch = ACS_VLINE;
	if (!r &&  u && !l &&  d) ch = ACS_VLINE;
	if (!r &&  u &&  l && !d) ch = ACS_LRCORNER;
	if (!r &&  u &&  l &&  d) ch = ACS_RTEE;
	if ( r && !u && !l && !d) ch = ACS_HLINE;
	if ( r && !u && !l &&  d) ch = ACS_ULCORNER;
	if ( r && !u &&  l && !d) ch = ACS_HLINE;
	if ( r && !u &&  l &&  d) ch = ACS_TTEE;
	if ( r &&  u && !l && !d) ch = ACS_LLCORNER;
	if ( r &&  u && !l &&  d) ch = ACS_LTEE;
	if ( r &&  u &&  l && !d) ch = ACS_BTEE;
	if ( r &&  u &&  l &&  d) ch = ACS_PLUS;
	return ch;
}

int main(void)
{
	int px = 0, py = 0;
	struct other other = { 1, 1, BIT_LEFT };
	int key;
	struct maze maze;
	RAND_TYPE rand = time(NULL);

	initscr();
	noecho();

	maze_generate(&maze, 30, 30, &rand);

	do {
		int x, y;
		TILE_TYPE t;
		erase();
		player_mark_seen(&maze, px, py, VIEW_DIST);
		for (y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
			for (x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
				int mx = x + px, my = y + py;
				if (mx >= 0 && my >= 0
				 && mx < maze.width && my < maze.height) {
					int sx = (x + VIEW_DIST) * 2,
					    sy = (y + VIEW_DIST) * 2;
					t = MAZE_GET(&maze, mx, my);
					if (t & BIT_PLAYER_SEEN) {
						if ((t & BIT_RIGHT)
						 && (MAZE_GET(&maze, mx + 1, my)
							& BIT_PLAYER_SEEN))
							mvaddch(sy,
								sx + 1,
								ACS_HLINE);	
						if ((t & BIT_DOWN)
						 && (MAZE_GET(&maze, mx, my + 1)
							& BIT_PLAYER_SEEN))
							mvaddch(sy + 1,
								sx,
								ACS_VLINE);
						if (mx == 0 && my == 0) {
							mvaddch(sy, sx, 'S');
						} else if (mx == maze.width - 1
						 && my == maze.height - 1) {
							mvaddch(sy, sx,
								A_STANDOUT
									| 'E');
						} else if (mx == other.x
						 && my == other.y)
						{
							mvaddch(sy, sx, 'O');
						} else {
							mvaddch(sy, sx,
								get_path_char(
									t));
						}
					}
				}
			}
		}
		player_unmark_seen(&maze, px, py, VIEW_DIST);
		mvaddch(VIEW_DIST * 2, VIEW_DIST * 2, '@');
		move(VIEW_DIST * 2, VIEW_DIST * 2);
		refresh();
		key = getch();
		t = MAZE_GET(&maze, px, py);
		switch (key) {
		case 'w':
			if (t & BIT_UP) --py;
			break;
		case 'a':
			if (t & BIT_LEFT) --px;
			break;
		case 's':
			if (t & BIT_DOWN) ++py;
			break;
		case 'd':
			if (t & BIT_RIGHT) ++px;
			break;
		}
		other_move_random(&other, &maze, &rand);
	} while (key != 'q');

	endwin();
	return EXIT_SUCCESS;
}

