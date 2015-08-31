#ifndef ITEM_H
#define ITEM_H

#include "../GenericList.h"

typedef struct item{
	char type[16];
	long effect_val;
	char stat[5];
	float weight;
	List(string)* desc;
}item_t;

define_list(item_t);

#endif
