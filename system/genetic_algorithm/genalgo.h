#ifndef SRN_GENETIC_ALGORITHM_H
#define SRN_GENETIC_ALGORITHM_H

typedef enum reproduction_method{
	GA_INTERLEAVE = 1000,
	GA_HALFNHALF  = 1100,
	GA_RANDOM     = 1200,
	
}ga_repo_method;

typedef struct genetic_gene{
	void* data;
	size_t length;
}ga_gene_t;

typedef struct genetic_organism{
	ga_gene_t** genes;
	size_t length;
	unsigned int fitness;
}ga_organism_t;

typedef struct genetic_system{
	unsigned int  (*fitness)(ga_organism_t* organism);
	void (*mutate)(ga_gene_t* gene);
	ga_organism_t** (*reproduction)(ga_organism_t* a, ga_organism_t* b, float rand, ga_repo_method form, size_t* retnum, void (*mutate)(ga_gene_t* gene)) = NULL;
	ga_organism_t** current_gen;
	size_t length;
}ga_system_t;

//Will iterate the specified number of times, keeping the top gen_size number of items, and then returning num_ret top organisms. If there are not enough thanks to the cut-off being too high, the current_gen will not be replaced
ga_organism_t* iterate(ga_system_t* sys, int times, unsigned int fitness_cutoff, int gen_size, int num_ret, float mutation, ga_repo_method form);

#endif
