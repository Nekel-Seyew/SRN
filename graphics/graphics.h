#include <ncurses.h>

#ifndef SRN_GRAPHICS_H
#define SRN_GRAPHICS_H

struct graphics_settings{
	struct border{
		char up_left;
		char up_right;
		char dn_left;
		char dn_right;
		char up;
		char left;
		char right;
		char down;
	}border;
	int color;
	int width;
	int height;
	int color_off;
};

struct sprite{
	int height;
	int width;
	char* sprite;
	int* color;
};
//if passed in parameter is null, then default settings created and returned, else, return passed in
void init_graphics(char* json);
void init_ncurses();
void draw_char(WINDOW* win, char a, int color, int x, int y);
void draw_sprite(WINDOW* win, struct sprite* spr, int x, int y);
void draw_window(WINDOW* win, int x, int y, struct graphics_settings* stng);
char* sprite_to_JSON(struct sprite* spr);
void sprite_from_JSON(struct sprite* spr, char* jsn);
void graphics_settings_from_JSON(struct graphics_settings* st, char* jsn);
void graphics_settings_to_JSON(struct graphics_settings* st);
#endif
