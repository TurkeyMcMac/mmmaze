#include "help.h"
#include "common-keys.h"
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

		CASES_QUIT
			return;
		}
	}
}

/* Returns when the user presses a quit key. */
static void wait_for_quit(void)
{
	for (;;) {
		switch (getch()) {
		CASES_QUIT
			return;
		}
	}
}

void help_overview_run(void)
{
	static const char overview[] =
		"The objective of the game is simple: Get cash and get out\n"
		"without getting got. You start at the top left and need\n"
		"to get to the end at the bottom right (marked with an E.)\n"
		"As you go, pick up some $$$ along the way, and stay away\n"
		"from the Ms, which are randomly-wandering monsters. The\n"
		"cash and monsters will spawn over time. The goal is to\n"
		"leave with as much money as possible, but the longer you\n"
		"stay, the more monsters appear, so be careful!";
	mvprintw(0, 0, "Overview\n\n%s\n\n(q) Back", overview);
	clrtobot();

	wait_for_quit();
}

void help_controls_run(void)
{
	static const char controls[] =
		"You can move with the arrow keys, WASD, or HJKL. Pressing\n"
		"I makes you idle for one game tick. Time only passes when\n"
		"you move or idle. You can quit an ongoing game with Q.\n"
		"Enter an uppercase H to bring up this help in-game.\n"
		"That's about it.";
	mvprintw(0, 0, "Controls\n\n%s\n\n(q) Back", controls);
	clrtobot();

	wait_for_quit();
}

void help_parameters_run(void)
{
	static const char parameters[] =
		"Several parameters are available to tune gameplay:\n"
		"- The width and height specify the size of the maze in\n"
		"  (potential) path intersections, or \"nodes.\"\n"
		"- The view distance is the distance you can see (in\n"
		"  tiles, not nodes.)\n"
		"- The cash placement interval is the time between $$$\n"
		"  being placed in the maze.\n"
		"- The maximum placed cash is the limit on the cash amount\n"
		"  placed at any one time.\n"
		"- The monster placement interval is self-explanatory.\n"
		"- The seed is used for random generation.";
	mvprintw(0, 0, "Parameters\n\n%s\n\n(q) Back", parameters);
	clrtobot();

	wait_for_quit();
}
