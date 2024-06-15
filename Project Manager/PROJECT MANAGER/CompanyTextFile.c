#include "CompanyTextFile.h"
#include <string.h>
#include <ctype.h>
#include "Macros.h"


int saveCompanyToFile(const Company* pCompany, const char* fileName) {
    FILE* fp;
    fp = fopen(fileName, "w");
    if (!fp) {
        return 0;
    }
    writeContactToFile(&pCompany->info, fp);
    writeClientsArrayCompanyToFile(fp, pCompany->clientsArray, pCompany->numOfClients);
    fprintf(fp, "%d\n",pCompany->sort);
    writeCandidatesArrayCompanyToFile(fp, (const Candidate**)pCompany->candidatesArray, pCompany->numOfCandidates);
    writeTeamMembersArrayCompanyToFile(fp, (const TeamMember**)pCompany->teamMembersArray, pCompany->numOfTeamMembers);
    writeProjectManagementArrayCompanyToFile(fp, (const ProjectManagement**) pCompany->projectManagementsArray, pCompany->numOfProjects);
    fprintf(fp, "%d\n",pCompany->companyEarning);

    fclose(fp);
    return 1;
}

void writeContactToFile(const Contact* pContact, FILE* fp){
    fprintf(fp, "%s\n%s\n%s\n", pContact->name, pContact->phoneNumber, pContact->email);
}

void writeClientsArrayCompanyToFile(FILE* fp, const Client* pClient, int clientsNum){
    fprintf(fp, "%d\n", clientsNum);
    for (int i = 0; i < clientsNum; i++) {
        writeClientToFile(&pClient[i], fp);
    }
}

void writeClientToFile(const Client* pClient, FILE* fp){
    fprintf(fp, "%d\n%s\n%s\n%s\n", pClient->id,pClient->info.name, pClient->info.phoneNumber, pClient->info.email);
}

void writeCandidatesArrayCompanyToFile(FILE* fp, const Candidate** pCandidate, int CandidatesNum){
    fprintf(fp, "%d\n", CandidatesNum);
    for (int i = 0; i < CandidatesNum; i++) {
        writeCandidateToFile(pCandidate[i],fp);
    }
}

void writeCandidateToFile(const Candidate * pCandidate, FILE* fp){
    writeContactToFile(&pCandidate->info, fp);
    fprintf(fp, "%d\n%d\n", pCandidate->speciality,pCandidate->experience);
}

void writeTeamMembersArrayCompanyToFile(FILE* fp, const TeamMember** pTeamMember, int TeamMembersNum){

    fprintf(fp, "%d\n", TeamMembersNum);
    for (int i = 0; i < TeamMembersNum; i++) {
        writeTeamMemberToFile(pTeamMember[i], fp);
    }
}

void writeTeamMemberToFile(const TeamMember* pTeamMember, FILE* fp){
    fprintf(fp, "%d\n", pTeamMember->id);
    writeContactToFile(&pTeamMember->info, fp);
    fprintf(fp, "%d\n%d\n", pTeamMember->speciality,pTeamMember->experience);
}


void writeProjectManagementArrayCompanyToFile(FILE* fp, const ProjectManagement** ProjectManagement, int ProjectManagementNum){
    fprintf(fp, "%d\n", ProjectManagementNum);
    for (int i = 0; i < ProjectManagementNum; i++) {
        writeProjectManagementToFile(ProjectManagement[i], fp);
    }
}

void writeProjectManagementToFile(const ProjectManagement* pProjectManagement, FILE* fp){
    writeProjectToFile(&pProjectManagement->project,fp);
    writePMTeamMembersIDArrayToFile(pProjectManagement,fp);
    writePMTasksToFile(pProjectManagement,fp);
    writePaymentToFile(&pProjectManagement->payment,fp);
}

