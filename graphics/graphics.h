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
		
		int color;
		int bold;
	}border;
	int background_color;
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
void draw_window(WINDOW* win, int x, int y, struct graphics_settings* stng, int refresh);

char* sprite_to_JSON(struct sprite* spr);
void sprite_from_JSON(struct sprite* spr, char* jsn);
void graphics_settings_from_JSON(struct graphics_settings* st, char* jsn);
char* graphics_settings_to_JSON(struct graphics_settings* st);

void change_up_left(char up_left, struct graphics_settings* s);
void change_up_right(char up_right, struct graphics_settings* s);
void change_dn_left(char dn_left, struct graphics_settings* s);
void change_dn_right(char dn_right, struct graphics_settings* s);
void change_up(char up, struct graphics_settings* s);
void change_down(char down, struct graphics_settings* s);
void change_left(char left, struct graphics_settings* s);
void change_right(char right, struct graphics_settings* s);
void change_height(int height, struct graphics_settings* s);
void change_width(int width, struct graphics_settings* s);
void change_color_off(int color_off, struct graphics_settings* s);
void change_border_color(int color, struct graphics_settings* s);
void change_background_color(int color, struct graphics_settings* s);

int get_opposite_color(int color);

struct graphics_settings* copy_global_graphics_settings();
#endif
