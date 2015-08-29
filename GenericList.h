/*

important notes:

the int (*cmp)(const void*,const void*) will be passed type* items.

*/
#include <string.h>
#include <stdlib.h>

#define define_list(type) \
\
    struct _list_##type; \
    \
    typedef struct \
    { \
        int (*is_empty)(const struct _list_##type*); \
        size_t (*size)(const struct _list_##type*); \
		void (*add)(struct _list_##type*, type);\
		void (*remove)(struct _list_##type*,type,int (*cmp)(const void*,const void*));\
    } _list_functions_##type; \
    \
    typedef struct _list_##type \
    { \
        size_t _size; \
		size_t _length;\
        type* _data; \
        _list_functions_##type* _functions; \
    } List_##type; \
    \
    List_##type* new_list_##type(); \
    int list_is_empty_##type(const List_##type* list); \
    size_t list_size_##type(const List_##type* list); \
	void list_add_##type(List_##type* list, type elem);\
	void list_remove_##type(List_##type* list, type elem,int (*cmp)(const void*,const void*));\
    \
    int list_is_empty_##type(const List_##type* list) \
    { \
        return list->_size == 0; \
    } \
    \
    size_t list_size_##type(const List_##type* list) \
    { \
        return list->_size; \
    } \
    \
    void list_add_##type(List_##type* list, type elem) \
    { \
        if(list->_size +1 >= list->_length){ \
			type* newData = (type*) malloc(sizeof(type) * 2 * list->_length); \
			memcpy((void*)newData, (const void*)list->_data, sizeof(type) * list->_length); \
			list->_length *= 2; \
			free(list->_data); \
			list->_data = newData; \
		} \
		list->_data[list->_size] = elem; \
		list->_size += 1; \
    } \
	void list_remove_##type(List_##type* list, type elem, int (*cmp)(const void*,const void*)){ \
		size_t i=0;\
		for(i=0; i<list->_size; ++i){ \
			if(cmp(&elem,&(list->_data[i])) == 0){ \
				size_t j; \
				for(j=i; j<list->_size; ++j){ \
					if(j+1 >= list->_size){ \
						list->_data[j] = (type)NULL; \
						list->_size -= 1; \
						return; \
					} \
					list->_data[j] = list->_data[j+1]; \
					list->_size -= 1; \
					return; \
				} \
			}\
		}\
	} \
    \
    _list_functions_##type _list_funcs_##type = { \
        &list_is_empty_##type, \
        &list_size_##type, \
		&list_add_##type, \
		&list_remove_##type \
    }; \
    \
    List_##type* new_list_##type() \
    { \
        List_##type* res = (List_##type*) malloc(sizeof(List_##type)); \
        res->_size = 0; \
		res->_length = 10;\
        res->_data = (type*) malloc(sizeof(type)*10); \
        res->_functions = &_list_funcs_##type; \
        return res; \
    }
	
#define list_is_empty(list) \
	list->_functions->is_empty(list)
#define list_size(list) \
	list->_functions->size(list)
#define list_add(list, item) \
	list->_functions->add(list,item)
#define list_remove(list,item) \
	list->_functions->remove(list,item)
	
#define List(type) \
    List_##type

#define new_list(type) \
    new_list_##type()
