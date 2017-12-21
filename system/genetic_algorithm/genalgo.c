#include "genalgo.h"
#include <stdlib.h>
#include <string.h>
#include "../random/srn_random.h"
#include "../../Collections-C/src/array.h"

//we want everything to be from HIGHEST to LOWEST
static int compar(const void* a, const void* b){
	ga_organism_t* aga = (ga_organism_t*)a;
	ga_organism_t* bga = (ga_organism_t*)b;
	if(aga->fitness > bga->fitness) return -1;
	else if(aga->fitness == bga->fitness) return 0;
	else return 1;
}

static ArrayConf repoconf;
static Array* repoarr = NULL;

static ga_organism_t** reproduce(ga_organism_t* a, ga_organism_t* b, float rand, ga_repo_method form, size_t* num, void (*mutate)(ga_gene_t* gene)){
	if(repoarr = NULL){
		array_conf_init(&repoconf);
		repoconf.exp_factor=2;
		conf.capacity=10;
	}
	uint64_t randmod=1;
	while(randmod * rand < 1){
		randmod*=10;
	}
	ga_organism_t** children=NULL;
	int i=0,j=0;
	switch(form){
		case GA_INTERLEAVE:{
			break;
		}
		case GA_HALFNHALF:{
			size_t half = a->length/2;
			ga_organism_t* cha = malloc(sizeof(ga_organism_t));
			ga_organism_t* chb = malloc(sizeof(ga_organism_t));
			cha->genes = malloc(sizeof(ga_organism_t*)*a->length);
			chb->genes = malloc(sizeof(ga_organism_t*)*a->length);
			cha->length = a->length;
			chb->length = a->length;
			memcpy(cha->genes,a->genes,sizeof(ga_gene_t*)*half);
			memcpy(chb->genes,b->genes,sizeof(ga_gene_t*)*half);
			memcpy(cha->genes+(sizeof(ga_gene_t*)*half),b->genes+(sizeof(ga_gene_t*)*half),a->length-half);
			memcpy(chb->genes+(sizeof(ga_gene_t*)*half),a->genes+(sizeof(ga_gene_t*)*half),a->length-half);
			size_t i=0;
			for(i=0; i<a->length; i++){
				int k = genrand()%randmod;
				if(k < rand*randmod) mutate(cha->genes[i]);
				k = genrand()%randmod;
				if(k < rand*randmod) mutate(chb->genes[i]);
			}
			array_add(repoarr, cha);
			array_add(repoarr, chb);
			break;
		}
		case GA_RANDOM:{//fall through to the default case of pure chaotic combinations
			*num = (a->length * (a->length-1));
			for(j=0; j< *num; ++j){
				ga_organism_t* chld = malloc(sizeof(ga_organism_t));
				chld->length = a->length;
				chld->genes = malloc(sizeof(ga_gene_t*)*a->length);
				for(i=0; i<a->length; ++i){
					int k = genrand()%2;
					ga_organism_t* prnt = NULL;
					if(k) prnt = a;
					else  prnt = b;
					k = genrand()%randmod;
					if(k < rand*randmod) mutate(prnt->genes[i]);
					chld->genes[i] = prnt->genes[i];
				}
				array_add(repoarr, chld);
			}
			break;
		}
		default:{//form specifies GENE splice grouping.
			*num = (a->length/form) * (a->length/form);
			size_t genomesize = a->length/form;
			size_t i=0,j=0;
			for(i=0; i<*num; i++){
				ga_organism_t* chld = malloc(sizeof(ga_organism_t));
				chld->genes = malloc(sizeof(ga_gene_t*)*a->length);
				chld->length = a->length;
				for(j=0; j<genomesize; ++j){//copies over the genomes
					int k= genrand()%2;
					ga_organism_t* prnt = NULL;
					if(k) prnt = a;
					else  prnt = b;
					size_t offset = (sizeof(ga_gene_t*)*(genomesize+j));
					memcpy(chld->genes+offset,prnt->genes+offset,sizeof(ga_gene_t*)*genomesize);
				}
				for(j=0; j<a->length; j++){//applies mutations
					int k = genrand()%randmod;
					if(k < rand*randmod) mutate(chld->genes[j]);
				}
				array_add(repoarr, chld);
			}
			break;
		}
			
	}
	children = malloc(sizeof(ga_organism_t*)*array_size(repoarr));
	for(i=0; i< array_size(repoarr); i++){
		ga_organism_t* hold;
		array_get_at(repoarr, i,(void**) &ga_organism_t);
		children[i] = hold;
	}
	*num = array_size(repoarr);
	array_remove_all(repoarr);
	return children;
}