void writeProjectToFile(const Project* pProject, FILE* fp) {
    fprintf(fp, "%d\n%s\n", pProject->zoomMeeting, pProject->name);
    writeClientWebsiteRequirements(pProject, fp);
    writeClientToFile(&pProject->client, fp);
    writeDateToFile(&pProject->dueDate, fp);
    fprintf(fp, "%d\n", pProject->projectStatus);
    if (!pProject->feedback) {
        fprintf(fp, "%d\n", -1);
    }
        if (pProject->projectStatus == 2 && pProject->feedback) {
            fprintf(fp, "%d\n", 1);
            fprintf(fp, "%d\n%s\n", pProject->feedback->rate, pProject->feedback->feedbackStr);
        }
    }

void writeClientWebsiteRequirements(const Project* pProject, FILE* fp){
    fprintf(fp, "%d\n", pProject->reqNumber);
    for (int i = 0; i < pProject->reqNumber; ++i) {
        fprintf(fp, "%s\n",  pProject->websiteReq[i]);
    }
}


void writeDateToFile(const Date* pDate ,FILE* fp ){
    fprintf(fp, "%d\n%d\n%d\n",  pDate->day ,pDate->month,pDate->year);


}

void writePMTeamMembersIDArrayToFile(const ProjectManagement* pProjectManagement, FILE* fp){
    fprintf(fp, "%d\n", pProjectManagement->numberOfTeamMembers);
    for (int i = 0; i < pProjectManagement->numberOfTeamMembers; ++i) {
        fprintf(fp, "%d\n", pProjectManagement->projectTeamMembersArray[i]->id);
    }
}

void writePMTasksToFile(const ProjectManagement * pProjectManagement, FILE* fp){
    const NODE* pNode = &pProjectManagement->projectManagementTasksLists.head;
    Task* tempTask;
    pNode = pNode->next;
    fprintf(fp, "%d\n", pProjectManagement->numberOfTasks);
    for (int i = 0; i <  pProjectManagement->numberOfTasks; ++i) {
        tempTask = pNode->key;
        writeTaskToFile(tempTask,fp);
        pNode = pNode->next;
    }
}


void writeTaskToFile(const Task* pTask, FILE* fp){
    TeamMember* pTeamMember = (TeamMember *) pTask->teamMember;
    fprintf(fp, "%d\n%s\n%d\n%d\n",pTask->id, pTask->taskStr , pTask->taskStatus,pTeamMember->id);
}

void writePaymentToFile(const Payment* pPayment, FILE* fp){
    fprintf(fp, "%d\n%d\n%d\n%d\n%d\n", pPayment->id,pPayment->tasksNumber,pPayment->total,pPayment->status,pPayment->NofPayments);
    if(pPayment->status == 1){
        writeDateToFile(&pPayment->paymentDate,fp);
    }
}
//--------------------------------------------------------------------------------------------------------------------


int readCompanyFromFile( Company* pCompany, const char* fileName){

    FILE* fp;
    fp = fopen(fileName, "r");
    if (!fp) {
        return 0;
    }
    if(!readContactFromFile(&pCompany->info,fp)){
        return 0;
    }

    if(!readClientsArrayFromFile(pCompany, fp)){
        freeContact(&pCompany->info);
        return 0;
    }

    if(fscanf(fp,"%d",&pCompany->sort)!=1)
        return 0;

    if(!readCandidatesArrayCompanyFromFile(pCompany, fp)){
        freeClientArrayFileVersion(pCompany, pCompany->numOfClients);
        freeContact(&pCompany->info);
        return 0;
    }
    if(!readTeamMembersArrayCompanyFromFile(pCompany, fp)){
        freeCandidatesArrayFileVersion(pCompany, pCompany->numOfCandidates);
        freeClientArrayFileVersion(pCompany, pCompany->numOfClients);
        freeContact(&pCompany->info);
    }
    
    if(!readProjectManagementsArrayCompanyFromFile(pCompany,fp)){
        freeTeamMembersArrayCompanyFileVersion(pCompany, pCompany->numOfTeamMembers);
        freeCandidatesArrayFileVersion(pCompany, pCompany->numOfCandidates);
        freeClientArrayFileVersion(pCompany, pCompany->numOfClients);
        freeContact(&pCompany->info);
    }
    if (fscanf(fp, "%d", &pCompany->companyEarning) != 1) {
        freeProjectManagementArrayFileVersion(pCompany, pCompany->numOfProjects);
        freeTeamMembersArrayCompanyFileVersion(pCompany, pCompany->numOfTeamMembers);
        freeCandidatesArrayFileVersion(pCompany, pCompany->numOfCandidates);
        freeClientArrayFileVersion(pCompany, pCompany->numOfClients);
        freeContact(&pCompany->info);
        return 0;
    }
    fclose(fp);
    return 1;

}

