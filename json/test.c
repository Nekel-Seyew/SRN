#include <stdlib.h>
#include <stdio.h>
#include "json.h"
#include <string.h>

int main(){
	jsonarray_t* array = malloc(sizeof(jsonarray_t));
	int* data = malloc(sizeof(int)*10);
	int i=0;
	for(i=0; i<10; ++i){
		data[i]=(i+13)*4;
	}
	array->data = (void*)data;
	array->type = JSON_INT;
	array->length = 10;

	char* jsonify = json_array("%010i",array);
	printf("%s\n",jsonify);

	return 0;
}
