#ifndef WEBSITE_MANAGER_CLIENT_H
#define WEBSITE_MANAGER_CLIENT_H

//Imports


#include <stdbool.h>
#include "General.h"
#include "Contact.h"

//ID COUNTER
static int clientID = 1;


//-------Client Struct-----
typedef struct
{
    int id;
    Contact info;
}Client;
//-----------------------------

//Functions
int     initClient(Client* pClient);
void    addToClientID(int num);
void    printClient(const void* client);
int     compareClientsByID(const void* C1,const void* C2);
int     compareClientsByName(const void* C1,const void* C2);
int     compareClientsByPhoneNumber(const void* C1,const void* C2);
void    freeClient(Client* pClient);

//Helper Function


#endif //WEBSITE_MANAGER_CLIENT_H
