#include <stdio.h>
#include <string.h>
#include "Client.h"
#include "Macros.h"

//Functions
int initClient(Client* pClient){
    CHECK_RETURN_INT(pClient,"Client was not found!");
    pClient->id = clientID++;
    if(!initContact(&pClient->info, "client")){
        return 0;
    }
    return 1;
}

void addToClientID(int num){
    clientID = clientID + num;
}


void    printClient(const void* client){
    const Client* pClient = (const Client*) client;

    if(!pClient) {
        printf("Client not found!");
        return;
    }
    printf("Client ID : %d\t ",pClient->id );
    printContact(&pClient->info);
    printf("\n");
}

int compareClientsByID(const void* C1,const void* C2){

    const Client* pC1 = (const Client*)C1;
    const Client* pC2 = (const Client*)C2;

    return MINUS(pC1->id, pC2->id);

}

int compareClientsByName(const void* C1,const void* C2){

    const Client* pC1 = (const Client*)C1;
    const Client* pC2 = (const Client*)C2;
    
    return strcicmp(pC1->info.name,pC2->info.name);
}

int compareClientsByPhoneNumber(const void* C1,const void* C2){
    const Client* pC1 = (const Client*)C1;
    const Client* pC2 = (const Client*)C2;

    return strcmp(pC1->info.phoneNumber, pC2->info.phoneNumber);
}


void    freeClient(Client* pClient){
    freeContact(&pClient->info);
}


