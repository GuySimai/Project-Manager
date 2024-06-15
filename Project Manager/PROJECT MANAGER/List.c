#include <stdio.h>
#include <stdlib.h>
#include "List.h"


BOOL L_init(LIST* pList){
    if ( pList == NULL ) return False;	// no list to initialize

    pList->head.next = NULL;
    return True;
}

NODE* L_insert(NODE* pNode, DATA Value)
{
    NODE* tmp;

    if ( !pNode ) return NULL;

    tmp = (NODE*)malloc(sizeof(NODE));	// new node

    if ( tmp != NULL )  {
        tmp->key = Value;
        tmp->next = pNode->next;
        pNode->next = tmp;
    }
    return tmp;
}

BOOL L_delete(NODE* pNode , void (*freeKey)(void*))
{
    NODE* tmp;

    if (!pNode)
        return False;

    tmp = pNode->next;
    if(!tmp)
        return False;

    pNode->next = tmp->next;
    if(freeKey)
        freeKey(tmp->key);
    free(tmp);

    return True;
}

NODE* L_find(NODE* pNode, DATA value, int(*compare)(const void*, const void*))
{
    NODE* temp = NULL;
    if (!pNode) return NULL;
    while(pNode!= NULL)
    {
        if(pNode->next == NULL){
            temp = pNode;
            break;
        }
        if(compare(pNode->next->key,value) == 0)
        {
            temp = pNode;
            break;
        }
        pNode = pNode->next;
    }

    return temp;


}


BOOL L_free(LIST* pList,void (*freeKey)(void*))
{
    NODE *tmp;
    BOOL cont = True;
    if ( !pList )
        return False;

    tmp = &(pList->head);
    while (cont)
        cont = L_delete(tmp,freeKey);

    return True;
}

int L_print(const LIST* pList , void (*print)(const void*))
{
    NODE	*tmp;
    int		c = 0;

    if ( !pList )
        return False;

    printf("\n");
    tmp = pList->head.next;
    while(tmp) // If not wroking change to tmp != NULL
    {
        print(tmp->key);
        c++;
        tmp = tmp->next;
    }
    printf("\n");
    return c;
}

int L_printWithNumbers(const LIST* pList , void (*print)(const void*))
{
    NODE	*tmp;
    int		c = 1;

    if ( !pList )
        return False;

    printf("\n");
    tmp = pList->head.next;
    while(tmp) // If not wroking change to tmp != NULL
    {
        printf ("%d)", c);
        c++;
        print(tmp->key);
        tmp = tmp->next;
    }
    printf("\n");
    return c;
}

int L_count(const LIST* pList)
{
    NODE	*tmp;
    int		c = 0;

    if ( !pList )
        return 0;

    tmp = pList->head.next;
    while(tmp)
    {
        c++;
        tmp = tmp->next;
    }
    return c;
}