#include "srn_random.h"
#include "../../asprintf.c/asprintf.h"

static uint64_t seed = 570956270388484096L;

inline void seed_rand(uint64_t i){
	seed = i;
}
inline uint64_t genrand(){
	uint64_t m = 9223372036854775808UL; 
	uint64_t a = 6364136223846793005UL;
	uint64_t c = 1442695040888963407UL;
	seed = (a*seed + c) % m;
	return seed;
}

char* get_seed(){
	char* a;
	asprintf(&a,"%li",seed);
	return a;
}

