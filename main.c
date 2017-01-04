#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* for getopt */
#include <getopt.h> /* for getopt */
#include <ncurses.h> /*for all the ncurses stuff */
#include "GenericList.h"
#include "game/gameMode.h"
#include "graphics/graphics.h"

typedef char* string;
//all lists go here
//define_list(string)
//define_list(gameMode_t)

gameMode_t null_mode = {0,0,0};
//extern variables
extern gameMode_t playerCreationMode; 
extern gameMode_t escape_screen;

int gameModeCmp(gameMode_t a, gameMode_t b){
	return (a.stateData == b.stateData);
}

void print_startup_options_help(){
	printf(" SRN is a terminal based RPG-MUD\n");
	printf(" To use this program, use the following parameters.\n");
	printf("	-s		host game as a server. Needs a portnum as well.\n");
	printf("	-p [num]	use this portnum to either host on this\n");
        printf("			computer, or connect to a server using this portnum.\n");
	printf("	-g [server]	connect to this server.\n");
	printf("	-h		print help.\n");
	printf("	-v		be verbose in loging.\n");
}

int main(int argc, string argv[]){
	int c;
	int portnum;
	char* game_address;
	int verbose=0, server=0;
	while ( (c = getopt(argc, argv, "sp:g:hv")) != -1){
		switch(c){
			case 's':
				server = 1;
				break;
			case 'p':
				portnum = atoi(optarg);
				break;
			case 'g':
				game_address = optarg;
				break;
			case 'h':
				print_startup_options_help();
				exit(1);
			case 'v':
				verbose = 1;
				break;
		}
	}
	if(server){
		//launch server code
	}
	else{
		clock_t timeNowMs = (1000* clock())/(CLOCKS_PER_SEC);
		init_ncurses();
		int done = 0;
		gameMode_t game_mode = playerCreationMode;
		gameMode_t prev_mode = null_mode;
		do{
			//add timing code while loop
			//add updating and drawing code
			int c = getch();
			if(c == 27 && !gameModeCmp(game_mode,escape_screen)){ //EscapeKey
				mvprintw(0,0,"GOING TO ESCAPE SCREEN");
				prev_mode = game_mode;
				game_mode = escape_screen; 
			}
			int update = game_mode.update(c);
			int draw   = game_mode.draw(stdscr);
			wrefresh(stdscr);	
			if(update == EXIT_GAME){
				done = 1;
			}else if(update == RETURN){
				game_mode = prev_mode;
				prev_mode = null_mode;
			} 			

			//timing code
			/*clock_t now = (1000* clock())/(CLOCKS_PER_SEC);
 			int k = 0;
			while((now = now = (1000* clock())/(CLOCKS_PER_SEC)) - timeNowMs < 16){
				k += 1;
			}
			timeNowMs = now;*/

		}while(!done);
		endwin();
	}
	return 0;
}
