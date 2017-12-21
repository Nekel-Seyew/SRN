#ifndef SRN_JSON_WRITER_H
#define SRN_JSON_WRITER_H

#include <stdlib.h>
#include "../Collections-C/src/hashtable.h"

typedef enum{
	JSON_DOUBLE = 0,
	JSON_FLOAT  = 1,
	JSON_INT    = 2,
	JSON_CHAR   = 3,
	JSON_BOOL   = 5,
	JSON_LONG   = 6,
	JSON_STRING = 7,
	JSON_ARRAY  = 8,
	JSON_OBJECT = 9,
} jsontype_t;

union json_data;

typedef struct json_array_type{//only c-like arrays: uniform data type
	jsontype_t type;
	void* data;
	size_t length;
} jsonarray_t;

typedef struct json_object{
    HashTable* data;
    //size_t length;
    //struct json_object_item* data;
    //char** item_names;
} jsonobject_t;

typedef union jsonitem jsonitem_t;

union jsonitem{
	char c;
	int i;
	int b;
	float f;
	double d;
	long l;
	char* s;
	jsonarray_t* a;
	jsonobject_t* o;
};

struct json_object_item{
    jsontype_t type;
    jsonitem_t object;
};



jsonobject_t* json_newobj();

//takes in two strings, merges them together with the joiner string seperating
//them, deletes the two passed in strings, and returns the resulting string.
char* merge_delete(char* a, char* b, char* joiner);
//does the same as merge_delete, but doesn't delete the two passed in strings
char* merge_string(char* a, char* b, char* joiner);

char* json_array(const char* fmt, jsonarray_t* array);
char* json_bool(int i);
char* json_tostring(jsonobject_t* object);

//num == -1 means the user has no idea how many tokens are in the json string.
//By default, json will assume no more than 1024
jsonitem_t json_fromstring(char* a, jsontype_t* type);

//Takes the objeict to put the item in, the name of the item, a void* space to put the item, and the type of the item. All input should be pointers to actual data location
void  json_put(jsonobject_t* obj, char* name, jsonitem_t val, jsontype_t type);
//Takes the object to search, the name of the item, and a pointer to a type object to put the type of object returned, and returns a void* of the item, all return types will be pointers of some kind
jsonitem_t json_get(jsonobject_t* obj, char* name, jsontype_t* type);


#endif
