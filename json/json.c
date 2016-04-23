#include "json.h"

char* merge_delete(char* a, char* b, char* joiner){
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	size_t lenj = strlen(joiner);

	char* ret = malloc(sizeof(char)*(lena+lenb+lenj+1));

	memcpy(ret+0,a,lena);
	memcpy(ret+lena,joiner,lenj);
	memcpy(ret+lena+lenj,b,lenb);

	ret[lena+lenb+lenj] = 0;
	
	free(a);
	free(b);

	return ret;
}


char* json_array(const char* fmt, jsonarray_t* array){
	char* ret;
	size_t len = array->length;
	return ret;
}


