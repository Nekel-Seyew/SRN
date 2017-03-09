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
	
	jsonobject_t* obj = json_newobj();
	jsonitem_t a; a.s = "SUP!";
	jsonitem_t b; b.d = 1.2334;
	jsonitem_t c; c.l = 100<<20;
	jsonitem_t r; r.a = array;
	
	json_put(obj,"a",a,JSON_STRING);
	json_put(obj,"b",b,JSON_DOUBLE);
	json_put(obj,"c",c,JSON_LONG);
	json_put(obj,"array",r,JSON_ARRAY);
	
	jsontype_t type;	

	char* jsonify2 = json_tostring(obj);
	printf("%s\n",jsonify2);
	printf("String: %s\n",json_get(obj,"a",&type).s);
	printf("Double: %lf\n",json_get(obj,"b",&type).d);
	printf("Long: %li\n",json_get(obj,"c",&type).l);
	printf("array: %s\n",json_array(NULL,json_get(obj,"array",&type).a));

	return 0;
}
