#ifndef WEBSITE_MANAGER_COMPANY_H
#define WEBSITE_MANAGER_COMPANY_H

#include "ProjectManagement.h"
#include "Candidate.h"



//-----Sort-----
typedef enum {eID, eName, ePhoneNumber, eNoSorted, eNofOptions} eSort;
static const char* enumSort[eNofOptions]= { "ID", "Name","Phone Number" ,"Not Sorted" };


//-------Company Struct-----
typedef struct
{
    Contact info;
    ProjectManagement** projectManagementsArray;
    TeamMember** teamMembersArray;
    Client* clientsArray;
    Candidate** candidatesArray;
    eSort sort;
    int numOfProjects;
    int numOfClients;
    int numOfTeamMembers;
    int numOfCandidates;
    int companyEarning;

    //Check
    //Contact* allContactsArray;
}Company;
//-----------------------------

//Check



//Functions
int         initCompany(Company* pCompany);
int         initCompanyFromFile();
void        printCompany(const Company* pCompany);
int         addProjectManagement(Company* pCompany);
int         removeProjectManagement(Company* pCompany);
void        removeProjectManagementHelper(Company* pCompany, int num);
int         addTeamMemberToArray(Company* pCompany , Candidate* pCandidate);
void        removeTeamMembersFromProjectManagement(ProjectManagement* pProjectManagement);
int         removeTeamMemberFromArray(Company* pCompany);
void        removeTeamMemberFromArrayInAllProjectManagement(Company* pCompany, TeamMember* pTeamMember);
void        findTeamMemberPlacementAndDelete(ProjectManagement * pProjectManagement, TeamMember* pTeamMember);
int         addCandidate(Company* pCompany);
int         removeCandidate(Company* pCompany);
int         candidateApproval(const Candidate* pCandidate);
int         addClientToArray(Company* pCompany);
void        freeCompany(Company* pCompany);
void        freeTeamMembersArray(Company* pCompany);
void        freeCandidatesArray(Company* pCompany);
void        freeProjectManagementsArray(Company* pCompany);
void        freeClients(Company* pCompany);
TeamMember* findTeamMemberWithTheSameIdInCompany(TeamMember ** TeamMemberArray,int amount, int numIdTeamMember);

//Helper Functions
Client*             findClient(Company* pCompany);
void                sortClient(Company* pCompany);
eSort               getSort();
int                 deleteTeamMemberFromArrayInCompany(Company* pCompany, int num);
int                 deleteCandidateInArray(Company* pCompany, int num);
void                addObjectByUserPreference(Company* pCompany);
void                removeObjectByUserPreference(Company* pCompany);
Candidate*          pickCandidate(const Company* pCompany ,int* choose);
ProjectManagement*  pickProjectManagement(const Company* pCompany);
TeamMember*         pickTeamMember(const Company* pCompany);
void                PrintObjectByUserPreference(Company* pCompany);
int                 payProjectPayments(Company* pCompany);
int addProjectManagementFromFile(Company* pCompany , ProjectManagement * projectManagement , int num);
void freeProjectManagementArrayFileVersion(Company* pCompany, int num);
//Menu
int menuAddCompany();
int menuRemoveCompany();
int menuPrintCompany();

#endif //WEBSITE_MANAGER_COMPANY_H