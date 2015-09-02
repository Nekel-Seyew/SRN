#include <ncurses.h>
#include "gameMode.h"

struct escape_screen_data{
	int options;
	int choice;
};

char* escape_screen_options[] = {"Return","Exit Game"};
struct escape_screen_data escreen_data={2,-1};

int escape_screen_draw(WINDOW* win){
	werase(win);
	int y,x;
	getmaxyx(win,y,x);
	//box drawn
	int i=0;
	for(i=(x/2)-10; i<((x/2)-10)+20; ++i){
		mvwaddch(win,(y/2)-2,i,ACS_HLINE);
		mvwaddch(win,(y/2)+1,i,ACS_HLINE);
	}
	for(i=(y/2)-2; i<((y/2)-2)+4; ++i){
		mvwaddch(win,i,(x/2)-10,ACS_VLINE);
		mvwaddch(win,i,(x/2)+10,ACS_VLINE);
	}
	mvwaddch(win,(y/2)-2,(x/2)-10,ACS_ULCORNER);
	mvwaddch(win,(y/2)-2,(x/2)+10,ACS_URCORNER);
	mvwaddch(win,(y/2)+1,(x/2)-10,ACS_LLCORNER);
	mvwaddch(win,(y/2)+1,(x/2)+10,ACS_LRCORNER);
	
	for(i=0; i<escreen_data.options; ++i){
		if(i == escreen_data.choice){
			wattron(win,A_STANDOUT);
		}
		mvwprintw(win,(y/2)-1+i,(x/2)-9,"%s",escape_screen_options[i]);
		if(i == escreen_data.choice){
			wattroff(win,A_STANDOUT);
		}
	}
	
	return 0;
}

int escape_screen_update(int userInput){
	
	if(userInput == KEY_UP){
		escreen_data.choice -= 1;
	}else if(userInput == KEY_DOWN){
		escreen_data.choice += 1;
	}else if(userInput == '\n'){
		switch(escreen_data.choice){
			case 0:
				return RETURN;
			case 1:
				return EXIT_GAME;
			default:
				break;
		}
	}
	
	if(escreen_data.choice < 0){
		escreen_data.choice = 0;
	}
	if(escreen_data.choice >= escreen_data.options){
		escreen_data.choice = escreen_data.options-1;
	}
	return 0;
}

gameMode_t escape_screen = {&escape_screen_draw,&escape_screen_update,(void*)&escreen_data};
