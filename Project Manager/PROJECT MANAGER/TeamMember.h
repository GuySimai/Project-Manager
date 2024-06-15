#ifndef WEBSITE_MANAGER_TEAMMEMBER_H
#define WEBSITE_MANAGER_TEAMMEMBER_H

//Imports
#include <stdio.h>
#include <stdlib.h>

#include "Contact.h"
#include "General.h"
#include "List.h"
#include "Task.h"
#include "Candidate.h"

//ID COUNTER
static int teamMemberID = 1;

typedef struct
{
    int id;
    int experience;
    Contact info;
    eSpecialization speciality;
    int numberOfTasks;
    LIST openTasksList;
    LIST closedTasksList;

} TeamMember;

//---------------------

//Functions
int     initTeamMember(TeamMember* pTeamMember);
void    addToTeamMemberID(int num);
void    printTeamMember(const void*  teamMember);
void    printTeamMemberTasks(const void*  teamMember);
int     insertByID(TeamMember* TM, Task* pTask, NODE* node); // Forward declaration of struct Task
int     deleteTaskOfTeamMemberByID(TeamMember* TM, int numIdTask);
void    freeTeamMember(TeamMember* TP1);
int     insertByID_TASKS(NODE* pNode, Task* pAir);
NODE*   changeTaskStatusOfTeamMemberByID(TeamMember* TM, int numIdTask, int status);
NODE*   changeTaskStatusOfTeamMemberByIDHelper(NODE* node, int id, int status);
int     turnToTeamMember(TeamMember* pTeamMember , Candidate* pCandidate);
int     sameTeamMemberID(const void* TM1,const void* TM2);

//Helper Functions
int     isTheSameTeamMemberId(TeamMember* TP1, int TP2);
int     switchDepartment(TeamMember* TP1);
int     deleteTaskOfTeamMemberByIDHelper(NODE* headList, int id);


#endif //WEBSITE_MANAGER_TEAMMEMBER_H
