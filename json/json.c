#include "json.h"
#include "../asprintf.c/asprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn/jsmn.h"
#include <assert.h>
#include <float.h>
#include <limits.h>

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
	            asprintf(&a,"\"%s\"",((char**)array->data)[i]);
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
				asprintf(&a,"\"%s\"",itm->object.s);
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

static char* strcp(char* a, size_t size){
	char* ret = malloc(sizeof(char)*(size+1));
	memcpy(ret, a, size);
	ret[size]='\0';
	return ret;
}

static int dump(char* js, jsmntok_t* t, size_t count, jsontype_t* type, jsonitem_t* itm){
	int i, j;
	if(count == 0){
		return 0;
	}
	if(t->type == JSMN_PRIMITIVE){
		size_t len = t->end - t->start;
		char* tmp = strcp(js+t->start, len);
		//see if its just a char
		if(len == 1){
			itm->c = *(js+t->start);
			*type = JSON_CHAR;
			free(tmp);
			return 1;
		}
		//bool test:
		if(strstr(tmp, "true") != NULL){
			*type = JSON_BOOL;
			itm->b = 1;
			free(tmp);
			return 1;
		}else if(strstr(tmp, "false") != NULL){
			*type = JSON_BOOL;
			itm->b = 0;
			free(tmp);
			return 1;
		}
		//float test
		if(strstr(tmp,".") != NULL){
			double val = atof(tmp);
			if((val > FLT_MAX || val < FLT_MAX*-1) || (val < FLT_MIN || val > FLT_MIN*-1)){
				*type = JSON_DOUBLE;
				itm->d = val;
			}else{
				*type = JSON_FLOAT;
				itm->f = (float)val;
			}
			free(tmp);
			return 1;
		}else{//must be a number
			long val = atol(tmp);
			if(val > INT_MAX || val < INT_MIN){
				*type = JSON_LONG;
				itm->l = val;
			}else{
				*type = JSON_INT;
				itm->i = (int)val;
			}
			free(tmp);
			return 1;
		}
		//should never reach this point, but just incase...
		free(tmp);
		return 1;
	} else if(t->type == JSMN_STRING){
		*type = JSON_STRING;
		itm->s = strcp(js+t->start, t->end - t->start);
		return 1;

	} else if(t->type == JSMN_OBJECT){
		*type = JSON_OBJECT;
		jsonobject_t* obj = json_newobj();
		itm->o=obj;
		j=0;
		for(i=0; i< t->size; i++){
			jsontype_t subt;
			jsonitem_t name;
			jsonitem_t subi;
			j += dump(js, t+1+j, count-j,&subt, &name);
			j += dump(js, t+1+j, count-j,&subt, &subi);
			json_put(obj, name.s, subi, subt);
		}
		return j+1;
		
	}else if(t->type == JSMN_ARRAY){
		j=0;
		jsonarray_t* arr = malloc(sizeof(jsonarray_t));
		arr->length = t->size;
		size_t len = t->size;
		*type=JSON_ARRAY;
		itm->a=arr;
		if(len == 0){
			return 1;
		}
		jsontype_t subt;
		jsonitem_t subi;
		j+=dump(js,t+1+j, count-j, &subt, &subi);
		arr->type = subt;
		switch(subt){
			case JSON_DOUBLE:
				arr->data = malloc(sizeof(double)*len);
				((double*)arr->data)[0]=subi.d;
				break;
			case JSON_FLOAT:
				arr->data = malloc(sizeof(float)*len);
				((float*)arr->data)[0]=subi.f;
				break;
			case JSON_INT:
				arr->data = malloc(sizeof(int)*len);
				((int*)arr->data)[0]=subi.i;
				break;
			case JSON_CHAR:
				arr->data = malloc(sizeof(char)*len);
				((char*)arr->data)[0]=subi.c;
				break;
			case JSON_BOOL:
				arr->data = malloc(sizeof(int)*len);
				((int*)arr->data)[0]=subi.b;
				break;
			case JSON_LONG:
				arr->data = malloc(sizeof(long)*len);
				((long*)arr->data)[0]=subi.l;
				break;
			case JSON_STRING:
				arr->data = malloc(sizeof(char*)*len);
				((char**)arr->data)[0]=subi.s;
				break;
			case JSON_ARRAY:
				arr->data = malloc(sizeof(jsonarray_t*)*len);
				((jsonarray_t**)arr->data)[0]=subi.a;
				break;
			case JSON_OBJECT:
				arr->data = malloc(sizeof(jsonobject_t*)*len);
				((jsonobject_t**)arr->data)[0]=subi.o;
				break;
			default:
				arr->data = NULL;
				break;
		}
		for(i=1; i<t->size; i++){
			jsontype_t st;
			jsonitem_t si;
			j+=dump(js,t+1+j,count-j,&st,&si);
			switch(st){
				case JSON_DOUBLE:
					((double*)arr->data)[i]=si.d;
					break;
				case JSON_FLOAT:
					((float*)arr->data)[i]=si.f;
					break;
				case JSON_INT:
					((int*)arr->data)[i]=si.i;
					break;
				case JSON_CHAR:
					((char*)arr->data)[i]=si.c;
					break;
				case JSON_BOOL:
					((int*)arr->data)[i]=si.b;
					break;
				case JSON_LONG:
					((long*)arr->data)[i]=si.l;
					break;
				case JSON_STRING:
					((char**)arr->data)[i]=si.s;
					break;
				case JSON_ARRAY:
					((jsonarray_t**)arr->data)[i]=si.a;
					break;
				case JSON_OBJECT:
					((jsonobject_t**)arr->data)[i]=si.o;
					break;
				default:
					arr->data = NULL;
					break;
			}
		}
		return j+1;
	}
	return 0;
}

jsonitem_t json_fromstring(char* a, jsontype_t* type){
	jsonitem_t ret;
	jsmn_parser p;
	jsmntok_t* t;
	size_t size = 128;
	int r;
	t = malloc(sizeof(jsmntok_t)*size);
	
	//if 128 isn't enough space, then we'll just realloc till we're good
	jsmn_init(&p);
	r = jsmn_parse(&p, a, strlen(a), t, size);
	while(r == JSMN_ERROR_NOMEM){
		free(t);
		size*=2;
		t = malloc(sizeof(jsmntok_t)*size);
		assert(t!=NULL); //otherwise, what the hell?
		r = jsmn_parse(&p, a, strlen(a), t, size);
	}
	assert(t[0].type == JSMN_OBJECT || t[0].type == JSMN_ARRAY);
	dump(a, t, p.toknext,type,&ret);
	return ret;
}

