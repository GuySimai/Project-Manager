#ifndef WEBSITE_MANAGER_PROJECT_H
#define WEBSITE_MANAGER_PROJECT_H

//Imports
#include "Client.h"
#include "Date.h"
#include "Feedback.h"

//---Project Struct----
typedef struct
{
    char* name;
    bool zoomMeeting;
    char** websiteReq;
    int reqNumber;
    Client client;
    Date dueDate;
    eStatus projectStatus;
    Feedback* feedback;
}Project;

//---------------------

//Functions
int     initProject(Project* pProject, Client* client);
void    printProject(const void* project);
int     getClientWebsiteRequirements(Project* pProject);
int     writeWebsiteReq(Project* pProject);
void    isZoomMeeting(Project* pProject);
void    printWebsiteRequirements(const Project* pProject);
void    freeProject(Project* pProject);

//Helper Functions
int     addFeedback(Project* pProject);
void	freeWebsiteReqArr(char** arr, int size);
void    zoomMeetingOccurred(Project* pProject);

#endif //WEBSITE_MANAGER_PROJECT_H