int readContactFromFile(Contact* pContact, FILE* fp){
    char name[255] = { 0 };
    char phoneNumber[11] = { 0 };
    char email[255] = { 0 };

    do {
        if (fgets(name, sizeof(name), fp) == NULL)
            return 0;
    } while (isspace((unsigned char)name[0]));

    removeNewLine(name);

    if (fscanf(fp, "%s", phoneNumber) != 1) {
        return 0;
    }

    do {
        if (fgets(email,sizeof (email), fp) == NULL)
            return 0;
    } while (isspace((unsigned char)email[0]));

    removeNewLine(email);

    pContact->name = _strdup(name);
    pContact->email = _strdup(email);
    strcpy(pContact->phoneNumber,phoneNumber);


    if(!pContact->name || !pContact->email || pContact->phoneNumber[0] == '\0') return 0;

    return 1;

}


int readClientsArrayFromFile( Company* pCompany, FILE* fp){
    Client* temp;

    if(fscanf(fp,"%d",&pCompany->numOfClients)!=1)
        return 0;
    int clientsNumber = pCompany->numOfClients;

    pCompany->clientsArray = NULL;

    if(clientsNumber == 0)
        return 1;

    int maxID = 0;

    for (int i = 0; i < clientsNumber; i++) {
        temp = (Client*)malloc(sizeof(Client));
        if (!temp)
            return 0;
        if (!readClientFromFile(temp, fp))
        {
            freeClientArrayFileVersion(pCompany,i);
            free(temp);
            return 0;
        }

        addClientFromFile(pCompany,temp,i);
        if(temp->id > maxID){
            maxID = temp->id;
        }
    }

    addToClientID(maxID);

    return 1;
}


int readClientFromFile( Client* client, FILE* fp){

    if (fscanf(fp,"%d",&client->id) != 1)
        return 0;

    readContactFromFile(&client->info,fp);

    return 1;
}

int readCandidatesArrayCompanyFromFile(Company* pCompany, FILE* fp){
    Candidate * temp;
    if(fscanf(fp,"%d",&pCompany->numOfCandidates)!=1)
        return 0;
    int candidatesNumber = pCompany->numOfCandidates;

        pCompany->candidatesArray = (Candidate**)malloc(candidatesNumber * sizeof(Candidate*));
        if (pCompany->candidatesArray == NULL) {
            return 0; // Or handle the error appropriately
        }

    if(candidatesNumber == 0)
        return 1;
    for (int i = 0; i < candidatesNumber; i++) {
        temp = (Candidate*)malloc(sizeof(Candidate));
        if (!temp)
            return 0;
        if (!readCandidateCompanyFromFile(temp, fp))
        {
            freeCandidatesArrayFileVersion(pCompany,i);
            free(temp);
            return 0;
        }
        addCandidateFromFile(pCompany,temp,i);
    }
    return 1;
}


int readCandidateCompanyFromFile(Candidate * pCandidate, FILE* fp){
    readContactFromFile(&pCandidate->info,fp);
    if(fscanf(fp,"%d",&pCandidate->speciality) != 1)
        return 0;

    if(fscanf(fp,"%d",&pCandidate->experience) != 1)
        return 0;

    return 1;
}


