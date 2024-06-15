#include <string.h>
#include "ProjectManagement.h"
#include "Macros.h"

typedef enum
{
    ePrintProjectManagement,ePrintAllTasks,ePrintTeamMemberTasks , eDeleteTeamMember, eAddTask, eDeleteTask,
    eChangeTaskStatus, eChangeProjectStatus, eNofOptionsProjectStatus
} eProjectManagementMenuOptions;

const char* strProjectManagement[eNofOptionsProjectStatus] = { "Print Project Management","Print All Tasks", "Print Team Member Tasks" , "Delete Team Member",
                                                               "Add Task", "Delete Task",
                                                               "Change Task Status", "Change Project Status" };



int initProjectManagement(ProjectManagement* PM,Client* client){
    CHECK_RETURN_INT(PM,"Project Management was not found!");
    CHECK_RETURN_INT(client,"Client was not found!");
    int num = initProject(&PM -> project, client);
    if(num == 0) {
     return 0;
    } else if(num == -1){
        printf("Can't create a project without a zoom meeting with the client\n");
        return -1;
    }
    PM -> numberOfTeamMembers = 0;
    PM -> projectTeamMembersArray = NULL;
    PM -> numberOfTasks = 0;
    if(!initPayment(&PM -> payment, PM -> numberOfTasks)){
        freeProject(&PM -> project);
        return 0;
    }
    if(!L_init(&PM ->projectManagementTasksLists)){
        freeProject(&PM -> project);
        return 0;
    }
    return 1;
}

void printProjectManagement(const void* projectManagement){
    const ProjectManagement* PM = *(const ProjectManagement**)projectManagement;
    if(!PM) {
        printf("Error! Something went wrong :(\n");
        return;
    }
    printProject(&PM -> project);
    printProjectTeamMembersArray(PM);
    printf("Tasks number: %d\n", PM->numberOfTasks);
    printPayment(&PM -> payment);
}

void printProjectManagementShort(const void* projectManagement){
    const ProjectManagement* PM = *(const ProjectManagement**)projectManagement;
    if(!PM) {
        printf("Error! Something went wrong :(\n");
        return;
    }
    printf("Project name: %s\t, Client ID : %d\t Client name: %s\n\n", PM->project.name,PM->project.client.id , PM->project.client.info.name);
}

int addTeamMemberToProjectManagement(ProjectManagement* PM , TeamMember* TM){
    CHECK_RETURN_INT(PM,"Project Management was not found!");
    CHECK_RETURN_INT(TM,"TeamMember was not found!");
    if(findTeamMemberWithTheSameId(PM,TM->id) != NULL){
        printf("This team member is already in the project system\n");
        return 0;
    }
 
    TeamMember** temp = (TeamMember**)realloc(PM->projectTeamMembersArray, (PM->numberOfTeamMembers + 1) * sizeof(TeamMember*));
    if (temp != NULL) {
        PM->projectTeamMembersArray = temp;
    }
    else {
        return 0;
    }
    PM->projectTeamMembersArray[PM->numberOfTeamMembers] = TM;
    PM->numberOfTeamMembers++;
    return 1;
}

int removeTeamMemberFromProjectManagement(ProjectManagement* PM){
    CHECK_RETURN_INT(PM,"Project Management was not found!");

    if(PM->numberOfTeamMembers == 0){
        printf("There isn't a team member on the project Management\n");
        return -1;
    }
    int num;
    printf("Which team member do you want to remove?\n");
    for (int i = 0; i < PM->numberOfTeamMembers; i++) {
        printf("%d) ", i+1);
        printTeamMember(&PM->projectTeamMembersArray[i]);
    }
    do{
        int ignore = scanf("%d",&num);
        if(num < 1 || num > PM->numberOfTeamMembers){
            printf("Invalid number, try again");
        }
    } while (num < 1 || num > PM->numberOfTeamMembers);

    return removeTeamMemberFromProjectManagementHelper(PM, num - 1);
}

int removeTeamMemberFromProjectManagementHelper(ProjectManagement* PM, int num){
    int temp = removeAllTasksOfTheProjectManagement(PM, PM->projectTeamMembersArray[num]);
    PM->projectTeamMembersArray[num]->numberOfTasks = MINUS(PM->projectTeamMembersArray[num]->numberOfTasks, temp);
    PM->numberOfTasks = MINUS(PM->numberOfTasks, temp);
    deleteTasksToTasksNumberPayment(&PM->payment, temp);

    if(PM->numberOfTeamMembers == 1){
        PM->projectTeamMembersArray = NULL;
        PM->numberOfTeamMembers--;
        return 1;
    } else {
        return deleteTeamMemberInArray(PM, num);
    }
}

