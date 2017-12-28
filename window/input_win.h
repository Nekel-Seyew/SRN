#ifndef SRN_INPUT_WIN_H
#define SRN_INPUT_WIN_H

#include <stdlib.h>
#include "../Collections-C/src/array.h"
#include <ncurses.h>
#include "../graphics/graphics.h"
#include <string.h>

#define INPUT_WIN_HIST_SIZE 128
#define INPUT_WIN_MAX_BUFFER 1024

struct input_win{
	ArrayConf hist_conf;
	Array* history;
	int head;
	int hist_select;
	

	char* buffer;
	int buf_head;
	int draw_head;//right most
	int draw_tail;//left most
	int draw_length;
	
	struct graphics_settings* settings;
	WINDOW* win;
	int x;
	int y;
	int width;
	int height;
	
	int cursor_loc;
	int has_entered;
};

//If settings is null, then will use default graphics settings
struct input_win* input_win_create(WINDOW* win, struct graphics_settings* settings,int x, int y);

void input_win_update(struct input_win* win, int input);
void input_win_draw(struct input_win* win);

char* input_win_get_buffer(struct input_win* win);

//will free the passed in pointer for you.
//but note, all data stored, including the history, will be deleted
void input_win_destroy(struct input_win* win);

char* get_last_entered(struct input_win* win);

#endif
