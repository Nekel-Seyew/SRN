#include <ncurses.h>

#ifndef SRN_GRAPHICS_H
#define SRN_GRAPHICS_H

struct sprite{
	int height;
	int width;
	char* sprite;
	int* color;
};

void init_ncurses();
void draw_char(WINDOW* win, char a, int color, int x, int y);
void draw_sprite(WINDOW* win, struct sprite* spr, int x, int y);
char* sprite_to_JSON(struct sprite* spr);
void sprite_from_JSON(struct sprite* spr, char* jsn);
#endif