int removeAllTasksOfTheProjectManagement(ProjectManagement* PM, TeamMember* teamMember) {
    CHECK_RETURN_INT(teamMember,"TeamMember was not found!");

    int counter = 0;
    counter += removeAllTasksOfTheProjectManagementTeamMemberList(PM, &teamMember->openTasksList.head);
    counter += removeAllTasksOfTheProjectManagementTeamMemberList(PM, &teamMember->closedTasksList.head);
    return counter;
}

int removeAllTasksOfTheProjectManagementTeamMemberList(ProjectManagement* PM, NODE* headListTeamMemberTask) {
    int counter = 0;
    NODE *pNodePM = &PM->projectManagementTasksLists.head;
    NODE *pNodeP = headListTeamMemberTask;
    int num;
    while (pNodeP->next != NULL) {
        num = 1;
        pNodePM = &PM->projectManagementTasksLists.head;
        while (pNodePM->next != NULL) {
            if (sameTaskID(pNodePM->next->key, pNodeP->next->key) == 0) {
                counter++;
                L_delete(pNodeP, freeTask);
                L_delete(pNodePM, freeTask);
                num = 0;
                break;
            }
            pNodePM = pNodePM->next;
        }
        if(num == 1){
            pNodeP = pNodeP->next;
        }
    }

    return counter;
}



int addTask(ProjectManagement* PM){
    CHECK_RETURN_INT(PM,"Project Management was not found!");
    TeamMember* teamMember = getTeamMember(PM->projectTeamMembersArray,PM->numberOfTeamMembers);
    if(!teamMember){
        return 0;
    }
    Task* task = (Task*)malloc(sizeof(Task));
    if (!task)
        return 0;

    if(!initTask(task, (struct TeamMember*) teamMember)){
        freeTask(task);
        return 0;
    }

    if(insertByID(teamMember, task, &teamMember->openTasksList.head) == 0){
        return 0;
    }

    return addTaskToProManList(PM, task);
}

int addTaskToProManList(ProjectManagement* PM, Task* task){
    CHECK_RETURN_INT(task,"Task was not found!")
    NODE* pNode = &PM->projectManagementTasksLists.head;
    while(pNode->next != NULL) {
        pNode = pNode->next;
    }
    Task* taskCopy = (Task*)malloc(sizeof(Task));
    if(taskCopy == NULL){
        return 0;
    }

    createCopyTask(taskCopy, task);
    if(L_insert(pNode,taskCopy) == NULL){
        printf("Task Unsuccessfully entered to PM!\n ");
        return 0;
    }
    PM -> numberOfTasks++;
    addTasksToTasksNumberPayment(&PM->payment, 1);
    return 1;
}

void createCopyTask(Task* taskCopy,const Task* task){
    memcpy(taskCopy, task, sizeof(Task));
    taskCopy->taskStr = (char*)malloc(strlen(task->taskStr) + 1);
    if (taskCopy->taskStr == NULL) {
        return;
    }
    strcpy(taskCopy->taskStr, task->taskStr);
}


int deleteTask(ProjectManagement* PM){
    if(PM->numberOfTasks == 0) {
        printf("There isn't a task on the project Management\n");
        return 0;
    }

    L_printWithNumbers(&PM->projectManagementTasksLists, printTask);

    int temp = chooseTask(PM);

    temp = removeTaskOfTheProjectManagement(PM, temp);

    if(temp == 1){
        deleteTasksToTasksNumberPayment(&PM->payment, 1);
        printf("The task has been deleted\n");
    } else {
        printf("The task was not found\n");
        }
    return 1 ;
    }


int chooseTask(ProjectManagement* PM){
    int temp;
    do {
        printf("Enter the serial number of the task:\n");
        int ignore = scanf("%d", &temp);
        if (temp < 1 || temp > PM->numberOfTasks){
            printf("Invalid number, ");
        }
    } while (temp < 1 || temp > PM->numberOfTasks);
    return temp;
}

int removeTaskOfTheProjectManagement(ProjectManagement* PM, int skip){

    NODE* pNodePM = &PM->projectManagementTasksLists.head;
    for (int i = 0; i < skip - 1; ++i) {
        pNodePM = pNodePM -> next;
    }
    if(!pNodePM || !pNodePM->next){
        return 0;
    }
    int numIdTeamMember = returnTeamMemberID(pNodePM -> next -> key);
    int numIdTask = returnIdTask(pNodePM -> next -> key);
    L_delete(pNodePM, freeTask);
    findTeamMemberWithTheSameIdAndDeleteTask(PM, numIdTeamMember, numIdTask);
    PM->numberOfTasks--;
    return 1;
}

void findTeamMemberWithTheSameIdAndDeleteTask(ProjectManagement* PM, int numIdTeamMember, int numIdTask){
    TeamMember* teamMember = findTeamMemberWithTheSameId(PM, numIdTeamMember);
    deleteTaskOfTeamMemberByID(teamMember, numIdTask);
}


