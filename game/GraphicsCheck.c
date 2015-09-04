#include <ncurses.h>
#include "gameMode.h"

int graphics_test_data = 0;

void special_chars_test(WINDOW* win, int y){
	//y = (y<=8 ? y : 8);
	//y = (y>=0 ? y : 0);
	char* specials[] = {"Upper left corner         %c  ACS_ULCORNER\n",
	                    "Lower left corner         %c  ACS_LLCORNER\n",
			    "Upper right corner        %c  ACS_URCORNER\n",
			    "Lower right corner        %c  ACS_LRCORNER\n",
			    "Tee pointing right        %c  ACS_LTEE\n",
			    "Tee pointing left         %c  ACS_RTEE\n",
			    "Tee pointing up           %c  ACS_BTEE\n",
			    "Tee pointing down         %c  ACS_TTEE\n",
			    "Horizontal line           %c  ACS_HLINE\n",
			    "Vertical line             %c  ACS_VLINE\n",
			    "Large Plus or cross over  %c  ACS_PLUS\n",
			    "Scan Line 1               %c  ACS_S1\n",
			    "Scan Line 3               %c  ACS_S3\n",
			    "Scan Line 7               %c  ACS_S7\n",
			    "Scan Line 9               %c  ACS_S9\n",
			    "Diamond                   %c  ACS_DIAMOND\n",
			    "Checker board (stipple)   %c  ACS_CKBOARD\n",
			    "Degree Symbol             %c  ACS_DEGREE\n",
			    "Plus/Minus Symbol         %c  ACS_PLMINUS\n",
			    "Bullet                    %c  ACS_BULLET\n",
			    "Arrow Pointing Left       %c  ACS_LARROW\n",
			    "Arrow Pointing Right      %c  ACS_RARROW\n",
			    "Arrow Pointing Down       %c  ACS_DARROW\n",
			    "Arrow Pointing Up         %c  ACS_UARROW\n",
			    "Board of squares          %c  ACS_BOARD\n",
			    "Lantern Symbol            %c  ACS_LANTERN\n",
			    "Solid Square Block        %c  ACS_BLOCK\n",
			    "Less/Equal sign           %c  ACS_LEQUAL\n",
			    "Greater/Equal sign        %c  ACS_GEQUAL\n",
			    "Pi                        %c  ACS_PI\n",
			    "Not equal                 %c  ACS_NEQUAL\n",
			    "UK pound sign             %c  ACS_STERLING\n"};
	char specialChar[] = {ACS_ULCORNER,ACS_LLCORNER,ACS_URCORNER,ACS_LRCORNER,
	ACS_LTEE,ACS_RTEE,ACS_BTEE,ACS_TTEE,ACS_HLINE,
	ACS_VLINE,ACS_PLUS,ACS_S1,ACS_S3,ACS_S7,ACS_S9,
	ACS_DIAMOND,ACS_CKBOARD,ACS_DEGREE,ACS_PLMINUS,
	ACS_BULLET,ACS_LARROW,ACS_RARROW,ACS_DARROW,
	ACS_UARROW,ACS_BOARD,ACS_LANTERN,ACS_BLOCK,
	ACS_LEQUAL,ACS_LEQUAL,ACS_GEQUAL,ACS_PI,ACS_NEQUAL,
	ACS_STERLING};
	int i = 0;
	for(i=0; i<23; ++i){
		mvwprintw(win,i+1,1,specials[i+y],specialChar[i+y]);
	}
}

int graphics_test_draw(WINDOW* win){
	werase(win);
	special_char_test(win,graphics_test_data);
	int i=0,j=0;
	for(i = 0; i<8; ++i){
		for(j = 0; j<8; ++j){
			wattron(win,COLOR_PAIR(i*8+j));
			mvwprintw(win,(35+i)-graphics_test_data,j+2,"aa");
			wattroff(win,COLOR_PAIR(i*8+j));
		}
	}
	return 0;
}

int graphics_test_update(int userinput){
	if(userinput == KEY_UP){
		graphics_test_data -=1;
	}else if(userinput == KEY_DOWN){
		graphics_test_data +=1;
	}else if(userinput == 'q'){
		return RETURN;
	}

	if(graphics_test_data < 0){
		graphics_test_data = 0;
	}else if(graphice_test_data >  20){
		graphics_test_data = 20;
	}
	return 0;
}

gameMode_t graphics_test = {&graphics_test_draw,&graphics_test_update,(void*)&graphics_test_data};
