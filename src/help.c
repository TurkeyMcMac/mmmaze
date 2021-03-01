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
		"to get to the end at the bottom right. As you go, pick up\n"
		"some $$$ along the way, and stay away from the Ms, which\n"
		"are randomly-wandering monsters. The cash and monsters\n"
		"will spawn over time. You only have a limited visual\n"
		"range, so you can't plan the whole path at once. The game\n"
		"is turn-based; things only move when you do.";
	mvprintw(0, 0, "Overview\n\n%s\n\n(q) Back", overview);
	clrtobot();

	wait_for_quit();
}

void help_controls_run(void)
{
	static const char controls[] =
		"You can move with the arrow keys, WASD, or HJKL. Pressing\n"
		"E makes you idle for one game tick. You can quit an\n"
		"ongoing game with Q. Enter an uppercase H to bring up\n"
		"this help in-game. That's about it.";
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
