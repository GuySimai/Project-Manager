#ifndef WEBSITE_MANAGER_PROJECTMANAGEMENT_H
#define WEBSITE_MANAGER_PROJECTMANAGEMENT_H

//Imports
#include "Project.h"
#include "TeamMember.h"
#include "List.h"
#include "Payment.h"

#define EXIT			(-1)
#define MAXTASKS 15

//---Project Struct----
typedef struct
{
    Project project;
    int numberOfTeamMembers;
    TeamMember** projectTeamMembersArray;
    int numberOfTasks;
    LIST projectManagementTasksLists;
    Payment payment;
}ProjectManagement;

//---------------------

//Functions
int     initProjectManagement(ProjectManagement* PM,Client* client);
void    printProjectManagement(const void* projectManagement);
void    printProjectManagementShort(const void* projectManagement);
int     addTeamMemberToProjectManagement(ProjectManagement* PM , TeamMember* TM);
int     removeTeamMemberFromProjectManagement(ProjectManagement* PM);
int     removeTeamMemberFromProjectManagementHelper(ProjectManagement* PM, int num);
int     removeAllTasksOfTheProjectManagementTeamMemberList(ProjectManagement* PM, NODE * headListTeamMemberTask);
int     addTask(ProjectManagement* PM);
int     addTaskToProManList(ProjectManagement* PM, Task* task);
void    createCopyTask(Task* taskCopy,const Task* task);
int     deleteTask(ProjectManagement* PM);
int     chooseTask(ProjectManagement* PM);
void    projectManagementMenu(ProjectManagement* PM);
int     projectManagementMenuHelper();
void    printTeamMemberTasksYouChoose(const ProjectManagement* PM);
void    freeProjectManagement(ProjectManagement* PM);

//Helper Functions
int         deleteTeamMemberInArray(ProjectManagement* PM, int num);
int         printProjectTeamMembersArray(const ProjectManagement* PM);
TeamMember* getTeamMember(TeamMember** teamMembersArray,int);
TeamMember* getTeamMemberByType(TeamMember** teamMembersArray,int,int);
void        printPMTasksList(const ProjectManagement* PM);
int         removeAllTasksOfTheProjectManagement(ProjectManagement* PM, TeamMember* teamMember);
int         removeTaskOfTheProjectManagement(ProjectManagement* PM, int skip);
void        findTeamMemberWithTheSameIdAndDeleteTask(ProjectManagement* PM, int numIdTeamMember, int numIdTask);
TeamMember* findTeamMemberWithTheSameId(const ProjectManagement* PM, int numIdTeamMember);
int         changeTaskStatusProjectManagement(ProjectManagement* PM);
int         switchLists(TeamMember* TM,NODE* pTemp, NODE* headListFrom, NODE* headListTo);
int         changeTaskStatusProjectManagementHelper(ProjectManagement* PM, int statusBefore, int newStatus, NODE* pNode);
int         changeProjectStatus(ProjectManagement* PM);
#endif //WEBSITE_MANAGER_PROJECTMANAGEMENT_H
