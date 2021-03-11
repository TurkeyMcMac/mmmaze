#include "ui.h"
#include "common-keys.h"
#include "help.h"
#include "play-menu.h"
#include <curses.h>

static void run_main_menu(void)
{
	for (;;) {
		mvaddstr(0, 0,
			"Monster Money Maze\n\n"
			"(p) Play\n"
			"(?) Help\n"
			"(q) Quit");
		clrtobot();

		switch (getch()) {
		case 'p':
			play_menu_run();
			break;
		case '?':
			help_run();
			break;
		CASES_QUIT
			return;
		}
	}
}

void ui_run(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	run_main_menu();

	endwin();
}
