#include <string.h>
#include "Company.h"
#include "Macros.h"


//-----menuAdd-----
typedef enum
{
    eAddClient,eAddCandidate, eAddTeamMember, eAddProject, eAddTeamMemberToProject,
    eAddFeedback,  eNofMenuAddOptions
} eMenuAddOptions;

const char* strAdd[eNofMenuAddOptions] = { "Add Client","Add Candidate","Add Team Member",
                                           "Add Project","Add Team Member To Project","Add Feedback"};

//---------MenuRemove--------
typedef enum
{
    eRemoveCandidate,eRemoveTeamMember, eRemoveProject,eNofMenuRemoveOptions
} eMenuRemoveOptions;

const char* strRemove[eNofMenuRemoveOptions] = { "Remove Candidate","Remove TeamMember","Remove Project"};

//-----menuPrint-----
typedef enum
{
    ePrintClients,ePrintTeamMembers, ePrintProjects, ePrintCandidates,eNofMenuPrintOptions
} eMenuPrintsOptions;

const char* strPrint[eNofMenuPrintOptions] = { "Print Clients","Print TeamMembers","Print Projects",
                                               "Print Candidates"};


//Functions
int initCompany(Company* pCompany){
    CHECK_RETURN_INT(pCompany,"Company was not found!");
    if (!initContact(&pCompany->info, "company")){
        return 0;
    }
    pCompany->projectManagementsArray = NULL;
    pCompany->teamMembersArray = NULL;
    pCompany->clientsArray = NULL;
    pCompany->candidatesArray = NULL;
    pCompany->sort= eNoSorted;
    pCompany->numOfProjects = 0;
    pCompany->numOfClients = 0;
    pCompany->numOfTeamMembers = 0;
    pCompany->numOfCandidates = 0;
    pCompany->companyEarning = 0;

// Check
//    pCompany->allContactsArray = NULL;

    return 1;
}

void printCompany(const Company* pCompany) {
    printf("Details of the %s company:\n", pCompany->info.name);
    printf("Contact information:\t Phone number: %s\t Email: %s\n", pCompany->info.phoneNumber, pCompany->info.email);
    printf("\nProjects:\n");
    if(pCompany->numOfProjects == 0){
        printf("There are no projects yet\n");
    } else{
        generalArrayFunctionWithNumbers(pCompany->projectManagementsArray, pCompany->numOfProjects, sizeof(Project*), printProjectManagement);
    }
    printf("\nTeamMembers:\n");
    if(pCompany->numOfTeamMembers == 0){
        printf("There are no team members yet\n");
    }else{
        generalArrayFunctionWithNumbers(pCompany->teamMembersArray, pCompany->numOfTeamMembers, sizeof(TeamMember*), printTeamMember);
    }
    printf("\nClients:\n");
    if(pCompany->numOfClients == 0){
        printf("There are no clients yet\n");
    }else{
        generalArrayFunctionWithNumbers(pCompany->clientsArray, pCompany->numOfClients, sizeof(Client), printClient);
    }
    printf("\nCOMPANY TOTAL INCOME: %d$\n", pCompany->companyEarning);
}

int addProjectManagement(Company* pCompany){
    ZERO_COUNTER(pCompany->numOfClients,"There are no clients yet, you need a client to open a project\n")
    ProjectManagement* pProjectManagement = (ProjectManagement*)calloc(1, sizeof(ProjectManagement));
    if (!pProjectManagement){
        printf("Memory Allocation Failed");
        return 0;
    }
    Client* projectClient = findClient(pCompany);
    int num = initProjectManagement(pProjectManagement,projectClient);
    if(num == -1){
        free(pProjectManagement);
        return -1;
    }else if(num == 0){
        free(pProjectManagement);
        return 0;
    }
    ProjectManagement** temp = (ProjectManagement**)realloc(pCompany->projectManagementsArray, (pCompany->numOfProjects + 1) * sizeof(ProjectManagement*));
    if(temp != NULL){
        pCompany->projectManagementsArray = temp;
    }
    else {
        printf("Memory Allocation Failed\n");
        return 0;
    }
    pCompany->projectManagementsArray[pCompany->numOfProjects] = pProjectManagement;
    pCompany->numOfProjects++;
    return 1;
}



