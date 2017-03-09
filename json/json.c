#include "json.h"
#include "../asprintf.c/asprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//source found at:http://www.cse.yorku.ca/~oz/hash.html 
static size_t djb2(const void *key, int l, uint32_t seed){
	size_t hash = 5381;
	char* str = (char*)key;
	int c;
	while((c = *str++)){
		hash = (hash << 5) + hash + c;
	}
	return hash%l;
}

static HashTableConf* conf = NULL;

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
	if(fmt == NULL){
		for(i=0; i<len; ++i){
			char* a;
			if(array->type == JSON_CHAR){
	            asprintf(&a,"%c",((char*)array->data)[i]);
			}else if(array->type == JSON_DOUBLE){
	            asprintf(&a,"%lf",((double*)array->data)[i]);
			}else if(array->type == JSON_FLOAT){
	            asprintf(&a,"%f",((float*)array->data)[i]);
			}else if(array->type == JSON_INT){
	            asprintf(&a,"%i",((int*)array->data)[i]);
			}else if(array->type == JSON_LONG){
	            asprintf(&a,"%li",((long*)array->data)[i]);
			}else if(array->type == JSON_STRING){
	            asprintf(&a,"%s",((char**)array->data)[i]);
			}else if(array->type == JSON_ARRAY){
	            a = json_array(fmt,&((jsonarray_t*)array->data)[i]);
			}else if(array->type == JSON_OBJECT){
	            a = json_tostring(&((jsonobject_t*)array->data)[i]);
			}else{
	            a = "10";
			}
			//asprintf(&a,fmt,(JSON_TYPE_CONVERT(array->type,array->data)[i]));
			if(i==0) ret = merge_delete(ret,a,"");
			else ret = merge_delete(ret,a,",");
		}

	}else{
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
			}else if(array->type == JSON_OBJECT){
	            a = json_tostring(&((jsonobject_t*)array->data)[i]);
			}else{
	            a = "10";
			}
			//asprintf(&a,fmt,(JSON_TYPE_CONVERT(array->type,array->data)[i]));
			if(i==0) ret = merge_delete(ret,a,"");
			else ret = merge_delete(ret,a,",");
		}
	}
	char* fret = malloc(sizeof(char)*2); fret[0]=']'; fret[1]='\0';
	ret = merge_delete(ret,fret,"");
	return ret;
}


jsonobject_t* json_newobj(){
	if (conf == NULL){
		conf = malloc(sizeof(HashTableConf));
		hashtable_conf_init(conf);
		conf->initial_capacity=100;
		conf->hash = djb2;
		conf->key_length= -1;
		conf->load_factor = 0.67f;
	}
	jsonobject_t* obj = malloc(sizeof(jsonobject_t));
	hashtable_new_conf(conf,&obj->data);
	return obj;
}


char* json_tostring(jsonobject_t* object){
	char* ret = malloc(sizeof(char)*2);
	ret[0]='{'; ret[1]='\0';
	HashTableIter iter;
	hashtable_iter_init(&iter, object->data);
	TableEntry* entry;
	int i=0;
	while(hashtable_iter_next(&iter,&entry) != CC_ITER_END){
		char* name = (char*)entry->key;
		struct json_object_item* itm = (struct json_object_item*)entry->value;
		char* a;
		char* nm;
		asprintf(&nm,"\"%s\"",name);
		switch(itm->type){
			case JSON_DOUBLE:
				asprintf(&a,"%lf",itm->object.d);
				break;
			case JSON_FLOAT:
				asprintf(&a, "%f",itm->object.f);
				break;
			case JSON_INT:
				asprintf(&a, "%i",itm->object.i);
				break;
			case JSON_CHAR:
				asprintf(&a,"%c",itm->object.c);
				break;
			case JSON_BOOL:
				asprintf(&a,"%s",json_bool(itm->object.b));
				break;
			case JSON_LONG:
				asprintf(&a,"%li",itm->object.l);
				break;
			case JSON_STRING:
				asprintf(&a,"%s",itm->object.s);
				break;
			case JSON_ARRAY:
				a = json_array(NULL,itm->object.a);
				break;
			case JSON_OBJECT:
				a = json_tostring(itm->object.o);
				break;
			default:
				a = "10";
				break;
		}
		a = merge_delete(nm,a,":");
		if(i==0) ret = merge_delete(ret,a,"");
		else ret = merge_delete(ret,a,",");
		i++;
	}
	char* cap = malloc(sizeof(char)*2);
	cap[0] = '}'; cap[1]='\0';
	ret = merge_delete(ret,cap,"");
	return ret;
}
//Takes the object to put the item in, the name of the item, a void* space to put the item, and the type of the item
void json_put(jsonobject_t* obj, char* name, jsonitem_t val, jsontype_t type){
	struct json_object_item* itm = malloc(sizeof(struct json_object_item));
	itm->object = val;
	itm->type = type;
	hashtable_add(obj->data, (void*)name, (void*)itm);
}
//Takes the object to search, the name of the item, and a pointer to a type object to put the type of object returned, and returns a void* of the item
jsonitem_t json_get(jsonobject_t* obj, char* name, jsontype_t* type){
	struct json_object_item* item;
	hashtable_get(obj->data, name, (void**)&item);
	*type = item->type;
	return item->object;
}

