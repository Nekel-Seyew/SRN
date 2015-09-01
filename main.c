#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* for getopt */
#include <ncurses.h> /*for all the ncurses stuff */
#include "GenericList.h"
#include "game/gameMode.h"

typedef char* string;
//all lists go here
define_list(string)
define_list(gameMode_t)

//extern variables
extern gameMode_t playerCreationMode; 

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

void init_ncurses(){
	initscr();
	keypad(stdscr, 1);
	if(has_colors()){
		start_color();
		int i=1,j=1;
		for(i=1; (i-1)<8; ++i){
			for(j=1; (j-1)<8; ++j){
				init_pair(10*i + j, (j-1), (i-1));
			}
		}
	}
}

int main(int argc, string argv[]){
	int c;
	int portnum;
	char* game_address;
	int verbose, server;
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
		halfdelay(1);
		int done = 0;
		do{
			//add timing code while loop
			//add updating and drawing code
			int c = getch();
			playerCreationMode.update(c);
			playerCreationMode.draw(stdscr);
			getch();
			//timing code
			clock_t now = (1000* clock())/(CLOCKS_PER_SEC);
 			int k = 0;
			while((now = now = (1000* clock())/(CLOCKS_PER_SEC)) - timeNowMs < 16){
				k += 1;
			}
			timeNowMs = now;

		}while(!done);
		endwin();
	}
	return 0;
}
