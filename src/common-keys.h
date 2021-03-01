#ifndef COMMON_KEYS_H_
#define COMMON_KEYS_H_

#include <curses.h>

#define CASES_QUIT case 'q': case '\033':

#define CASES_BACKSPACE \
	case KEY_BACKSPACE: case '\b': case KEY_DC: case KEY_LEFT:

#define CASES_RIGHT case KEY_RIGHT: case 'd': case 'l':

#define CASES_UP case KEY_UP: case 'w': case 'k':

#define CASES_LEFT case KEY_LEFT: case 'a': case 'h':

#define CASES_DOWN case KEY_DOWN: case 's': case 'j':

#endif /* COMMON_KEYS_H_ */
