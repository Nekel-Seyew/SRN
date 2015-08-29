#ifndef ITEM_H
#define ITEM_H

struct item{
	char type[16];
	long effect_val;
	char stat[5];
	float weight;
};

struct item_array{
	struct item* array;
	int size; //actual number of items
	int length; //length of the array
}

struct item_array* new_item_array(int size);
int add_item_item_array(struct item_array* a, struct item* i);
int remove_item_item_array(struct item_array* a, struct item* i);
#endif