int readTeamMembersArrayCompanyFromFile(Company* pCompany, FILE* fp){
    TeamMember* temp;

    if(fscanf(fp,"%d",&pCompany->numOfTeamMembers)!=1)
        return 0;
    int teamMembersNumber = pCompany->numOfTeamMembers;

        pCompany->teamMembersArray = (TeamMember **)malloc(teamMembersNumber * sizeof(TeamMember*));
        if (pCompany->teamMembersArray == NULL) {

            return 0; // Or handle the error appropriately
        }

    if(teamMembersNumber == 0)
        return 1;

    int maxID = 0;
    for (int i = 0; i < teamMembersNumber; i++) {
        temp = (TeamMember*)malloc(sizeof(TeamMember));
        if (!temp)
            return 0;
        if (!readTeamMemberCompanyFromFile(temp, fp))
        {
            freeTeamMembersArrayCompanyFileVersion(pCompany,i);
            free(temp);
            return 0;
        }
        addTeamMemberFromFile(pCompany,temp,i);
        if(temp->id > maxID){
            maxID = temp->id;
        }
    }
    addToTeamMemberID(maxID);
    return 1;
}


int readTeamMemberCompanyFromFile(TeamMember* pTeamMember, FILE* fp){
    if(fscanf(fp,"%d",&pTeamMember->id)!=1)
        return 0;
    ZERO_COUNTER(readContactFromFile(&pTeamMember->info,fp),"")
    if(fscanf(fp,"%d",&pTeamMember->speciality)!=1)
        return 0;
    if(fscanf(fp,"%d",&pTeamMember->experience)!=1)
        return 0;

    pTeamMember->numberOfTasks=0;// Check

    L_init(&pTeamMember->openTasksList);
    L_init(&pTeamMember->closedTasksList);
    return 1;
}


int readProjectManagementsArrayCompanyFromFile(Company* pCompany, FILE* fp){
    ProjectManagement* temp;
    if(fscanf(fp,"%d",&pCompany->numOfProjects)!=1)
        return 0;
    int projectsNumber = pCompany->numOfProjects;

        pCompany->projectManagementsArray = (ProjectManagement **)malloc( projectsNumber * sizeof(ProjectManagement*));
        if (pCompany->projectManagementsArray == NULL) {
            return 0; // Or handle the error appropriately
        }

    if(projectsNumber == 0)
        return 1;
    for (int i = 0; i < projectsNumber; i++) {
        temp = (ProjectManagement*)malloc(sizeof(ProjectManagement));
        if (!temp)
            return 0;
          int asd = ftell(fp);
        if (!readProjectManagementFromFile(temp, pCompany->teamMembersArray, pCompany->numOfTeamMembers ,  fp))
        {
            freeProjectManagementArrayFileVersion(pCompany,i);
            free(temp);
            return 0;
        }

        addProjectManagementFromFile(pCompany,temp,i);
    }
    return 1;
}


int readProjectManagementFromFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr,int amount, FILE* fp){
  
    if(!readProjectFromFile(&pProjectManagement->project, fp)){
        return 0;
    }
    if(!readTeamMembersIDFromFile(pProjectManagement, teamMemberArr, amount, fp)){
        return 0;
    }
    if(!readPMTasksFromFile(pProjectManagement, fp)){
        return 0;
    }
    if(!readPaymentFromFile(&pProjectManagement->payment, fp)){
        return 0;
    }
    return 1;
}
int readPaymentFromFile(Payment* pPayment, FILE* fp) {
    if (fscanf(fp, "%d", &pPayment->id) != 1)
        return 0;

    if (fscanf(fp, "%d", &pPayment->tasksNumber) != 1)
        return 0;

    if (fscanf(fp, "%d", &pPayment->total) != 1)
        return 0;

    if (fscanf(fp, "%d", &pPayment->status) != 1)
        return 0;

    if (fscanf(fp, "%d", &pPayment->NofPayments) != 1)
        return 0;

    if (pPayment->status == 1) {
        readDateFromTextFile(&pPayment->paymentDate, fp);
    }
    return 1;
}

