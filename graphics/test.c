#include "graphics.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
	struct sprite* sprite;
	sprite->height = 7;
	sprite->width = 13;
	sprite->sprite="...........................................................................................";
	int a[91] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
	sprite->color =a ;
	char* json = sprite_to_JSON(sprite);
	printf("%s\n",json);
	return 0;
}
