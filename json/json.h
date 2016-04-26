#ifndef SRN_JSON_WRITER_H
#define SRN_JSON_WRITER_H

#include <stdlib.h>

#define JSON_TYPE_CONVERT(t,x) (t==JSON_DOUBLE ? ((double*)x) : \
		               (t==JSON_FLOAT  ? ((float*)x)  : \
			       (t==JSON_INT    ? ((int*)x)    : \
			       (t==JSON_CHAR   ? ((char*)x)   : \
			       (t==JSON_LONG   ? ((long*)x)   : \
			       (t==JSON_STRING ? ((char**)x)  : \
			       (0)))))))

typedef enum{
	JSON_DOUBLE = 0,
	JSON_FLOAT  = 1,
	JSON_INT    = 2,
	JSON_CHAR   = 3,
	JSON_LONG   = 4,
	JSON_STRING = 5
} jsontype_t;

typedef struct json_array_type{
	void* data;
	size_t length;
	jsontype_t type;
} jsonarray_t;

//takes in two strings, merges them together with the joiner string seperating
//them, deletes the two passed in strings, and returns the resulting string.
char* merge_delete(char* a, char* b, char* joiner);
char* merge_string(char* a, char* b, char* joiner);

char* json_array(const char* fmt, jsonarray_t* array);

#endif