#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* for getopt */
#include <ncurses.h> /*for all the ncurses stuff */
#include "GenericList.h"

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
		init_ncurses();
		//add timing code while loop
		//add updating and drawing code
		endwin();
	}
	return 0;
}