//Helper Functions
Client* findClient(Company* pCompany) {
    
    if (pCompany->numOfClients == 0){
        printf("There is no clients in the company!\n");
        return NULL;
    }
    if(pCompany->numOfClients == 1){
        printf("%s is the only client, to you want him? 0-no, 1-yes",pCompany->clientsArray->info.name);
        CHOOSE(num ,0,1);
        if(num == 1){
            return pCompany->clientsArray;
        } else{
            return NULL;
        }
    }
    Client temp;
    Client* pFound;

    int (*compare)(const void *, const void *);

    if(pCompany->sort == eNoSorted){
        printf("\nThe search of clients for the company cannot be performed, array not sorted\n");
        return NULL;
    }

    generalArrayFunctionWithNumbers(pCompany->clientsArray,pCompany->numOfClients,sizeof (Client), printClient);

    if(pCompany->sort == eID){
        printf("type the ID of the client that want to open a project:\n");
        int ignore = scanf("%d",&temp.id);
        compare = &compareClientsByID;
    } else if(pCompany->sort == eName){
        temp.info.name = getStrExactName("Type Name of the client that want to open a project:");
        compare = &compareClientsByName;
    }
    else{
        char* tempChar = getStrExactName("Type the Phone Number of the client that want to open a project:");
        strcpy(temp.info.phoneNumber,tempChar);
        compare = &compareClientsByPhoneNumber;
    }
    pFound = (Client*)bsearch(&temp, pCompany->clientsArray, pCompany->numOfClients, sizeof(Client), compare);
    if(pFound == NULL) {
        printf("Client was not found\n");
        return NULL;
    }

    return pFound;

}

void sortClient(Company* pCompany) {
    eSort temp = getSort();
    int sortChoice = (int) temp;
    int (*compare)(const void *, const void *);

    if (sortChoice == 0)
        compare = &compareClientsByID;
    else if(sortChoice == 1){
        compare = &compareClientsByName;
    } else{
        compare = &compareClientsByPhoneNumber;
    }

    qsort(pCompany->clientsArray, pCompany->numOfClients, sizeof(Client), compare);
    pCompany->sort = temp;

}

eSort getSort()
{
    
    int sortOptions = eNofOptions-1;

    printf("By what property do you want to find the client with?\n");
    for (int i = 0; i < sortOptions; ++i) {
        printf("%d) - %s\n" , i+1 , enumSort[i]);
    }
    CHOOSE( sort, 1,sortOptions);
    sort--;
    int ignore = getchar();
    return (eSort) sort;

}


int addTeamMemberToArray(Company* pCompany , Candidate* pCandidate){
    TeamMember* pTeamMember = (TeamMember*)calloc(1, sizeof(TeamMember));
    CHECK_RETURN_INT(pTeamMember,"Memory Allocation Failed")
    turnToTeamMember(pTeamMember,pCandidate);

    TeamMember** temp = (TeamMember**)realloc(pCompany->teamMembersArray, (pCompany->numOfTeamMembers + 1) * sizeof(TeamMember*));
    if (temp != NULL) {
        pCompany->teamMembersArray = temp;
    }
    CHECK_RETURN_INT(pCompany->teamMembersArray, "Memory Allocation Failed")
    pCompany->teamMembersArray[pCompany->numOfTeamMembers] = pTeamMember;
    pCompany->numOfTeamMembers++;

    return 1;
}