int readPMTasksFromFile(ProjectManagement* pProjectManagement, FILE* fp){
    int numberOfTasks;
    L_init(&pProjectManagement->projectManagementTasksLists);

    if(fscanf(fp,"%d",&numberOfTasks)!=1)
        return 0;
    Task* temp;
    int maxID = 0;
    for (int i = 0; i < numberOfTasks; i++)
    {
        temp = (Task*)malloc(sizeof(Task));
        if (!temp)
        {
            fclose(fp);
            return 0;
        }


        if (!readPMTaskFromFile(temp, pProjectManagement, fp))
        {
            freeTeamMembersArrayPMFileVersion(pProjectManagement, pProjectManagement->numberOfTeamMembers);
            L_free(&pProjectManagement->projectManagementTasksLists, freeTask);
            free(temp);
            return 0;
        }

        if(temp->id > maxID){
            maxID = temp->id;
        }

    }
    pProjectManagement->numberOfTasks = numberOfTasks;
    addToTaskID(maxID);
    return 1;

}

int readPMTaskFromFile(Task* pTask, ProjectManagement* pProjectManagement, FILE* fp){

    if(fscanf(fp,"%d",&pTask->id)!=1)
        return 0;
    char taskStr[255] = { 0 };
    do {

        if (fgets(taskStr, sizeof(taskStr), fp) == NULL)
            return 0;
    } while (isspace((unsigned char)taskStr[0]));
    removeNewLine(taskStr);
    pTask->taskStr = _strdup(taskStr);

    if(fscanf(fp,"%d",&pTask->taskStatus)!=1)
        return 0;


    int id;
    if(fscanf(fp,"%d",&id)!=1)
        return 0;
    TeamMember* teamMember= findTeamMemberWithTheSameId(pProjectManagement, id);
    pTask->teamMember = (struct TeamMember *) teamMember;
    if(pTask->taskStatus == 2){
        if(!insertByID(teamMember, pTask,&teamMember->closedTasksList.head )){
            return 0;
        }
    } else{
        if(!insertByID(teamMember, pTask,&teamMember->openTasksList.head )){
            return 0;
        }
    }
    if(!addTaskToProManList(pProjectManagement,pTask)){
        return 0;
    }
    return 1;
}

int readTeamMembersIDFromFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp) {
    if (fscanf(fp, "%d", &pProjectManagement->numberOfTeamMembers) != 1)
        return 0;

    if (pProjectManagement->numberOfTeamMembers == 0) {
        pProjectManagement->projectTeamMembersArray = NULL;
        return 1;
    }
        

    pProjectManagement->projectTeamMembersArray =(TeamMember**)malloc(pProjectManagement->numberOfTeamMembers * sizeof(TeamMember*));
        if(pProjectManagement->projectTeamMembersArray == NULL)//ADDD
            return 0;//ADDDDDD

    int id;
    for (int i = 0; i < pProjectManagement->numberOfTeamMembers; i++) {

        if (fscanf(fp, "%d", &id) != 1) {
            return 0;
        }
        pProjectManagement->projectTeamMembersArray[i] = findTeamMemberWithTheSameIdInCompany(teamMemberArr,amount,id);
        if (!pProjectManagement->projectTeamMembersArray[i]) {
            return 0;
        }
    }

    return 1;
}

int readProjectFromFile(Project * pProject, FILE* fp){

    if(fscanf(fp,"%d",(int*)&(pProject->zoomMeeting))!=1)
        return 0;

    char projectName[255] = { 0 };
    do {
        if (fgets(projectName, sizeof(projectName), fp) == NULL)
            return 0;
    } while (isspace((unsigned char)projectName[0]));
    removeNewLine(projectName);
    pProject->name = _strdup(projectName);

    if(fscanf(fp,"%d",&pProject->reqNumber)!=1)
        return 0;
    readWebsiteReq(pProject, fp);

    readClientFromFile(&pProject->client, fp);

    readDateFromTextFile(&pProject->dueDate, fp);

    if(fscanf(fp,"%d",&pProject->projectStatus)!=1)
        return 0;

    int writeIsFeedback;
    if(fscanf(fp,"%d",&writeIsFeedback)!=1)
        return 0;
    pProject->feedback = NULL;
    if (writeIsFeedback == 1) {
        pProject->feedback = (Feedback *) malloc(sizeof(Feedback));
        if (!readFeedbackFromTextFile(pProject->feedback, fp)) {
            freeFeedback(pProject->feedback);
            return 0;
        }
    }
  


    return 1;
}

