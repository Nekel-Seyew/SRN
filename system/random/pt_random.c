#include "srn_random.h"
#include "../../asprintf.c/asprintf.h"
#include <pthread.h>
#include <time.h>

//THis is the thread safe version. Use it.

pthread_mutex_t randlock = PTHREAD_MUTEX_INITIALIZER; 

static uint64_t seed = 570956270388484096L;

inline void seed_rand(uint64_t i){
	pthread_mutex_lock(&randlock); 	
	seed = i;
	pthread_mutex_unlock(&randlock);
}
inline uint64_t genrand(){
	pthread_mutex_lock(&randlock); 	
	uint64_t m = 9223372036854775808UL; 
	uint64_t a = 6364136223846793005UL;
	uint64_t c = 1442695040888963407UL;
	seed = (a*seed + c) % m;
	uint64_t k = seed;
	pthread_mutex_unlock(&randlock);
	return k;
}

char* get_seed(){
	pthread_mutex_lock(&randlock); 	
	char* a;
	asprintf(&a,"%li",seed);
	pthread_mutex_unlock(&randlock);
	return a;
}

