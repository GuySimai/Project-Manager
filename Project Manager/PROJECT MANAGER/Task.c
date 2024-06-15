#include <stdio.h>
#include <stdlib.h>
#include "Task.h"
#include "TeamMember.h"
#include "Macros.h"

//Functions
int         initTask(Task* pTask , struct TeamMember* teamMember){
    CHECK_RETURN_INT(pTask,"Task was not found");
    pTask->id = taskID++;
    pTask->taskStr = getStrExactName("Add a task: ");
    pTask->taskStatus = eWaiting;
    pTask->teamMember = teamMember;

    return 1;
}

void addToTaskID(int num){
    taskID = taskID + num;
}

void        printTask(const void* pTask){

    Task* t1 = (Task*)pTask;
    if(!t1) {
        printf("Task not found!\n");
        return;
    }
    TeamMember* teamMember = (TeamMember*)t1->teamMember;
    printf("In charge: %s, ID: %d\n" , teamMember->info.name, t1->id);
    printf("The task: %s \n",t1->taskStr);
    printf("Task Status: %s\n\n", enumStatus[t1->taskStatus]);

}

int         isTheSameTaskId(Task* t1,Task* t2){
    CHECK_RETURN_INT(t1,"Task was not found");
    CHECK_RETURN_INT(t2,"Task was not found");

    if (t1->id == t2->id)
        return 1;

    return 0;
}

int         changeTaskStatus(Task* t1){
    CHECK_RETURN_INT(t1,"Task was not found");

    int option;
    printf("\n\n");
    do{
        printf("What is the status of the task?\n");
        for (int i = 0; i < eStatusNofOption; i++)
            printf("%d for %s\n", i, enumStatus[i]);
        scanf("%d", &option);
    }while(option < 0 || option > eStatusNofOption && option != t1->taskStatus);

    getchar();
    t1->taskStatus = option;

    return option;
}

void freeTask(void* pTask){
    Task* t1 = (Task*)pTask;
    free(t1->taskStr);
}

//Helper Functions


int   editTask(Task* t1){
    CHECK_RETURN_INT(t1,"Task was not found");

    t1->taskStr = getStrExactName("Add a new task: ");
    return 1;
}

int changeStatus(Task* t1, int option){
    CHECK_RETURN_INT(t1,"Task was not found");
    t1->taskStatus = option;
    return 1;
}

int compareTaskByID(const void* T1,const void* T2){

    const Task * pT1 = (const Task*)T1;
    const Task* pT2 = (const Task*)T2;

    if (MINUS(pT1->id,  pT2->id) > 0){
        return 0;
    } else return -1;
}

int sameTaskID(const void* T1,const void* T2){

    const Task * pT1 = (const Task*)T1;
    const Task* pT2 = (const Task*)T2;

    if (pT1->id == pT2->id){
        return 0;
    } else return -1;
}

int sameTaskIDByInt(const void* T1,const void* id){
    const Task * pT1 = (const Task*)T1;
    const int * pT2 = (const int*)id;

    if (pT1->id == *pT2){
        return 0;
    } else return -1;
}

int returnTeamMemberID(const void* T1){
    const Task* task = (const Task*)T1;
    TeamMember* teamMember = (TeamMember*)task->teamMember;
    return teamMember->id;
}

int returnIdTask(const void* T1){
    const Task * pT1 = (const Task*)T1;
    return pT1->id;
}

int getTaskStatus(const void* T1){
    const Task * pT1 = (const Task*)T1;
    return pT1->taskStatus;
}

