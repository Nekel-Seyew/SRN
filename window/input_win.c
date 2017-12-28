#include "input_win.h"
#include "../asprintf.c/asprintf.h"
#include <stdlib.h>
#include "../graphics/graphics.h"
static int is_valid_text(char a){
	char* valid = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1!2@3#4$5%6^7&8*9(0)-_=+[{]}\\|;:'i\",<.>/?`~\t ";
	char* search;
	asprintf(&search, "%c", a);
	char* rest = strstr(valid,search);
	free(search);
	return rest != NULL;
}

struct input_win* input_win_create(WINDOW* win, struct graphics_settings* settings, int x, int y){
	struct input_win* inwin = malloc(sizeof(struct input_win));
	inwin->win = win;
	inwin->settings = ( (settings==NULL) ? copy_global_graphics_settings(): settings);
	
	array_conf_init(&(inwin->hist_conf));
	inwin->hist_conf.capacity = INPUT_WIN_HIST_SIZE;
	array_new_conf(&(inwin->hist_conf), &(inwin->history));
	inwin->head = 0;
	inwin->hist_select=0;
	
	inwin->buffer = malloc(sizeof(char) * (INPUT_WIN_MAX_BUFFER+1));
	memset(inwin->buffer,'\0', sizeof(char) * (INPUT_WIN_MAX_BUFFER+1));
	inwin->buf_head = 0;
	
	inwin->x = x;
	inwin->y = y;
	
	int cy,cx;
	getmaxyx(win,cy,cx);
	int total_num_chars = (cy*cx) -1 -2*cx -2*cy +4; //to account for the border, but add back in the orners to account for double subtract, but then subtract 1 for the > always there
	inwin->draw_length = total_num_chars;
	inwin->height = cy;
	inwin->width = cx;
	
