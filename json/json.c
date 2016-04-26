#include "json.h"
#include "../asprintf/asprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* merge_delete(char* a, char* b, char* joiner){
	char* ret = merge_string(a,b,joiner);	
	free(a);
	free(b);
	return ret;
}
char* merge_string(char* a, char* b, char* joiner){
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	size_t lenj = strlen(joiner);

	char* ret = malloc(sizeof(char)*(lena+lenb+lenj+1));
	memset(ret,0,lena+lenb+lenj+1);
	
	strcat(ret,a);
	strcat(ret,joiner);
	strcat(ret,b);
	
	ret[lena+lenb+lenj] = '\0';

	return ret;

}


char* json_array(const char* fmt, jsonarray_t* array){
	char* ret = malloc(sizeof(char)*2);
	ret[0]='['; ret[1]='\0';
	size_t len = array->length;
	size_t i=0;
	for(i=0; i<len; ++i){
		char* a;
		asprintf(&a,fmt,(JSON_TYPE_CONVERT(array->type,array->data)[i]));
		ret = merge_delete(ret,a,",");
	}
	char* fret = malloc(sizeof(char)*2); fret[0]=']'; fret[1]='\0';
	ret = merge_delete(ret,fret,"");
	return ret;
}