int removeProjectManagement(Company* pCompany){
    if(pCompany->numOfProjects == 0){
        printf("There aren't projects in the system\n");
        return 0;
    }

    printf("Select a project to delete from the system:\n");
    generalArrayFunctionWithNumbers(pCompany->projectManagementsArray, pCompany->numOfProjects, sizeof(ProjectManagement*), printProjectManagement);
    CHOOSE(num,1, pCompany->numOfProjects);

    ProjectManagement* pProjectManagement = pCompany->projectManagementsArray[num -1];
    removeTeamMembersFromProjectManagement(pProjectManagement);
    removeProjectManagementHelper(pCompany, num -1 );
    freeProjectManagement(pProjectManagement);

    if (pCompany->numOfProjects - 1 == 0) {
        free(pCompany->projectManagementsArray);
        pCompany->projectManagementsArray = NULL;
    }
    else {
        ProjectManagement** temp = (ProjectManagement **)realloc(pCompany->projectManagementsArray, (pCompany->numOfProjects - 1) * sizeof(ProjectManagement*));
            if (temp != NULL) {
                pCompany->projectManagementsArray = temp;
            }
            else {
                printf("Memory allocation failed.\n");
            }
    }
    pCompany->numOfProjects--;

    return 1;
}


void removeProjectManagementHelper(Company* pCompany, int num){
    if(num + 1 != pCompany->numOfProjects){
        for (int i = num; i < pCompany->numOfProjects; i++) {
            pCompany->projectManagementsArray[i] = pCompany->projectManagementsArray[i+1];
        }
    }
}

void removeTeamMembersFromProjectManagement(ProjectManagement* pProjectManagement){
    int numberOfTeamMembersStart= pProjectManagement->numberOfTeamMembers;
    for (int i = 0; i < numberOfTeamMembersStart; i++) {
        removeTeamMemberFromProjectManagementHelper(pProjectManagement, 0);
    }
}


int removeTeamMemberFromArray(Company* pCompany){
    ZERO_COUNTER(pCompany->numOfTeamMembers, "There aren't team member in the system")

    printf("Select a team member to delete from the system:\n");
    generalArrayFunctionWithNumbers(pCompany->teamMembersArray, pCompany->numOfTeamMembers, sizeof(TeamMember*), printTeamMember);
    CHOOSE(num,1, pCompany->numOfTeamMembers);

    TeamMember* teamMember = pCompany->teamMembersArray[num -1];
    removeTeamMemberFromArrayInAllProjectManagement(pCompany, pCompany->teamMembersArray[num -1]);
    deleteTeamMemberFromArrayInCompany(pCompany, num - 1);

    freeTeamMember(teamMember);

    return 1;
}

int deleteTeamMemberFromArrayInCompany(Company* pCompany, int num){
    if(num + 1 != pCompany->numOfTeamMembers){
        for (int i = num; i < pCompany->numOfTeamMembers; i++) {
            pCompany->teamMembersArray[i] = pCompany->teamMembersArray[i+1];
        }
    }
    if (pCompany->numOfTeamMembers - 1 == 0) {
        free(pCompany->teamMembersArray);
        pCompany->teamMembersArray = NULL;
    }
    else {
        TeamMember** temp = (TeamMember**)realloc(pCompany->teamMembersArray, (pCompany->numOfTeamMembers - 1) * sizeof(TeamMember*));
        if (temp != NULL) {
            pCompany->teamMembersArray = temp;
        }
    }
    pCompany->numOfTeamMembers--;
    return 1;
}

void removeTeamMemberFromArrayInAllProjectManagement(Company* pCompany, TeamMember* pTeamMember){
    for (int i = 0; i < pCompany->numOfProjects; i++) {
        findTeamMemberPlacementAndDelete(pCompany->projectManagementsArray[i], pTeamMember);
    }
}