TeamMember* findTeamMemberWithTheSameId(const ProjectManagement* PM, int numIdTeamMember){
    for (int i = 0; i < PM->numberOfTeamMembers; i++) {
        if(PM->projectTeamMembersArray[i]->id == numIdTeamMember){
            return PM->projectTeamMembersArray[i];
        }
    }
    return NULL;
}

void freeProjectManagement(ProjectManagement* PM){
    freeProject(&PM->project);
    L_free(&PM->projectManagementTasksLists, freeTask);
    free(PM->projectTeamMembersArray);
}



//Helper Functions

int deleteTeamMemberInArray(ProjectManagement* PM, int num) {
    if(num + 1 != PM->numberOfTeamMembers){
        for (int i = num; i < PM->numberOfTeamMembers; i++) {
            PM->projectTeamMembersArray[i] = PM->projectTeamMembersArray[i+1];
        }
    }

    if (PM->numberOfTeamMembers - 1 == 0) {
        free(PM->projectTeamMembersArray);
        PM->projectTeamMembersArray = NULL;
    }
    else {
        TeamMember** temp = (TeamMember**)realloc(PM->projectTeamMembersArray, (PM->numberOfTeamMembers - 1) * sizeof(TeamMember*));
        if (temp != NULL) {
            PM->projectTeamMembersArray = temp;
        }
        else {
            printf("Allocation failed\n");
        }
    }
    PM->numberOfTeamMembers--;
    return 1;
}

int printProjectTeamMembersArray(const ProjectManagement* PM){
    printf("Team members: \n");
    if(PM->numberOfTeamMembers == 0){
        printf("There are no team members yet\n");
        return 0;
    }
    generalArrayFunctionWithNumbers(PM->projectTeamMembersArray, PM->numberOfTeamMembers, sizeof(TeamMember*) , printTeamMember);
    return 1;
}


TeamMember* getTeamMember(TeamMember** teamMembersArray,int size){
    if(size == 0 || teamMembersArray == NULL){
        printf("There isn't team members on the list\n");
        return NULL;
    }
    int choice = 0;

    printf("What type of profession do you need for that task?\n");
    printSpeciality();
    int ignore = scanf("%d",&choice);

    while(choice < 1 || choice >4){
        printf("You picked invalid choice , Please pick again\n");
        ignore = scanf("%d",&choice);
    }
    return getTeamMemberByType(teamMembersArray,size,choice -1);

}

TeamMember* getTeamMemberByType(TeamMember** teamMembersArray,int size,int option){
    int tempCounter = 0;
    int* tempArr = NULL;
    for (int i = 0; i < size; i++) {
        if(teamMembersArray[i]->speciality != option) continue;
        printf("%d) " , tempCounter+1 );
        printTeamMember(&teamMembersArray[i]);
        printf("\n");
        int* temp = (int*)realloc(tempArr, (tempCounter + 1) * sizeof(int));
        if (temp != NULL) {
            tempArr = temp;
        }
        else {
            printf("Failed To Allocate Memory\n");
            free(tempArr);
            return NULL;
        }
        tempArr[tempCounter] = i;
        tempCounter++;
    }
    if (tempCounter == 0){
        printf("No team member with that speciality!\n");
        return NULL;
    } do{
        printf("Pick a team member from the list above:\n");
        int ignore = scanf("%d", &option);
        if(option < 1 || option > tempCounter) printf("Invalid choice, ");
    }while(option < 1 || option > tempCounter);
    TeamMember* selectedTeamMember = teamMembersArray[tempArr[option - 1]];
    free(tempArr);
    return selectedTeamMember;
}


void printPMTasksList(const ProjectManagement* PM){
    if(PM->numberOfTasks == 0){
        printf("There aren't tasks yet\n");
    }
    L_printWithNumbers(&PM->projectManagementTasksLists, printTask);
}



int changeTaskStatusProjectManagement(ProjectManagement* PM){
    CHECK_RETURN_INT(PM,"Project Management was not found!");

    if(PM->numberOfTasks == 0){
        printf("there aren't tasks on the list.\n");
        return -1;
    }

    printPMTasksList(PM);
    int skip = chooseTask(PM);

    NODE * pNode = &PM->projectManagementTasksLists.head;
    for (int i = 0; i < skip ; i++) {
        pNode = pNode -> next;
    }
    if(!pNode)
        return 0;
    int statusBefore = getTaskStatus(pNode->key);
    int newStatus = changeTaskStatus(pNode->key);

    return  changeTaskStatusProjectManagementHelper(PM, statusBefore, newStatus, pNode );
}