	inwin->has_entered = 0;
	return inwin;
}
void input_win_update(struct input_win* win, int input){
	int i;
	char* a;
	win->has_entered = 0;
	int max_left = 2;
	int max_right = win->settings->width-2;
	//action keys
	switch(input){
		case '\n':
			goto input_win_update_enter;
		case KEY_BACKSPACE:
			goto input_win_update_backspace;
		case KEY_UP:
			goto input_win_update_key_up;
		case KEY_DOWN:
			goto input_win_update_key_down;
		case KEY_LEFT:
			goto input_win_update_key_left;
		case KEY_RIGHT:
			goto input_win_update_key_right;
		case KEY_HOME:
			win->cursor_loc=0;
			return;
		case KEY_END:
			win->cursor_loc=strlen(win->buffer);
			return;
		default:
			if(is_valid_text(input) && win->buf_head < INPUT_WIN_MAX_BUFFER-1){
				//win->buffer[(win->buf_head)++] = input;
				if(win->cursor_loc == win->buf_head){
					win->buffer[(win->buf_head)] = input;
					win->buf_head += 1;
					win->cursor_loc += 1;
					win->draw_head = win->buf_head;
					while(win->buf_head-win->draw_tail > win->draw_length){
						win->draw_tail += 1;
					}
				}
			}
			return;
	}
	return;//should prevent fall-through
	input_win_update_enter:
		if(array_size(win->history) >= INPUT_WIN_HIST_SIZE){
			char* rm;
			array_replace_at(win->history,win->buffer,win->head, (void**)&rm);
			if(rm != NULL){
				free(rm);
			}
		}else{
			array_add_at(win->history,win->buffer,win->head);
		}
		win->hist_select = win->head; //select becomes last item given
		win->head = (win->head + 1) % INPUT_WIN_HIST_SIZE;
		win->buf_head = 0;
		win->buffer = malloc(sizeof(char) * (INPUT_WIN_MAX_BUFFER+1));
		memset(win->buffer, '\0', sizeof(char)*(INPUT_WIN_MAX_BUFFER+1));
		win->draw_head = 0;
		win->draw_tail = 0;
		win->has_entered = 1;
		return;
	input_win_update_backspace:
		if(win->cursor_loc != 0){
			for(i=win->cursor_loc-1; i<win->buf_head; i++){
				win->buffer[i] = win->buffer[i+1];
			}
			win->buffer[(win->buf_head)--]='\0'; 
			win->cursor_loc -= 1;
		}
		return;
	input_win_update_key_up:
		//clear buffer
		win->buf_head = 0;
		memset(win->buffer, '\0', sizeof(char)*(INPUT_WIN_MAX_BUFFER+1));
		//get last item
		array_get_at(win->history,win->hist_select,(void**)&a);
		strcpy(win->buffer,a);
		win->buf_head = strlen(win->buffer);
		win->draw_head = win->buf_head;
		while(win->buf_head-win->draw_tail > win->draw_length){
			win->draw_tail += 1;
		}
		//move the history pointer
		win->hist_select = ((array_size(win->history) == INPUT_WIN_HIST_SIZE) ? (win->hist_select-1)%INPUT_WIN_HIST_SIZE : 0);
		return;
	input_win_update_key_down:
		//clear buffer
		win->buf_head = 0;
		memset(win->buffer, '\0', sizeof(char)*(INPUT_WIN_MAX_BUFFER+1));
		//get last item
		if(win->hist_select >= array_size(win->history)){
			win->draw_head = 0;
			win->draw_tail = 0;
			return;
		}
		array_get_at(win->history,win->hist_select,(void**)&a);
		strcpy(win->buffer,a);
		win->buf_head = strlen(win->buffer);
		win->draw_head = win->buf_head;
		while(win->buf_head - win->draw_tail > win->draw_length){
			win->draw_tail += 1;
		}
		//move the history pointer
		win->hist_select = ((array_size(win->history) == INPUT_WIN_HIST_SIZE) ? (win->hist_select+1)%INPUT_WIN_HIST_SIZE : win->head-1);
		return;
	input_win_update_key_left:
		if(win->cursor_loc == win->draw_tail && win->draw_tail > 0){
			win->draw_tail  -= 1;
			win->draw_head  -= 1;
		}
		win->cursor_loc = ((win->cursor_loc > 0) ? win->cursor_loc-1 : 0);
		return;
	input_win_update_key_right:
		if(win->cursor_loc == win->buf_head) return;
		if(win->cursor_loc == win->draw_head && win->draw_head < INPUT_WIN_MAX_BUFFER){
			win->draw_tail+=1;
			win->draw_head+=1;
		}
		win->cursor_loc = ((win->cursor_loc < INPUT_WIN_MAX_BUFFER) ? win->cursor_loc+1 : INPUT_WIN_MAX_BUFFER);
		return;
	//shouldn't get here, but just in case...
	return;
}
void input_win_draw(struct input_win* win){
	draw_window(win->win, win->x, win->y, win->settings,0);
	mvprintw(1,0,"input win buffer: %s",win->buffer);
	mvwaddch(win->win,1,1,'>');
	int cy=0,cx=0,i = 0;
	int left = win->draw_tail;
	int right = win->draw_head;
	if(win->draw_head - win->draw_tail > win->draw_length){
		left = win->draw_head - (win->draw_length);
	}
	i = left;
	mvprintw(2,0,"draw_tail %i, draw_head %i, left: %i, total_num_chars: %i",win->draw_tail,win->draw_head, left, win->draw_length);
	for(cy=1; cy < win->height; cy++){
		for(cx = 1; cx < win->width-1; cx++){
			if(cy ==1 && cx == 1) continue;
			if(i>=right) goto input_win_draw_buffer_length_reached;
			mvprintw(3,0,"i: %i",i);
			mvprintw(4,0,"cursor_loc: %i",win->cursor_loc);
			if(i == win->cursor_loc) wattron(win->win,A_REVERSE);
			mvwaddch(win->win,cy,cx,win->buffer[i]);
			if(i == win->cursor_loc) wattroff(win->win,A_REVERSE);
			i++;
		}
	}
	input_win_draw_buffer_length_reached:
		
		wrefresh(win->win);
		return;
	//wrefresh(win->win);
}
char* input_win_get_buffer(struct input_win* win){
	return win->buffer;
}


void input_win_destroy(struct input_win* win){
	int i;
	int hist = array_size(win->history);
	
	//delete the strings stored in hist;
	for(i=0; i < hist; i++){
		char* dest;
		array_get_at(win->history, i, (void**)&dest);
		free(dest);
	}
	
	array_destroy(win->history);
	free(win->settings);
	free(win->win);
	free(win);
}

char* get_last_entered(struct input_win* win){
	char* a;
	array_get_at(win->history, win->head, (void**)&a);
	return a;
}
