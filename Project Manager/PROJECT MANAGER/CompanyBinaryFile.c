#include "CompanyBinaryFile.h"
#include <string.h>
#include <ctype.h>
#include "Macros.h"

//BinaryFile

int saveCompanyToBinaryFile(const Company* pCompany, const char* fileName){
    FILE* fp = fopen(fileName, "wb");
    if (!fp){
        printf("Failed to open file!\n");
        return 0;
    }

    writeContactToBinaryFile(&pCompany->info,fp);

    if (writeClientsArrayToBinaryFile(pCompany, fp) == 0) {
        fclose(fp);
        printf("Failed to read Clients!\n");
        return 0;
    }
    

    if (fwrite(&pCompany->sort, sizeof(pCompany->sort), 1, fp) != 1) {
        fclose(fp);
        printf("Failed to write sort!\n");
        return 0;
    }

    if(writeCandidatesArrayToBinaryFile(pCompany,fp) == 0) {
        fclose(fp);
        printf("Failed to write candidate!\n");
        return 0;
    }
    
    if(writeTeamMembersArrayToBinaryFile(pCompany, fp) == 0) {
        fclose(fp);
        printf("Failed to write team members!\n");
        return 0;
    }

    if (writeProjectManagementsArrayToBinaryFile(pCompany, fp) == 0) {
            fclose(fp);
            printf("Failed to write projects!\n");
            return 0;
     }

    if (fwrite(&pCompany->companyEarning, sizeof(pCompany->companyEarning), 1, fp) != 1) {
        fclose(fp);
        printf("Failed to company earning!\n");
        return 0;
    }

    fclose(fp);
    return 1;
}

void writeContactToBinaryFile(const Contact* pContact, FILE* fp){

    // Write the name
    writeStringToBinaryFile(fp, pContact->name);

    // Write the phone number (fixed length, can be written directly)
    if (fwrite(pContact->phoneNumber, sizeof(char), LengthOfPhoneNum+1, fp) != LengthOfPhoneNum + 1) {
        printf("Failed to write contact!\n");
        fclose(fp);
        return;
    }

    // Write the email
    writeStringToBinaryFile(fp, pContact->email);

}

int writeClientsArrayToBinaryFile(const Company* pCompany, FILE* fp){
    if ( fwrite(&pCompany->numOfClients, sizeof(int), 1, fp) != 1) {
        printf("Failed to find num of clients!\n");
        return 0;
    }

    for (int i = 0; i < pCompany->numOfClients; i++) {
        writeClientToBinaryFile(&pCompany->clientsArray[i],fp);
    }

    return 1;
}

