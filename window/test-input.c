#include "../graphics/graphics.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "input_win.h"

int main(){
	init_graphics(NULL);
	init_ncurses();
	WINDOW* test_input;
	int stdwidth, stdheight;
	getmaxyx(stdscr,stdheight,stdwidth);
	test_input = newwin(10,10,10,10);
	draw_window(test_input,10,10,NULL,1);
	//box(test_input,'-','-');
	wrefresh(test_input);
	struct input_win* input= input_win_create(test_input,NULL,0,10);
	int ch;
	refresh();
	while((ch = getch()) != KEY_F(1)){
		mvprintw(0,0,"char was: %c",ch);
		//draw_window(test_input,10,10,NULL);
		//box(test_input,'-','-');
		//wrefresh(test_input);
		input_win_update(input,ch);
		input_win_draw(input);
		refresh();
	}
	endwin();
	return 0;
}
