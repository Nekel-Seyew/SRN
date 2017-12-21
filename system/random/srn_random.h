#ifndef SRN_RANDOM_H
#define SRN_RANDOM_H

#include <stdint.h>

void     seed_rand(uint64_t i);
uint64_t genrand();
char*    get_seed();
#endif