int writeCandidatesArrayToBinaryFile(const Company* pCompany, FILE* fp){
    if ( fwrite(&pCompany->numOfCandidates, sizeof(pCompany->numOfCandidates), 1, fp) != 1) {
        printf("Failed to find num of clients!\n");
        return 0;
    }
   
    for (int i = 0; i < pCompany->numOfCandidates; i++) {
        writeCandidateToBinaryFile(pCompany->candidatesArray[i],fp);
    }
    return 1;
}
void writeCandidateToBinaryFile(const Candidate * pCandidate, FILE* fp){
    // Write the Contact information
    writeContactToBinaryFile(&pCandidate->info,fp);

    // Write the specialization
    if (fwrite(&pCandidate->speciality, sizeof(pCandidate->speciality), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }
 

    // Write the experience
    if (fwrite(&pCandidate->experience, sizeof(pCandidate->experience), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

}

void writeClientToBinaryFile(const Client* pClient, FILE* fp){
    // Write the client ID
    if ( fwrite(&pClient->id, sizeof(pClient->id), 1, fp) != 1) {
        printf("Failed!\n");
        return ;
    }
    writeContactToBinaryFile(&pClient->info, fp);


}

int writeTeamMembersArrayToBinaryFile(const Company* pCompany, FILE* fp){
    if (fwrite(&pCompany->numOfTeamMembers, sizeof(pCompany->numOfTeamMembers), 1, fp) != 1) {
        printf("Failed!\n");
        return 0;
    }
    for (int i = 0; i < pCompany->numOfTeamMembers; ++i) {
        writeTeamMemberToBinaryFile(pCompany->teamMembersArray[i], fp);
    }
    return 1;
}

void writeTeamMemberToBinaryFile(const TeamMember * pTeamMember, FILE* fp){
    // Write the ID
    if (fwrite(&pTeamMember->id, sizeof(pTeamMember->id), 1, fp) != 1) {
        printf("Failed!\n");
        return ;
    }

    // Write the Contact information
    writeContactToBinaryFile(&pTeamMember->info,fp);

    // Write the specialization
    if ( fwrite(&pTeamMember->speciality, sizeof(pTeamMember->speciality), 1, fp)!= 1) {
        printf("Failed!\n");
        return ;
    }

    // Write the experience
    if (fwrite(&pTeamMember->experience, sizeof(pTeamMember->experience), 1, fp) != 1) {
        printf("Failed!\n");
        return ;
    }
    


}

void writeProjectToBinaryFile(const Project* pProject, FILE* fp){

    // Write the boolean zoomMeeting
    if (fwrite(&pProject->zoomMeeting, sizeof(pProject->zoomMeeting), 1, fp)!= 1) {
        printf("Failed!\n");
        return;
    }

    // Write name length and name
    writeStringToBinaryFile(fp,pProject->name);


    // Write the number of websiteReq and then each websiteReq
    writeWebsiteRequirements(pProject->websiteReq, pProject->reqNumber , fp);

    // Write the Client
    writeClientToBinaryFile(&pProject->client,fp);


    // Write the Date
    saveDateToFile(fp, &pProject->dueDate);

    // Write the projectStatus
    if (fwrite(&pProject->projectStatus, sizeof(pProject->projectStatus), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

    // Write the feedback

    int temp = -1;

    if (!pProject->feedback) {
        if (fwrite(&temp, sizeof(int), 1, fp)!= 1) {
            printf("Failed!\n");
            return;
        }
    }
    if (pProject->projectStatus == 2 && pProject->feedback) {
        temp =1;
        if (fwrite(&temp, sizeof(int), 1, fp) != 1) {
            printf("Failed!\n");
            return;
        }
        if (fwrite(&pProject->feedback->rate, sizeof(pProject->feedback->rate), 1, fp) != 1) {
            printf("Failed!\n");
            return;
        }
        writeStringToBinaryFile(fp,pProject->feedback->feedbackStr);

    }

}



int writeProjectManagementsArrayToBinaryFile(const Company* pCompany, FILE* fp){

    if (fwrite(&pCompany->numOfProjects, sizeof(pCompany->numOfProjects), 1, fp) != 1) {
        printf("Failed!\n");
        return 0;
    }

    for (int i = 0; i < pCompany->numOfProjects; ++i) {
        writeProjectManagementToBinaryFile(pCompany->projectManagementsArray[i],fp);
    }
    return 1;
}
void writeProjectManagementToBinaryFile(const ProjectManagement * pProjectManagement, FILE* fp){
    writeProjectToBinaryFile(&pProjectManagement->project,fp);
    writePMTeamMembersIDArrayToBinaryFile(pProjectManagement,fp);
    writePMTasksToBinaryFile(pProjectManagement,fp);
    savePaymentToBinaryFile(fp,&pProjectManagement->payment);
    //writePaymentToBinaryFile(&pProjectManagement->payment,fp);
}


//BinaryHelper
void writeStringToBinaryFile(FILE* file, const char* string) {
    if (string != NULL) {
        int length = strlen(string) + 1;
        fwrite(&length, sizeof(length), 1, file);
        fwrite(string, sizeof(char), length, file);
    } else {
        int length = 0;
        fwrite(&length, sizeof(length), 1, file);
    }

}

void writeDateToBinaryFile(const Date *date, FILE *fp) {
    fwrite(&date->day, sizeof(date->day), 1, fp);
    fwrite(&date->month, sizeof(date->month), 1, fp);
    fwrite(&date->year, sizeof(date->year), 1, fp);
}

void writeWebsiteRequirements(char **websiteReq, int reqNumber,FILE *file) {
    // Write the number of website requirements

    if (fwrite(&reqNumber, sizeof(reqNumber), 1, file)!= 1) {
        printf("Failed!\n");
        return 0;
    }
    

    // Write each website requirement
    for (int i = 0; i < reqNumber; ++i) {
        // Write the length of the requirement string (including null terminator)
        int reqLength = strlen(websiteReq[i]) + 1;

        if (fwrite(&reqLength, sizeof(reqLength), 1, file) != 1) {
            printf("Failed!\n");
            return;
        }

        // Write the requirement string
        if (fwrite(websiteReq[i], sizeof(char), reqLength, file) != reqLength) {
            printf("Failed!\n");
            return ;
        }
        
    }
}

void writePMTeamMembersIDArrayToBinaryFile(const ProjectManagement* pProjectManagement, FILE* fp){
    fwrite(&pProjectManagement->numberOfTeamMembers, sizeof(int), 1, fp);
    // Write each team member's ID
    for (int i = 0; i < pProjectManagement->numberOfTeamMembers; i++) {

        if (fwrite(&pProjectManagement->projectTeamMembersArray[i]->id, sizeof(int), 1, fp) != 1) {
            printf("Failed!\n");
            return;
        }
        
    }

}

void writePMTasksToBinaryFile(const ProjectManagement* pProjectManagement, FILE* fp){

// Write the number of tasks
    if (fwrite(&pProjectManagement->numberOfTasks, sizeof(int), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }
    

    // Write each task to the file
    const NODE* pNode = &pProjectManagement->projectManagementTasksLists.head;
    Task* tempTask;
    pNode = pNode->next;
    for (int i = 0; i <  pProjectManagement->numberOfTasks; ++i) {
        tempTask = pNode->key;
        writePMTaskToBinaryFile(tempTask,fp);
        pNode = pNode->next;
    }

}

void writePMTaskToBinaryFile(const Task* pTask, FILE* fp){
    TeamMember* pTeamMember = (TeamMember *) pTask->teamMember;
    // Write the task ID
    if (fwrite(&pTask->id, sizeof(pTask->id), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

    // Write the length of the task string and the task string itself
    writeStringToBinaryFile(fp,pTask->taskStr);

    // Write the task status
    if (fwrite(&pTask->taskStatus, sizeof(pTask->taskStatus), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

    // Write the pointer address of the team member
    if (fwrite(&pTeamMember->id, sizeof(int), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

}

void writePaymentToBinaryFile(const Payment *pPayment, FILE *fp){

    if (fwrite(&pPayment->id, sizeof(pPayment->id), 1, fp) != 1) {
        printf("Failed!\n");
        return;
    }

    if (fwrite(&pPayment->tasksNumber, sizeof(pPayment->tasksNumber), 1, fp)!= 1) {
        printf("Failed!\n");
        return;
    }
    if ( fwrite(&pPayment->total, sizeof(pPayment->total), 1, fp)!= 1) {
        printf("Failed!\n");
        return;
    }
    if (fwrite(&pPayment->status, sizeof(pPayment->status), 1, fp)!= 1) {
        printf("Failed!\n");
        return;
    }
    if (fwrite(&pPayment->NofPayments, sizeof(pPayment->NofPayments), 1, fp)!= 1) {
        printf("Failed!\n");
        return;
    }
    
    if(pPayment->status == 1){
        saveDateToFile(fp ,&pPayment->paymentDate);
    }
}


//READ BINARYFILE
int readCompanyFromBinaryFile( Company* pCompany, const char* fileName){
    FILE *fp = fopen(fileName, "rb");
    if (!fp) {
        printf("Failed to open file\n");
        initCompany(pCompany);
        return 0;
    }
    if(!readContactFromBinaryFile(&pCompany->info,fp)){
        printf("Failed to read file.\n");
        fclose(fp);
        return 0;
    }
    if(!readClientsArrayFromBinaryFile(pCompany,fp)){
        printf("Failed to read file.\n");
        fclose(fp);
        return 0;
    }
     
    if (fread(&pCompany->sort, sizeof(int), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    if(!readCandidatesArrayFromBinaryFile(pCompany,fp)){
        printf("Failed to read file.\n");
        fclose(fp);
        return 0;
    }
    if(!readTeamMembersArrayFromBinaryFile(pCompany,fp)){
        printf("Failed to read file.\n");
        fclose(fp);
        return 0;
    }
    if(!readProjectManagementsArrayFromBinaryFile(pCompany,fp)){
        printf("Failed to read file.\n");
        fclose(fp);
        return 0;
    }

    if (fread(&pCompany->companyEarning, sizeof(pCompany->companyEarning), 1, fp) != 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    fclose(fp);
    return 1;
}


int readContactFromBinaryFile(Contact* pContact, FILE* fp){
    // Read the length of the name string
    int nameLength;
    if ( fread(&nameLength, sizeof(nameLength), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    // Allocate memory for the name string and read it from the file
    pContact->name =(char*) malloc(nameLength * sizeof(char));
    if(!pContact->name)
    {
        printf("Memory Allocation Failed!\n");
        return 0 ;
    }
    fread(pContact->name, sizeof(char), nameLength, fp);

    // Read the phone number directly from the file
    fread(pContact->phoneNumber, sizeof(char), LengthOfPhoneNum + 1, fp);

    // Read the length of the email string
    int emailLength;
    if (fread(&emailLength, sizeof(emailLength), 1, fp) != 1) {
        return 0;
    }
    

    // Allocate memory for the email string and read it from the file
    pContact->email = malloc(emailLength * sizeof(char));
    if(!pContact->email)
    {
        printf("Memory Allocation Failed!\n");
        free(pContact->name);
        return 0;
    }
    fread(pContact->email, sizeof(char), emailLength, fp);

    return 1;
}

int readClientsArrayFromBinaryFile(Company* pCompany, FILE* fp){

    if (fread(&pCompany->numOfClients, sizeof(pCompany->numOfClients), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    pCompany->clientsArray = malloc(sizeof (Client) * pCompany->numOfClients);
    if(!pCompany->clientsArray)
    {
        printf("Memory Allocation Failed!\n");
        return 0;
    }
    int maxID = 0;
    for (int i = 0; i < pCompany->numOfClients; ++i) {
        readClientFromBinaryFile(&pCompany->clientsArray[i], fp);
        if(pCompany->clientsArray[i].id > maxID){
            maxID = pCompany->clientsArray[i].id;
        }
    }
    addToClientID(maxID);
    return 1;
}

int readClientFromBinaryFile(Client* pClient, FILE* fp){
    // Read the client ID
    if (fread(&pClient->id, sizeof(pClient->id), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    
    // Read the Contact struct from the file
    readContactFromBinaryFile(&pClient->info, fp);

    return 1;
}


int readCandidatesArrayFromBinaryFile(Company* pCompany, FILE* fp){

    if (fread(&pCompany->numOfCandidates, sizeof(int), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    

    pCompany->candidatesArray = (Candidate**)malloc(pCompany->numOfCandidates * sizeof(Candidate*));
    if (!pCompany->candidatesArray) {
        printf("Memory allocation failed\n");
        return 0;
    }

    for (int i = 0; i < pCompany->numOfCandidates; ++i) {
        pCompany->candidatesArray[i] = (Candidate*)malloc(sizeof(Candidate)); // Allocate memory for the candidate struct
        readCandidateFromBinaryFile(pCompany->candidatesArray[i], fp);
    }


    return 1;
}

int readCandidateFromBinaryFile(Candidate * pCandidate, FILE* fp){

    // Read Contact
    readContactFromBinaryFile(&pCandidate->info,fp);

    // Read specialization
    if (fread(&pCandidate->speciality, sizeof(pCandidate->speciality), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    

    // Read experience
    if (fread(&pCandidate->experience, sizeof(pCandidate->experience), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
   

    return 1;
}

int readTeamMembersArrayFromBinaryFile(Company* pCompany, FILE* fp){
    // Read the number of team members
    if (fread(&pCompany->numOfTeamMembers, sizeof(int), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    
    // Allocate memory for the array of pointers to team members
    pCompany->teamMembersArray = (TeamMember**)malloc(pCompany->numOfTeamMembers * sizeof(TeamMember*));
    if (!pCompany->teamMembersArray) {
        perror("Memory allocation failed");
        return 0;
    }
    int maxID = 0;
    // Read each team member from the file
    for (int i = 0; i < pCompany->numOfTeamMembers; ++i) {
        pCompany->teamMembersArray[i] = (TeamMember*)malloc(sizeof(TeamMember)); // Allocate memory for the team member struct
        readTeamMemberFromBinaryFile(pCompany->teamMembersArray[i], fp);
        if(pCompany->teamMembersArray[i] != NULL && pCompany->teamMembersArray[i]->id > maxID){
            maxID = pCompany->teamMembersArray[i]->id;
        }
    }
    addToTeamMemberID(maxID);

    return 1;
}
int readTeamMemberFromBinaryFile(TeamMember* pTeamMember, FILE* fp){
    pTeamMember->numberOfTasks = 0;
    // Read id
    if (fread(&pTeamMember->id, sizeof(pTeamMember->id), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    

    // Read Contact
    readContactFromBinaryFile(&pTeamMember->info,fp);

    // Read specialization
    if (fread(&pTeamMember->speciality, sizeof(pTeamMember->speciality), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    // Read experience
    if (fread(&pTeamMember->experience, sizeof(pTeamMember->experience), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    
    L_init(&pTeamMember->openTasksList);
    L_init(&pTeamMember->closedTasksList);

    return 1;
}


int readProjectManagementsArrayFromBinaryFile(Company* pCompany, FILE* fp){
    ProjectManagement* temp;
    if (fread(&pCompany->numOfProjects, sizeof(int), 1, fp) != 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    int projectsNumber = pCompany->numOfProjects;
    pCompany->projectManagementsArray = (ProjectManagement **)malloc( projectsNumber * sizeof(ProjectManagement*));
    if (pCompany->projectManagementsArray == NULL) {
        return 0; // Or handle the error appropriately
    }

    for (int i = 0; i < projectsNumber; i++) {
        temp = (ProjectManagement*)calloc(1,sizeof(ProjectManagement));
        if (!temp)
            return 0;
        if (!readProjectManagementFromBinaryFile(temp, pCompany->teamMembersArray, pCompany->numOfTeamMembers ,  fp))
        {
            freeProjectManagementArrayFileVersion(pCompany, i);
            free(temp);
            return 0;
        }
      addProjectManagementFromFile(pCompany,temp,i);
        }
    return 1;
}
int readProjectManagementFromBinaryFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp){
    if(!readProjectFromBinaryFile(&pProjectManagement->project,fp)){
        printf("Failed to read file.\n");
        return 0;
    }
    if(!readPMTeamMembersIDArrayFromBinaryFile(pProjectManagement,teamMemberArr,amount,fp)){
        printf("Failed to read file.\n");
        return 0;
    }
    if(!readTasksArrayFromBinaryFile(pProjectManagement,fp)){
        printf("Failed to read file.\n");
        return 0;
    }

    if (!readPaymentToBinaryFile(fp, &pProjectManagement->payment)) {
        printf("Failed to read file.\n");
        return 0;
    }

   /* if(!readPaymentFromBinaryFile(&pProjectManagement->payment,fp)){
        printf("Failed to read file.\n");
        return 0;
    }*/



    return 1;
}

int readProjectFromBinaryFile(Project * pProject, FILE* fp){

    if (fread(&pProject->zoomMeeting, sizeof(pProject->zoomMeeting), 1, fp) != 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    pProject->name = readStringFromFile(fp);

    if(!pProject->name){
        return 0;
    }

    readWebsiteReqBinary(pProject,fp);

    readClientFromBinaryFile(&pProject->client,fp);

    readDateFromBinaryFile(&pProject->dueDate,fp);

    
    if (fread(&pProject->projectStatus, sizeof(int), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }

    int isFeedback = 0;

    if (fread(&isFeedback, sizeof(int), 1, fp) != 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    
    pProject->feedback = NULL;

    if(isFeedback == 1){
        pProject->feedback = (Feedback*)malloc(sizeof(Feedback));
        if (!pProject->feedback)
        {
            printf("Memory Allocation failed.\n");
            return 0;
        }

        fread(&pProject->feedback->rate, sizeof(int), 1, fp);
        pProject->feedback->feedbackStr = readStringFromFile(fp);
    }
    return 1;
}


//BinaryRead Helper
char* readStringFromFile(FILE *file) {
    int length;
    fread(&length, sizeof(length), 1, file);
    char *str = (char*)calloc(length ,sizeof(char));
    if (str != NULL) {
        fread(str, sizeof(char), length, file);
    }
    return str;
}

int readWebsiteReqBinary(Project *pProject, FILE *fp) {

    if (fread(&pProject->reqNumber, sizeof(pProject->reqNumber), 1, fp)!= 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    

    // Allocate memory for the website requirements array
    pProject->websiteReq = malloc(pProject->reqNumber * sizeof(char *));
    if (!pProject->websiteReq) {
        printf("Memory allocation failed");
        return 0;
    }

    // Read each website requirement from the binary file
    for (int i = 0; i < pProject->reqNumber; i++) {
        // Read the string from the file using readStringFromFile function
        pProject->websiteReq[i] = readStringFromFile(fp);
        if (!pProject->websiteReq[i]) {
            printf("Failed to read requirement string");
            return 0;
        }
    }

    return 1;
}

int readPMTeamMembersIDArrayFromBinaryFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp) {
    // Read the number of team members
    if (fread(&pProjectManagement->numberOfTeamMembers, sizeof(int), 1, fp) != 1) {
        printf("Failed to read file.\n");
        return 0;
    }
    

    if (pProjectManagement->numberOfTeamMembers == 0)
        return 1;

    // Allocate memory for the team members array
    pProjectManagement->projectTeamMembersArray = (TeamMember**)malloc(pProjectManagement->numberOfTeamMembers * sizeof(TeamMember *));
    if (!pProjectManagement->projectTeamMembersArray) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int id;

    // Read each team member's ID
    for (int i = 0; i < pProjectManagement->numberOfTeamMembers; i++) {

        if (fread(&id, sizeof(int), 1, fp)!= 1) {
            printf("Failed to read file.\n");
            return 0;
        }
    
        // Allocate memory for the TeamMember struct
        pProjectManagement->projectTeamMembersArray[i] = findTeamMemberWithTheSameIdInCompany(teamMemberArr,amount,id);
        if (!pProjectManagement->projectTeamMembersArray[i]) {
            printf("Memory allocation failed\n");
            return 0;
        }
    }

    return 1;
}


int readTasksArrayFromBinaryFile(ProjectManagement* pProjectManagement, FILE* fp){

    pProjectManagement->numberOfTasks = 0;
    int numOfTasks;
    if(fread(&numOfTasks, sizeof(int), 1, fp) != 1){
        return 0;
    }

    Task* temp;
    int maxID = 0;

    L_init(&pProjectManagement->projectManagementTasksLists);

    for (int i = 0; i < numOfTasks; i++)
    {
        temp = (Task*)calloc(1,sizeof(Task));
        if (!temp)
        {
            return 0;
        }
        if (!readyTaskFromBinaryFile(pProjectManagement,temp, fp))
        {
            //NOTE
            L_free(&pProjectManagement->projectManagementTasksLists, freeTask);
            free(temp);
            return 0;
        }
        if(temp->id > maxID){
            maxID = temp->id;
        }

    }
    addToTaskID(maxID);
    return 1;
}
int readyTaskFromBinaryFile(ProjectManagement* pProjectManagement,Task* pTask,FILE* fp){
    if (pTask == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }

    // Read the task ID
    if (fread(&pTask->id, sizeof(pTask->id), 1, fp) != 1) {
        printf("Error reading task ID from file\n");
        free(pTask);
        return 0;
    }

    // Read the task string
    pTask->taskStr = readStringFromFile(fp);
    if (pTask->taskStr == NULL) {
        printf("Error reading task string from file\n");
        free(pTask);
        return 0;
    }
    // Read the task status
    if (fread(&pTask->taskStatus, sizeof(pTask->taskStatus), 1, fp) != 1) {
        printf("Error reading task status from file\n");
        free(pTask->taskStr);
        free(pTask);
        return 0;
    }
//    // Read the team member ID
    int id;
    if(fread(&id, sizeof(int), 1, fp)!=1)
        return 0;
    TeamMember* teamMember= findTeamMemberWithTheSameId(pProjectManagement, id);
    if (teamMember == NULL) {
        printf("asdas");
    }
    pTask->teamMember = (struct TeamMember *) teamMember;
    if (pTask->teamMember == NULL) {
        printf("Allocation Failed");
        return 0;
    }
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

int readPaymentFromBinaryFile(Payment * pPayment, FILE* fp){

    if (fread(&pPayment->id, sizeof(pPayment->id), 1, fp) != 1)
        return 0;

    if (fread(&pPayment->tasksNumber, sizeof(pPayment->tasksNumber), 1, fp) != 1)
        return 0;

    if (fread(&pPayment->total, sizeof(pPayment->total), 1, fp) != 1)
        return 0;

    if (fread(&pPayment->status, sizeof(pPayment->status), 1, fp) != 1)
        return 0;

    if (fread(&pPayment->NofPayments, sizeof(pPayment->NofPayments), 1, fp) != 1)
        return 0;

    if (pPayment->status == 1) {
        if (!readDateFromBinaryFile(&pPayment->paymentDate, fp))
            return 0;
    }

    return 1;

}