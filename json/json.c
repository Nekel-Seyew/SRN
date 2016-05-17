#include "json.h"
#include "../asprintf.c/asprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* truefalse[2] = {"true", "false"};

inline char* json_bool(int i){
    return i ? truefalse[0] : truefalse[1];
}

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
		if(array->type == JSON_CHAR){
            asprintf(&a,fmt,((char*)array->data)[i]);
		}else if(array->type == JSON_DOUBLE){
            asprintf(&a,fmt,((double*)array->data)[i]);
		}else if(array->type == JSON_FLOAT){
            asprintf(&a,fmt,((float*)array->data)[i]);
		}else if(array->type == JSON_INT){
            asprintf(&a,fmt,((int*)array->data)[i]);
		}else if(array->type == JSON_LONG){
            asprintf(&a,fmt,((long*)array->data)[i]);
		}else if(array->type == JSON_STRING){
            asprintf(&a,fmt,((char**)array->data)[i]);
		}else if(array->type == JSON_ARRAY){
            a = json_array(fmt,&((jsonarray_t*)array->data)[i]);
		}else{
            a = "10";
		}
		//asprintf(&a,fmt,(JSON_TYPE_CONVERT(array->type,array->data)[i]));
		ret = merge_delete(ret,a,",");
	}
	char* fret = malloc(sizeof(char)*2); fret[0]=']'; fret[1]='\0';
	ret = merge_delete(ret,fret,"");
	return ret;
}


