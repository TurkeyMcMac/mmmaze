#ifndef COMMON_KEYS_H_
#define COMMON_KEYS_H_

#include <curses.h>

/* These are sets of cases for common actions triggered by multiple keys. */

/* Cases for quitting a UI state. */
#define CASES_QUIT case 'q': case '\033':

/* Cases synonymous with BACKSPACE for our purposes. */
#define CASES_BACKSPACE \
	case KEY_BACKSPACE: case '\b': case KEY_DC: case KEY_LEFT:

/* Cases for rightward movement. */
#define CASES_RIGHT case KEY_RIGHT: case 'd': case 'l':

/* Cases for upward movement. */
#define CASES_UP case KEY_UP: case 'w': case 'k':

/* Cases for leftward movement. */
#define CASES_LEFT case KEY_LEFT: case 'a': case 'h':

/* Cases for downward movement. */
#define CASES_DOWN case KEY_DOWN: case 's': case 'j':

#endif /* COMMON_KEYS_H_ */