static void delete(ga_organism_t* d){
	size_t i=0;
	for(i=0; i<d->length; i++){
		ga_gene_t* gene = d->genes[i];
		free(gene->data);
	}
	free(d->genes);
	free(d);
}

//Will iterate the specified number of times, keeping the top gen_size number of items, and then returning num_ret top organisms. If there are not enough thanks to the cut-off being too high, the current_gen will not be replaced
ga_organism_t* iterate(ga_system_t* sys, int times, unsigned int fitness_cutoff, int gen_size, int num_ret, float mutation, ga_repo_method form){
	int i=0;
	size_t j=0, l=0;
	ga_organism_t** output = malloc(sizeof(ga_organism_t*)*num_ret);
	ArrayConf conf;
	Array* gen_hold;
	array_conf_init(&conf);
	conf.capacity = 100;
	conf.exp_factor = 2;
	array_new_conf(&conf, &gen_hold);
	//gen_size MUST be at least num_ret, anything else doesn't make sense
	if(gen_size < num_ret) return NULL;
	//start the magic
	for(i=0; i<times; i++){
		//randomly pairs and breeds children.
		//keeps the children which pass the fitness criteria
		for(j=0; j<sys->length; i++){
			size_t k=genrand() % sys->length;
			size_t num=0;
			ga_organism_t** children = NULL
			if(sys->reproduction != NULL){
				children = sys->reproduction(sys->current_gen[j],sys->current_gen[k],mutation, form, &num,sys->mutate);

			}else{
				children = reproduce(sys->current_gen[j],sys->current_gen[k],mutation, form, &num,sys->mutate);
			}
			for(l=0; l<num; l++){
				unsigned int ftns = sys->fitness(children[l]);
				if(ftns >= fitness_cutoff){//good child
					array_add(gen_hold,children[l]);
					children[l]->fitness = ftns;
				}else{//bad child
					delete(children[l]);
				}
			}
			free(children);//cleaning up data
		}
		//organize all children from best to worst
		array_sort(gen_hold, compar);
		//if we didn't produce enough children, the constraints are too tight
		//cleanup system, and return NULL(Failure)
		if(array_size(gen_hold) < gen_size){
			for(j=0; j<array_size(gen_hold); j++){
				ga_organism_t* d;
				array_get_at(gen_hold, j,(void**)&d);
				delete(d);
			}
			array_destroy(gen_hold);
			return NULL;
		}
		//keep the requested number of children in the system for future iterations
		ga_organism_t** this_gen = malloc(sizeof(ga_organism_t*)*gen_size);
		ga_organism_t* hold;
		for(j=0; j<gen_size; j++){
			array_get_at(gen_hold,j,(void**)&hold);
			this_gen[j] = hold;
		}
		//eliminate unecessary children (which are also thanks to sorting, lesser
		for(j=gen_size; j<array_size(gen_hold); j++){
			ga_organism_t* hold;
			array_get_at(gen_hold, j,(void**) &hold);
			delete(hold);
		}
		array_remove_all(gen_hold);
		free(sys->current_gen);
		sys->current_gen=this_gen;
		sys->length=gen_size;
	}
	for(i=0; i<num_ret; i++){
		output[i] = sys->current_gen[i];
	}
	return output;
}