int readFeedbackFromTextFile(Feedback * pFeedback, FILE* fp){

    if(fscanf(fp,"%d",&pFeedback->rate)!=1)
        return 0;

    char Feedback[255] = { 0 };
    do {
        if (fgets(Feedback, sizeof(Feedback), fp) == NULL)
            return 0;
    } while (isspace((unsigned char)Feedback[0]));

    removeNewLine(Feedback);
    pFeedback->feedbackStr = _strdup(Feedback);

    return 1;
}

int readDateFromTextFile(Date * pDate, FILE* fp){
    if(fscanf(fp,"%d",&pDate->day)!=1)
        return 0;
    if(fscanf(fp,"%d",&pDate->month)!=1)
        return 0;
    if(fscanf(fp,"%d",&pDate->year)!=1)
        return 0;

    return 1;
}

int readWebsiteReq(Project * pProject, FILE* fp){

 
    pProject->websiteReq = (char**)calloc(pProject->reqNumber, sizeof(char*));
    for (int i = 0; i < pProject->reqNumber; i++) {
        pProject->websiteReq[i] = (char*)calloc(255, sizeof(char));
    }
    int num_lines = 0;

    // Read each line from the file and store them in the array of strings
    while (num_lines < pProject->reqNumber) {
        do {
            if (fgets(pProject->websiteReq[num_lines], sizeof(char)*255, fp) == NULL)
                return 0;
        } while (isspace((unsigned char)pProject->websiteReq[num_lines][0]));
        removeNewLine(pProject->websiteReq[num_lines]);

        num_lines++;
    }

    return 1;
}


//HELPER

void removeNewLine(char *str) {
    size_t len = strcspn(str, "\n");
    if (len > 0 && str[len - 1] == '\r') {
        // Handle the case where '\r' precedes '\n' (Windows line ending)
        len--;
    }
    str[len] = '\0';
}

int addClientFromFile(Company* pCompany , Client* client , int num){
    Client*  temp = (Client*)realloc(pCompany->clientsArray, (pCompany->numOfClients + 1) * sizeof(Client));
    if (temp != NULL) {
        pCompany->clientsArray = temp;
    }
    else {
        printf("Memory Allocation Failed.\n");
        return 0;
    }
    
    pCompany->clientsArray[num] = *client;
    return 1;

}


void freeCandidatesArrayFileVersion(Company* pCompany,int num){
    for (int i = 0; i < num; i++) {
        freeCandidate(pCompany->candidatesArray[i]);
    }
    free(pCompany->candidatesArray);
}

void freeClientArrayFileVersion(Company* pCompany,int num){
    for (int i = 0; i < num; i++) {
        freeClient(&pCompany->clientsArray[i]);
    }
}

void freeTeamMembersArrayCompanyFileVersion(Company* pCompany,int num){
    for (int i = 0; i < num; i++) {
        freeTeamMember(pCompany->teamMembersArray[i]);
    }
    free(pCompany->teamMembersArray);
}

void freeTeamMembersArrayPMFileVersion(ProjectManagement* pProjectManagement, int num){
    for (int i = 0; i < num; i++) {
        freeTeamMember(pProjectManagement->projectTeamMembersArray[i]);
    }
    free(pProjectManagement->projectTeamMembersArray);
}

int addTeamMemberFromFile(Company* pCompany , TeamMember* pTeamMember , int num){
//    pCompany->teamMembersArray = (TeamMember**)realloc(pCompany->teamMembersArray, (pCompany->numOfTeamMembers + 1) * sizeof(TeamMember*));
//    CHECK_RETURN_INT(pCompany->teamMembersArray,"Memory Allocation Failed")
    pCompany->teamMembersArray[num] = pTeamMember;

    return 1;

}

int addCandidateFromFile(Company* pCompany , Candidate * candidate , int num){
//    pCompany->candidatesArray = (Candidate**)realloc(pCompany->candidatesArray, (pCompany->numOfCandidates + 1) * sizeof(Candidate*));
//    CHECK_RETURN_INT(pCompany->candidatesArray,"Memory Allocation Failed")
    pCompany->candidatesArray[num] = candidate;

    return 1;

}



