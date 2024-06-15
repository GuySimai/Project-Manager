#ifndef WEBSITE_MANAGER_LIST_H
#define WEBSITE_MANAGER_LIST_H


#include "def.h"


// List
typedef struct
{
    NODE head;
}LIST;


BOOL L_init(LIST* pList);
NODE* L_insert(NODE* pNode, DATA Value);
BOOL L_delete(NODE* pNode , void (*freeKey)(void*));
NODE* L_find(NODE* pNode, DATA value, int(*compare)(const void*, const void*));
BOOL L_free(LIST* pList,void (*freeKey)( void*));
int L_print(const LIST* pList , void (*print)(const void*));
int L_printWithNumbers(const LIST* pList , void (*print)(const void*));
int L_count(const LIST* pList);




#endif //WEBSITE_MANAGER_LIST_H
