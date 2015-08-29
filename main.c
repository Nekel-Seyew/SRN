#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> /* for getopt */
#include "GenericList.h"

define_list(int)

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

int main(int argc, char** argv){
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
	return 0;
}
