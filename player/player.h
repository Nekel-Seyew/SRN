#ifndef PLAYER_H
#define PLAYER_H
#include "../GenericList.h"
#include "../items/item.h"
#include "../graphics/graphics.h"

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
	struct sprite* sprite;
};




#endif
