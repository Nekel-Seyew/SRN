#include <ncurses.h>
#include "../GenericList.h"

#define EXIT_GAME 101
#define RETURN    102

#ifndef GAME_MODE_H_
#define GAME_MODE_H_
typedef struct gameMode{
	int (*draw)(WINDOW* win);
	int (*update)(int userInput);
	void* stateData;
}gameMode_t;

#endif
