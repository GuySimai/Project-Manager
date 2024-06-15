#include <string.h>
#include "TeamMember.h"
#include "Task.h"
#include "Macros.h"

//Functions
int initTeamMember(TeamMember* pTeamMember){
    CHECK_RETURN_INT(pTeamMember,"TeamMember was not found");

    pTeamMember->id = teamMemberID++;
    initContact(&pTeamMember->info, "team member");
    pTeamMember->speciality = getSpeciality();
    pTeamMember->experience = getExperience();
    pTeamMember -> numberOfTasks = 0;
    if(!L_init(&pTeamMember -> openTasksList)){
        L_free(&pTeamMember -> openTasksList, freeTask);
        freeContact(&pTeamMember->info);
        return 0;
    }
    if(!L_init(&pTeamMember -> closedTasksList)){
        L_free(&pTeamMember -> openTasksList, freeTask);
        freeContact(&pTeamMember->info);
        return 0;
    }
    return 1;
}

void addToTeamMemberID(int num){
    teamMemberID = teamMemberID + num;
}

void printTeamMember(const void*  teamMember){
    const TeamMember* pTeamMember = *(const TeamMember**) teamMember;

    if(!pTeamMember) {
        printf("TeamMember not found!");
        return;
    }

    printf("Team Member ID: %d,\t",pTeamMember->id );
    printContact(&pTeamMember->info);
    printf("Speciality: %s\t Experience: %d\t Number of tasks: %d\n" ,enumSpeciality[pTeamMember->speciality], pTeamMember->experience, pTeamMember->numberOfTasks);
}

void printTeamMemberTasks(const void*  teamMember){
    const TeamMember* pTeamMember = (const TeamMember*) teamMember;
    if (pTeamMember->numberOfTasks == 0){
        printf("There are no tasks\n");
        return;
    }
        printf("Open Tasks:\n");
        L_printWithNumbers(&pTeamMember->openTasksList, printTask);
        printf("Closed Tasks:\n");
        L_printWithNumbers(&pTeamMember->closedTasksList, printTask);
}


int isTheSameTeamMemberId(TeamMember* TP1,int TP2){
    CHECK_RETURN_INT(TP1,"TeamMember was not found");

    if(TP1->id == TP2) return 1;
    return 0;
}

int switchDepartment(TeamMember* TP1){
    CHECK_RETURN_INT(TP1,"TeamMember was not found");

    printf("You are about to switch department:\n");
    TP1->speciality =getSpeciality();
    return 1;
}

int insertByID(TeamMember* TM, Task* pTask, NODE* node) {

    NODE* pTasksNode = node;

    if(insertByID_TASKS(pTasksNode,pTask) == 0){
        printf("Failed to insert the task into the list.\n");
        return 0;
    }
    TM->numberOfTasks++;
    return 1;
}

int  insertByID_TASKS(NODE* pNode, Task* pTask){
    pNode = L_find(pNode, pTask,compareTaskByID);
    if(!pNode){
        printf("Failed to find the appropriate position to insert the task.\n");
        return 0;
    }
    pNode = L_insert(pNode, pTask);
    if(!pNode)
        return 0;
    return 1;
}


int deleteTaskOfTeamMemberByID(TeamMember* TM, int numIdTask){
    if(TM->numberOfTasks == 0){
        return 0;
    }
    int temp= deleteTaskOfTeamMemberByIDHelper(&TM->openTasksList.head, numIdTask);
    if(temp == 0) {
        temp = deleteTaskOfTeamMemberByIDHelper(&TM->closedTasksList.head, numIdTask);
    }

    if (temp == 1) {
        TM->numberOfTasks--; // Decrease the number of tasks only if a task was successfully deleted
    }

    return temp;
}

int deleteTaskOfTeamMemberByIDHelper(NODE* node, int id) {
    if (!node) {
        return 0; // List is empty
    }

    NODE* foundNode = L_find(node, &id, sameTaskIDByInt);
    if (foundNode != NULL && foundNode->next != NULL) {
        L_delete(foundNode, freeTask);
        return 1;
    }

    return 0;
}

NODE* changeTaskStatusOfTeamMemberByID(TeamMember* TM, int numIdTask, int status){
    if(TM->numberOfTasks == 0){
        return 0;
    }
    NODE* temp= changeTaskStatusOfTeamMemberByIDHelper(&TM->openTasksList.head, numIdTask, status);
    if(temp == NULL) {
        temp = changeTaskStatusOfTeamMemberByIDHelper(&TM->closedTasksList.head, numIdTask, status);
    }

    return temp;
}

NODE* changeTaskStatusOfTeamMemberByIDHelper(NODE* node, int id, int status){
    if(!node)
        return NULL;

    NODE* pNodePM = node;
    pNodePM = L_find(pNodePM, &id, sameTaskIDByInt);
    if (pNodePM->next != NULL) {
        changeStatus(pNodePM->next->key,status);
        return pNodePM->next;
    }
    return NULL;
}

int sameTeamMemberID(const void* TM1,const void* TM2){

    const TeamMember * pT1 = (const TeamMember*)TM1;
    const TeamMember* pT2 = (const TeamMember*)TM2;

    if (pT1->id == pT2->id){
        return 0;
    } else return -1;
}

int turnToTeamMember(TeamMember* pTeamMember , Candidate* pCandidate){
    CHECK_RETURN_INT(pTeamMember,"TeamMember was not found")

    pTeamMember->id = teamMemberID++;

    pTeamMember->info.name = _strdup(pCandidate->info.name);
    pTeamMember->info.email = _strdup(pCandidate->info.email);
    strcpy(pTeamMember->info.phoneNumber,pCandidate->info.phoneNumber);

    pTeamMember->speciality = pCandidate->speciality;
    pTeamMember->experience = pCandidate->experience;
    pTeamMember -> numberOfTasks = 0;

    if(!L_init(&pTeamMember -> openTasksList)){
        L_free(&pTeamMember -> openTasksList, freeTask);
        freeContact(&pTeamMember->info);
        return 0;
    }
    if(!L_init(&pTeamMember -> closedTasksList)){
        L_free(&pTeamMember -> openTasksList, freeTask);
        freeContact(&pTeamMember->info);
        return 0;
    }
    return 1;
}

void freeTeamMember(TeamMember* TP1){
    freeContact(&TP1->info);
    L_free(&TP1->closedTasksList, freeTask);
    L_free(&TP1->openTasksList, freeTask);
}