void findTeamMemberPlacementAndDelete(ProjectManagement * pProjectManagement, TeamMember* pTeamMember){
    if(pProjectManagement->numberOfTeamMembers == 0){
        return;
    }
    for (int i = 0; i < pProjectManagement->numberOfTeamMembers; i++) {
        if(sameTeamMemberID(pProjectManagement->projectTeamMembersArray[i], pTeamMember ) == 0){
            removeTeamMemberFromProjectManagementHelper(pProjectManagement, i);
        }
    }
}


int removeCandidate(Company* pCompany){
    CHECK_RETURN_INT(pCompany,"Project Management was not found!")
    ZERO_COUNTER(pCompany->numOfCandidates ,"There are no candidates in the system")


    printf("Which candidate do you want to remove?\n");
    for (int i = 0; i < pCompany->numOfCandidates; i++) {
        printf("%d) ", i+1);
        printCandidate(&pCompany->candidatesArray[i]);
    }
    CHOOSE(num,1,pCompany->numOfCandidates);
    deleteCandidateInArray(pCompany,num-1);
    return 1;
}

int addCandidate(Company* pCompany){

    Candidate* pCandidate = (Candidate*)calloc(1, sizeof(Candidate));
    CHECK_RETURN_INT(pCandidate,"Memory Allocation Failed")
    initCandidate(pCandidate); 
    Candidate** temp = (Candidate**)realloc(pCompany->candidatesArray, (pCompany->numOfCandidates + 1) * sizeof(Candidate*));
    if (temp != NULL) {
        pCompany->candidatesArray = temp;
    }
    CHECK_RETURN_INT(pCompany->candidatesArray,"Memory Allocation Failed")
    pCompany->candidatesArray[pCompany->numOfCandidates] = pCandidate;
    pCompany->numOfCandidates++;
    return 1;
}

int candidateApproval(const Candidate* pCandidate){
    CHECK_RETURN_INT(pCandidate,"Memory Allocation Failed")
    printCandidate(&pCandidate);

    printf("Do you accept this candidate for now to be part of the company? (1-Yes , 0-No)");
   CHOOSE(num,0,1);

    return num;
}


int deleteCandidateInArray(Company* pCompany, int num){
    Candidate* pTempCandidate = pCompany->candidatesArray[num];
    if(num + 1 != pCompany->numOfCandidates){
        for (int i = num; i < pCompany->numOfCandidates; i++) {
            pCompany->candidatesArray[i] = pCompany->candidatesArray[i+1];
        }
    }
    if (pCompany->numOfCandidates - 1 == 0) {
        free(pCompany->candidatesArray);
        pCompany->candidatesArray = NULL;
    }
    else {
        Candidate** temp = (Candidate**)realloc(pCompany->candidatesArray, (pCompany->numOfCandidates - 1) * sizeof(Candidate*));
        if (temp != NULL) {
            pCompany->candidatesArray = temp;
        }
        else {
            printf("Allocation failed");
            return 0;
        }
    }
    pCompany->numOfCandidates--;
    return 1;
}


int addClientToArray(Company* pCompany){
    Client* temp = (Client*)realloc(pCompany->clientsArray, (pCompany->numOfClients + 1) * sizeof(Client));
    if (temp != NULL) {
        pCompany->clientsArray = temp;
    }
    CHECK_RETURN_INT(pCompany->clientsArray,"Memory Allocation Failed")
    initClient(&pCompany->clientsArray[pCompany->numOfClients]);
    pCompany->numOfClients++;
    return 1;
}



void freeCompany(Company* pCompany){
    freeContact(&pCompany->info);
    freeTeamMembersArray(pCompany);
    freeCandidatesArray(pCompany);
    freeProjectManagementsArray(pCompany);
    freeClients(pCompany);
}

void freeClients(Company* pCompany){
    free(pCompany->clientsArray);
}

void freeTeamMembersArray(Company* pCompany){
    for (int i = 0; i < pCompany->numOfTeamMembers; i++) {
        freeTeamMember(pCompany->teamMembersArray[i]);
    }
    free(pCompany->teamMembersArray);
}