int changeTaskStatusProjectManagementHelper(ProjectManagement* PM, int statusBefore, int newStatus, NODE* pNode){
    int taskIdTeamMember = returnTeamMemberID(pNode->key);
    int taskIdNumber = returnIdTask(pNode->key);
    TeamMember* teamMember = findTeamMemberWithTheSameId(PM, taskIdTeamMember);
    NODE* node =  changeTaskStatusOfTeamMemberByID(teamMember, taskIdNumber, newStatus);

    if((statusBefore == 0 || statusBefore == 1) && (newStatus == 0 || newStatus == 1) || (statusBefore == 2 && newStatus == 2)){
        return 1;
    }

    if ((statusBefore == 0 || statusBefore == 1) && newStatus == 2) {
        if (switchLists(teamMember, node, &teamMember->openTasksList.head, &teamMember->closedTasksList.head) == 0) {
            return 0;
        }
    }
    if (statusBefore == 2 && (newStatus == 0 || newStatus == 1)) {
        if (switchLists(teamMember, node, &teamMember->closedTasksList.head, &teamMember->openTasksList.head) == 0) {
            return 0;
        }
    }
    return 1;
}

int         changeProjectStatus(ProjectManagement* PM){
    CHECK_RETURN_INT(PM,"Project was not found!")
    Task* temp;
    if(PM->project.projectStatus == 2){
        printf("The project is already finished, can't change project status\n");
        return 1;
    }

    int option;
    printf("\n\n");
    do{
        printf("What is the status of the project?\n");
        for (int i = 0; i < eStatusNofOption; i++)
            printf("%d for %s\n", i, enumStatus[i]);
        int ignore = scanf("%d", &option);
    }while(option < 0 || option >= eStatusNofOption);

    if(option == 2){
        NODE * pNode = &PM->projectManagementTasksLists.head;

        for (int i = 0; i < PM->numberOfTasks ; i++) {
            pNode = pNode -> next;
            temp = pNode->key;
            if (temp->taskStatus != 2){
                printf("Can't close project when not all tasks are finished!\n");
                return 0;
            }

        }
    }
    PM->project.projectStatus = option;

    return 1;
}


int switchLists(TeamMember* TM, NODE* pTemp, NODE* headListFrom, NODE* headListTo){
    if(!TM || !pTemp)
        return 0;

    insertByID_TASKS(headListTo,pTemp->key);
    NODE* oTasksFind = L_find(headListFrom,pTemp->key,sameTaskID);
    if(!oTasksFind)
        return 0;
    L_delete(oTasksFind,NULL);
    return 1;
}


void printTeamMemberTasksYouChoose(const ProjectManagement* PM){
    if(!printProjectTeamMembersArray(PM)){
        return;
    }
    CHOOSE(num,1,PM->numberOfTeamMembers);
    printf("%s tasks:\n",PM->projectTeamMembersArray[num - 1]->info.name );
    printTeamMemberTasks(PM->projectTeamMembersArray[num - 1]);
}

void projectManagementMenu(ProjectManagement* PM){

        int option;
        int stop = 0;


        do
        {
            option = projectManagementMenuHelper();
            switch (option)
            {
                case ePrintProjectManagement:
                    printProjectManagement(&PM);
                    break;

                case ePrintAllTasks:
                    printPMTasksList(PM);
                    break;

                case ePrintTeamMemberTasks:
                    printTeamMemberTasksYouChoose(PM);
                    break;

                case eDeleteTeamMember:
                    if (!removeTeamMemberFromProjectManagement(PM))
                        printf("Error adding task\n");
                    break;

                case eAddTask:
                    if (PM->numberOfTasks < MAXTASKS) {
                        addTask(PM);
                    }    
                    else {
                        printf("Can't add more than %d Tasks", MAXTASKS);
                    }
                    break;

                case eDeleteTask:
                    deleteTask(PM);
                    break;

                case eChangeTaskStatus:
                    if (!changeTaskStatusProjectManagement(PM))
                        printf("Error change task status\n");
                    break;

                case eChangeProjectStatus:
                    if (!changeProjectStatus(PM))
                        printf("Error change project status\n");
                    break;

                case EXIT:
                    printf("Exiting the project management system for the company\n\n");
                    stop = 1;
                    break;

                default:
                    printf("Wrong option\n");
                    break;
            }
        } while (!stop);

    }

    int projectManagementMenuHelper()
    {
        int option;
        printf("\n\n");
        printf("Please choose one of the following options\n");
        for(int i = 0 ; i < eNofOptionsProjectStatus ; i++)
            printf("%d - %s\n",i,strProjectManagement[i]);
        printf("%d - Quit\n", EXIT);
        int ignore = scanf("%d", &option);
        //clean buffer
        char tav;
        ignore = scanf("%c", &tav);
        return option;
    }


