#ifndef PLAYER_H
#define PLAYER_H

#include "../items/item.h"

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
	struct item_array* inventory;
};



#endif
