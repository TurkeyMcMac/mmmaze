#include "help.h"
#include <curses.h>

void help_run(void)
{
	for (;;) {
		mvaddstr(0, 0,
			"Help\n\n"
			"(a) Overview\n"
			"(b) Controls\n"
			"(c) Parameters\n"
			"\n(q) Back");
		clrtobot();

		switch (getch()) {
		case 'a':
			help_overview_run();
			break;
		case 'b':
			help_controls_run();
			break;
		case 'c':
			help_parameters_run();
			break;

		case 'q':
			return;
		}
	}
}

void help_overview_run(void)
{
	static const char overview[] =
		"The objective of the game is simple: Get cash and get out\n"
		"without getting got. As you look for the end of the maze,\n"
		"pick up some $$$ along the way, and stay away from the Ms\n"
		"which are monsters. The cash and monsters will spawn as\n"
		"you go. You only have a limited visual range, so you\n"
		"can't plan the whole path at once. The game is\n"
		"turn-based, so things only move when you do.";
	mvprintw(0, 0, "Overview\n\n%s\n\n(q) Back", overview);
	clrtobot();

	while (getch() != 'q')
		;
}

void help_controls_run(void)
{
	static const char controls[] =
		"You can move with WASD. Pressing E makes you idle for one\n"
		"game tick. You can quit an ongoing game with Q. H is also\n"
		"available in-game to bring up this help. That's about it.";
	mvprintw(0, 0, "Controls\n\n%s\n\n(q) Back", controls);
	clrtobot();

	while (getch() != 'q')
		;
}

void help_parameters_run(void)
{
	static const char parameters[] =
		"Several parameters are available to tune gameplay:\n"
		"- The width and height specify the size of the maze in\n"
		"  (potential) path intersections.\n"
		"- The view distance is the distance you can see (in\n"
		"  tiles, not intersections.)\n"
		"- The cash placement interval is the time between $$$\n"
		"  being placed in the maze.\n"
		"- The maximum placed cash is the limit on the cash amount\n"
		"  placed at any one time.\n"
		"- The monster placement interval is self-explanatory.\n"
		"- The seed is used for random generation.";
	mvprintw(0, 0, "Parameters\n\n%s\n\n(q) Back", parameters);
	clrtobot();

	while (getch() != 'q')
		;
}
