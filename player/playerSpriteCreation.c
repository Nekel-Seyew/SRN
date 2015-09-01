#include <ncurses.h>
#include "../game/gameMode.h"

struct player_sprite_creation_data{
	
};

struct player_sprite_creation_data sprc_data;

int player_sprite_creation_draw(WINDOW* win){
	box(win,0,0);
	int i=0;
	//it's row major
	mvaddch(1,32,ACS_ULCORNER);
	mvaddch(1,46,ACS_URCORNER);
	for(i=0; i<13;i++){
		mvaddch(1,33+i,ACS_HLINE);
		mvaddch(9,33+i,ACS_HLINE);
	}
	for(i=0;i<7;i++){
		mvaddch(2+i,32,ACS_VLINE);
		mvaddch(2+i,46,ACS_VLINE);
	}
	mvaddch(9,32,ACS_LLCORNER);
	mvaddch(9,46,ACS_LRCORNER);
	return 0;
}
int player_sprite_creation_update(int charinput){
	return 0;
}
gameMode_t playerCreationMode = {&player_sprite_creation_draw,&player_sprite_creation_update,(void*)&sprc_data};
