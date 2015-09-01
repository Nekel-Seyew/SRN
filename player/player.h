#ifndef PLAYER_H
#define PLAYER_H
#include "../GenericList.h"
#include "../items/item.h"

struct player_char{
	char chars[7][13];
	int colors[7][13];
}

struct Player{
	long health;
	long max_health;
	//Stats
	long constitution;
	long strength;
	long dexterity;
	long intelligence;
	long wisdom;
	long charisma;
	//Defenses
	long armor;
	long footing;
	float dodge;
	//attack
	long attack;
	//inventory
	List(item_t)* inventory;
	//misc
	List(string)* background;
	struct player_char sprite;
};




#endif
