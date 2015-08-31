#include "../items/item.h"
#include "../GenericList.h"

typedef struct quest_task{
	char* name;
	List(string)* desc;
}quest_task_t;

define_list(quest_task_t)

typedef struct quest{
	char* title;
	List(string)* desc;
	List(item_t)* rewards;
	List(quest_task_t)* tasks;
}quest_t;