void freeCandidatesArray(Company* pCompany){
    for (int i = 0; i < pCompany->numOfCandidates; i++) {
        freeCandidate(pCompany->candidatesArray[i]);
    }

   free(pCompany->candidatesArray);
}

void freeProjectManagementsArray(Company* pCompany){
    for (int i = 0; i < pCompany->numOfProjects; i++) {
        freeProjectManagement(pCompany->projectManagementsArray[i]);
    }

        free(pCompany->projectManagementsArray);
}


void addObjectByUserPreference(Company* pCompany){
    int option;
    int stop = 0;
    int choose = -1;
    Candidate* candidate;
    ProjectManagement* PM;
    TeamMember * TM;

    option = menuAddCompany();
    switch (option) {
        case eAddClient:
            addClientToArray(pCompany);
            break;
        case eAddCandidate:
            addCandidate(pCompany);
            break;
        case eAddTeamMember:
            candidate = pickCandidate(pCompany,&choose);
            if(!candidate){
                break;
            }
            if(candidateApproval(candidate) == 1){
                addTeamMemberToArray(pCompany , candidate);
                deleteCandidateInArray(pCompany,choose);
            }
            else
                printf("Candidate is not approved for now..\n");
            break;
        case eAddProject:
            addProjectManagement(pCompany);
            break;

        case eAddTeamMemberToProject:
        BREAK_IF_0(pCompany->numOfProjects, "There are no projects yet\n")
            BREAK_IF_0(pCompany->numOfTeamMembers, "There are no Team Members yet\n")
            PM = pickProjectManagement(pCompany);
            TM = pickTeamMember(pCompany);
            addTeamMemberToProjectManagement(PM,TM);
            break;

        case eAddFeedback:
            PM = pickProjectManagement(pCompany);
            addFeedback(&PM->project);
            break;

        case EXIT:
            stop = 1;
            break;

        default:
            printf("Invalid option\n");
            break;
    }

}


Candidate* pickCandidate(const Company* pCompany ,int* choose){
    ZERO_COUNTER(pCompany->numOfCandidates , "You dont have any candidates to transfer into Team Member.\n")

    printf("Please Pick a Candidate by Serial Number:\n");
    generalArrayFunctionWithNumbers(pCompany->candidatesArray,pCompany->numOfCandidates , sizeof (Candidate*) , printCandidate);
    CHOOSE(num,1, pCompany->numOfCandidates);
    num--;
    *choose = num;
    return pCompany->candidatesArray[num];
}

ProjectManagement* pickProjectManagement(const Company* pCompany){
    if(pCompany->numOfProjects == 0){
        printf("You dont have any projects in company...\n");
        return NULL;
    }

    printf("Please Pick a Project by Serial Number:\n");
    generalArrayFunctionWithNumbers(pCompany->projectManagementsArray,pCompany->numOfProjects , sizeof (ProjectManagement*) , printProjectManagementShort);
    CHOOSE(num,1, pCompany->numOfProjects);
    num--;

    return pCompany->projectManagementsArray[num];

}

TeamMember* pickTeamMember(const Company* pCompany){
    ZERO_COUNTER(pCompany->numOfTeamMembers , "You dont have any team members in company...\n")

    printf("Please Pick a Team Member by Serial Number:\n");
    generalArrayFunctionWithNumbers(pCompany->teamMembersArray,pCompany->numOfTeamMembers , sizeof (TeamMember*) , printTeamMember);
    CHOOSE(num,1, pCompany->numOfTeamMembers);
    num--;

    return pCompany->teamMembersArray[num];
}



int menuAddCompany(){
    int option;
    printf("\n\n");
    printf("Please choose one of the following options\n");
    for(int i = 0 ; i < eNofMenuAddOptions ; i++)
        printf("%d - %s\n",i,strAdd[i]);
    printf("%d - Cancel adding\n", EXIT);
    int ignore = scanf("%d", &option);

    //clean buffer
    char tav;
    ignore = scanf("%c", &tav);
    return option;
}

