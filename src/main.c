#include "maze.h"
#include "other.h"
#include "player.h"
#include "rand.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define VIEW_DIST 11

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
	struct other other = { 0, 0, BIT_LEFT };
	int ox, oy;
	int key;
	struct maze maze;
	RAND_TYPE rand = time(NULL);

	initscr();
	noecho();
	curs_set(0);

	maze_generate(&maze, 30, 30, &rand);

	maze_random_node(&maze, &ox, &oy, &rand);
	other.x = ox;
	other.y = oy;

	do {
		int x, y;
		TILE_TYPE t;
		player_mark_seen(&maze, px, py, VIEW_DIST);
		for (y = -VIEW_DIST; y <= VIEW_DIST; ++y) {
			for (x = -VIEW_DIST; x <= VIEW_DIST; ++x) {
				chtype ch = ' ';
				int mx = x + px, my = y + py;
				if (mx >= 0 && my >= 0
				 && mx < maze.width && my < maze.height) {
					t = MAZE_GET(&maze, mx, my);
					if (t & BIT_PLAYER_SEEN) {
						if (x == 0 && y == 0) {
							ch = '@';
						} else if (mx == 0 && my == 0) {
							ch = 'S';
						} else if (mx == maze.width - 1
						 && my == maze.height - 1) {
							ch = A_STANDOUT | 'E';
						} else if (mx == other.x
						 && my == other.y) {
							ch = 'O';
						} else {
							ch = get_path_char(t);
						}
					}
				}
				mvaddch(y + VIEW_DIST, x + VIEW_DIST, ch);
			}
		}
		player_unmark_seen(&maze, px, py, VIEW_DIST);
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

