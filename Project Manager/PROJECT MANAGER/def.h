#ifndef _DEF_
#define _DEF_


typedef void* DATA;
typedef enum {False, True} BOOL;


typedef struct node
{
    DATA			key;
    struct node*	next;
}NODE;

#endif