void removeObjectByUserPreference(Company* pCompany){
    int option;
    int stop = 0;

    option = menuRemoveCompany();
    switch (option) {
        case eRemoveCandidate:
            removeCandidate(pCompany);
            break;
        case eRemoveTeamMember:
            removeTeamMemberFromArray(pCompany);
            break;
        case eRemoveProject:
            removeProjectManagement(pCompany);
            break;

        case EXIT:
            stop = 1;
            break;

        default:
            printf("Invalid option\n");
            break;
    }

}

int menuRemoveCompany(){
    int option;
    printf("\n\n");
    printf("Please choose one of the following options\n");
    for(int i = 0 ; i < eNofMenuRemoveOptions ; i++)
        printf("%d - %s\n",i,strRemove[i]);
    printf("%d - Cancel Removing\n", EXIT);
    int ignore = scanf("%d", &option);

    //clean buffer
    char tav;
    int ingore = scanf("%c", &tav);
    return option;
}

void PrintObjectByUserPreference(Company* pCompany){
    int option;
    int stop = 0;

    option = menuPrintCompany();

    switch (option) {
        case ePrintClients:
        BREAK_IF_0(pCompany->numOfClients, "There are no clients yet");
            generalArrayFunctionWithNumbers(pCompany->clientsArray,pCompany->numOfClients,sizeof (Client), printClient);
            break;
        case ePrintTeamMembers:
        BREAK_IF_0(pCompany->numOfTeamMembers, "There are no team members yet");
            generalArrayFunctionWithNumbers(pCompany->teamMembersArray,pCompany->numOfTeamMembers,sizeof (TeamMember*), printTeamMember);
            break;
        case ePrintProjects:
        BREAK_IF_0(pCompany->numOfProjects, "There are no projects yet");
            generalArrayFunctionWithNumbers(pCompany->projectManagementsArray,pCompany->numOfProjects,sizeof (ProjectManagement*), printProjectManagement);
            break;
        case ePrintCandidates:
        BREAK_IF_0(pCompany->numOfCandidates, "There are no Candidates yet");
            generalArrayFunctionWithNumbers(pCompany->candidatesArray,pCompany->numOfCandidates,sizeof (Candidate*), printCandidate);
            break;

        case EXIT:
            stop = 1;
            break;

        default:
            printf("Invalid option\n");
            break;
    }

}

int menuPrintCompany(){
    int option;
    printf("\n\n");
    printf("Please choose one of the following options\n");
    for(int i = 0 ; i < eNofMenuPrintOptions ; i++)
        printf("%d - %s\n",i,strPrint[i]);
    printf("%d - Cancel printing\n", EXIT);
    int ignore = scanf("%d", &option);

    //clean buffer
    char tav;
    ignore = scanf("%c", &tav);
    return option;
}

int payProjectPayments(Company* pCompany){
    ZERO_COUNTER(pCompany->numOfProjects,"There are no projects yet to pay for them\n")
    ProjectManagement* PM = pickProjectManagement(pCompany);
    pCompany->companyEarning +=  payPayment(&PM->payment);

    return 1;
}

TeamMember* findTeamMemberWithTheSameIdInCompany(TeamMember ** TeamMemberArray, int amount , int numIdTeamMember){
    for (int i = 0; i < amount; i++) {
        if(TeamMemberArray[i]->id == numIdTeamMember){
            return TeamMemberArray[i];
        }
    }
    return NULL;
}

int addProjectManagementFromFile(Company* pCompany , ProjectManagement * projectManagement , int num){
    pCompany->projectManagementsArray[num] = projectManagement;
    return 1;
}

void freeProjectManagementArrayFileVersion(Company* pCompany, int num) {
    for (int i = 0; i < num; i++) {
        freeProjectManagement(pCompany->projectManagementsArray[i]);
    }
    free(pCompany->projectManagementsArray);
}