#ifndef WEBSITE_MANAGER_COMPANYTEXTFILE_H
#define WEBSITE_MANAGER_COMPANYTEXTFILE_H
#include "Company.h"


int     saveCompanyToFile(const Company* pManager, const char* fileName);
void    writeClientsArrayCompanyToFile(FILE* fp, const Client* pClient, int clientsNum);
void    writeContactToFile(const Contact* pContact, FILE* fp);
void    writeClientToFile(const Client* pClient, FILE* fp);
void    writeCandidatesArrayCompanyToFile(FILE* fp, const Candidate** pCandidate, int CandidatesNum);
void    writeCandidateToFile(const Candidate * pCandidate, FILE* fp);
void    writeTeamMembersArrayCompanyToFile(FILE* fp, const TeamMember** pTeamMember, int TeamMembersNum);
void    writeTeamMemberToFile(const TeamMember* pTeamMember, FILE* fp);
void    writeProjectManagementArrayCompanyToFile(FILE* fp, const ProjectManagement** ProjectManagement, int ProjectManagementNum);
void    writeDateToFile(const Date* pDate ,FILE* fp );
void    writeProjectToFile(const Project* pProject, FILE* fp);
void    writePMTeamMembersIDArrayToFile(const ProjectManagement* pProjectManagement, FILE* fp);
void    writeTaskToFile(const Task* pTask, FILE* fp);
void    writeProjectManagementToFile(const ProjectManagement* pProjectManagement, FILE* fp);
void    writeClientWebsiteRequirements(const Project* pProject, FILE* fp);
void    writePMTasksToFile(const ProjectManagement * pProjectManagement, FILE* fp);
void    writePaymentToFile(const Payment* pPayment, FILE* fp);



int     readCompanyFromFile(Company* pCompany, const char* fileName);
int     readContactFromFile(Contact* pContact, FILE* fp);
int     readClientFromFile( Client* client, FILE* fp);
int     readClientsArrayFromFile( Company* pCompany, FILE* fp);
int     readCandidatesArrayCompanyFromFile(Company* pCompany, FILE* fp);
int     readCandidateCompanyFromFile(Candidate * pCandidate, FILE* fp);
int     readTeamMemberCompanyFromFile(TeamMember* pTeamMember, FILE* fp);
int     readTeamMembersArrayCompanyFromFile(Company* pCompany, FILE* fp);
int     readProjectManagementsArrayCompanyFromFile(Company* pCompany, FILE* fp);
int     readProjectManagementFromFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr,int amount, FILE* fp);
int     readWebsiteReq(Project * pProject, FILE* fp);
int     readProjectFromFile(Project * pProject, FILE* fp);
int     readDateFromTextFile(Date * pDate, FILE* fp);
int     readFeedbackFromTextFile(Feedback * pFeedback, FILE* fp);
int     readTeamMembersIDFromFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp);
int     readPMTasksFromFile(ProjectManagement* pProjectManagement, FILE* fp);
int     readPMTaskFromFile(Task* pTask, ProjectManagement* pProjectManagement, FILE* fp);
int     readPaymentFromFile(Payment* pPayment, FILE* fp);

//HELPER
void    removeNewLine(char *str);
int     addClientFromFile(Company* pCompany , Client* client , int num);
void    freeClientArrayFileVersion(Company* pCompany,int num);
void    freeCandidatesArrayFileVersion(Company* pCompany,int);
int     addCandidateFromFile(Company* pCompany , Candidate * candidate , int num);
void    freeTeamMembersArrayCompanyFileVersion(Company* pCompany,int num);
int     addTeamMemberFromFile(Company* pCompany , TeamMember* pTeamMember , int num);
void    freeTeamMembersArrayPMFileVersion(ProjectManagement* pProjectManagement,int num);



#endif //WEBSITE_MANAGER_COMPANYTEXTFILE_H
