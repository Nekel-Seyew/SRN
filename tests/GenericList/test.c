#include <stdlib.h>
#include <stdio.h>
#include "../../GenericList.h"
#include <time.h>
#include <assert.h>

define_list(int)

int cmp(const void* a, const void* b){
	int aa = *((int*)a);
	int bb = *((int*)b);
	return (aa) - (bb);
}

int main(int argc, char** argv){
	srand(time(NULL));
	List(int)* mylist = new_list(int);
	int i=0;
	int size = rand()%1000000;
	printf("Creating List, size: %i\n",size);
	for(i=0; i<size; ++i){
		list_add(mylist,i);
	}
	printf("Checking to make sure additions happened correctly\n");
	for(i=1; i<list_size(mylist); ++i){
		assert(list_at(mylist,i) > list_at(mylist,i-1));
	}
	int times = rand() % list_size(mylist);
	printf("Going to remove %i items\n",times);
	for(i=0; i<times; ++i){
		list_remove(mylist,rand() % list_size(mylist),cmp);
	}
	printf("Asserting no copies\n");
	for(i=1; i<list_size(mylist); ++i){
		assert(list_at(mylist,i) > list_at(mylist,i-1));
	}
	List(int)* test = new_list(int);
	list_add(test,1);
	list_add(test,2);
	list_add(test,3);
	list_remove(test,2,cmp);
	assert(list_size(test) ==2);
	assert(list_at(test,0) == 1);
	assert(list_at(test,1) == 3);
	return 0;
}
