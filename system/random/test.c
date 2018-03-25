#include <stdlib.h>
#include <stdio.h>

#include "srn_random.h"

int main(int argc, char** argv){
	int i=0;
	//seed_rand()
	for(i=0; i< 10; i++){
		printf("%li\n",genrand());
	}
	seed_rand(100);
	for(i=0; i< 100; i++){
		printf("%li\n",genrand());
	}
	printf("%s\n",get_seed());
	return 0;

}
