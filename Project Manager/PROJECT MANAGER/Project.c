#include <stdio.h>
#include <stdlib.h>
#include "Project.h"
#include "Macros.h"

#define MIN 3
#define MAX 10

//Functions
int  initProject(Project* pProject, Client* client){
    CHECK_RETURN_INT(pProject,"Project was not found!");
    CHECK_RETURN_INT(client,"Client was not found!");
    isZoomMeeting(pProject);
    if(pProject->zoomMeeting == false){
        return -1;
    }
    pProject->name = getStrExactName("Enter project name:");
    if(!pProject->name){
        return 0;
    }
    CHECK_RETURN_INT(pProject->name,"Memory allocation problem!");
    if(!getClientWebsiteRequirements(pProject)){
        free(pProject->name);
        return 0;
    }
    pProject->client = *client;
    printf("Due ");
    getCorrectDate(&pProject->dueDate);
    pProject->projectStatus=eWaiting;
    pProject->feedback = NULL;
    return 1;
}

void printProject(const void* project){
    const Project* pProject = (const Project*) project;


    if(!pProject) {
        printf("Project not found!");
        return;
    }
    if (pProject->zoomMeeting == false){
        printf("We don't have details about the meeting.\n");
        return;
    }

    printf("\nProject Name: %s, \t Client Name: %s, \t",pProject->name,pProject->client.info.name);
    printDate(&pProject->dueDate);
    printf("\t Project Status: %s,\n" , enumStatus[pProject->projectStatus]);
    printWebsiteRequirements(project);
    if(pProject->feedback == NULL || pProject->projectStatus!=2)
        printf("NO FEEDBACK YET!\n");
    else
        printf("Feedback Rate: %d\nFeedback Description: %s\n",pProject->feedback->rate,pProject->feedback->feedbackStr);
}


//int  changeProjectStatus(Project* pProject){
//    CHECK_RETURN_INT(pProject,"Project was not found!");
//    if(pProject->projectStatus == 2){
//        printf("The project is already finished, can't change project status\n");
//        return 1;
//    }
//
//    int option;
//    printf("\n\n");
//    do{
//        printf("What is the status of the project?\n");
//        for (int i = 0; i < eStatusNofOption; i++)
//            printf("%d for %s\n", i, enumStatus[i]);
//        scanf("%d", &option);
//    }while(option < 0 || option >= eStatusNofOption);
//
//    if(option == 2){
//
//    }
//    pProject->projectStatus = option;
//
//    return 1;
//}

int  removeFeedback(Project* pProject){
    CHECK_RETURN_INT(pProject,"Project was not found!");

    freeFeedback(pProject->feedback);
    pProject->feedback = NULL;
    return 1;
}

void  changeDueDate(Project* pProject){
    getCorrectDate(&pProject->dueDate);
}

int addFeedback(Project* pProject){
    CHECK_RETURN_INT(pProject,"Project was not found!");

    if (pProject->feedback){
        printf("Feedback Already Exists.\n");
        return 0;
    }

    if (pProject->projectStatus != 2){
        printf("Cannot write feedback before the project completed.\n");
        return 0;
    }

    pProject->feedback = malloc(sizeof (Feedback));
    if(!pProject->feedback){
        printf("Memory Allocation Failed!\n");
        return 0;
    }

    pProject->feedback->feedbackStr = getFeedbackString(pProject->feedback);
    if(pProject->feedback->feedbackStr == NULL){
        freeFeedback(pProject->feedback);
        pProject->feedback = NULL;
    }
    return 1;
}

void  freeProject(Project* pProject){
   free(pProject->name);
   if(pProject->feedback && pProject->projectStatus == 2){
       freeFeedback(pProject->feedback);
   }
    freeWebsiteReqArr(pProject->websiteReq, pProject->reqNumber);
    free(pProject->websiteReq);
}

int getClientWebsiteRequirements(Project* pProject){
    CHECK_RETURN_INT(pProject,"Project was not found!");

    if(!pProject->zoomMeeting){
        printf("Cannot write requirements before chatting with the client!\n");
        return 0;
    }

    int reqNumber;
    do{
        printf("How much requirements did the client had?(minimum %d , maximum %d)\n",MIN,MAX);
        int ignore = scanf("%d" ,&reqNumber);

        if(reqNumber < MIN || reqNumber > MAX){
            printf("Invalid input, ");
        }
    } while(reqNumber < MIN || reqNumber > MAX);

    pProject->reqNumber = reqNumber;

    if(writeWebsiteReq(pProject) == 0){
        return 0;
    } else return 1;
}

int writeWebsiteReq(Project* pProject){
    CHECK_RETURN_INT(pProject,"Project was not found!");

    pProject->websiteReq = (char **)malloc(pProject->reqNumber * sizeof(char *));
    if (pProject->websiteReq == NULL) {
        printf("Memory allocation failed\n");
        return 0; // Exit the program with an error status
    }

    printf("Client websites requirements\n");
    for (int i = 0; i < pProject->reqNumber ; i++) {
        printf("Requirement number %d: ", i+1);
        pProject->websiteReq[i] = getStrExactName("");
        if(pProject->websiteReq[i] == NULL){
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(pProject->websiteReq[j]);
            }
            free(pProject->websiteReq);
            return 0; // Exit the function with an error status
        }
    }
    return 1;
}

void isZoomMeeting(Project* pProject){
    int choice;
    do {
        printf("Is the zoom meeting occurred?(0 - No, 1 - Yes)\n");
        int ignore = scanf("%d", &choice);
        if (choice != 0 && choice != 1) {
            printf("Invalid Input, ");
        }
    } while(choice != 0 && choice != 1);

    if(choice == 1)
        zoomMeetingOccurred(pProject);
    else pProject->zoomMeeting = false;
}

void zoomMeetingOccurred(Project* pProject){
    pProject->zoomMeeting = true;
}


void    printWebsiteRequirements(const Project* pProject){
    printf("The summary from the meeting showed %d requirements\n",pProject->reqNumber);
    for (int i = 0; i < pProject->reqNumber ; i++) {
        printf("Require %d :  %s\n",i+1 , pProject->websiteReq[i]);
    }
}

void	freeWebsiteReqArr(char** arr, int size)
{
    for (int i = 0; i < size; i++) {
        free(arr[i]);
    }

}

//Helper Function