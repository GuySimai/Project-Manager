#ifndef WEBSITE_MANAGER_TASK_H
#define WEBSITE_MANAGER_TASK_H

//Imports
#include <stdlib.h>
#include "Date.h"
#include "General.h"


// Forward declaration of struct TeamMember
struct TeamMember;

//ID COUNTER
static int taskID = 1;

//-------Task Struct-----
typedef struct{
    int id;
    char* taskStr;
    struct TeamMember* teamMember;
    eStatus taskStatus;

} Task;
//-----------------------------

//Functions
int     initTask(Task* pTask, struct TeamMember* teamMember);
void    addToTaskID(int num);
void    printTask(const void* pTask);
void    freeTask(void* pTask);
int     changeTaskStatus(Task* t1);

//Helper Functions
int     changeStatus(Task* t1, int option);
int     compareTaskByID(const void* T1, const void* T2);
int     sameTaskID(const void* T1, const void* T2);
int     returnTeamMemberID(const void* T1);
int     returnIdTask(const void* T1);
int     getTaskStatus(const void* T1);
int     sameTaskIDByInt(const void* T1,const void* id);

#endif //WEBSITE_MANAGER_TASK_H
