#ifndef WEBSITE_MANAGER_COMPANYBINARYFILE_H
#define WEBSITE_MANAGER_COMPANYBINARYFILE_H
#include "Company.h"

//BINARYFILE WRITE
int saveCompanyToBinaryFile(const Company* pCompany, const char* fileName);
void writeContactToBinaryFile(const Contact* pContact, FILE* fp);
int writeClientsArrayToBinaryFile(const Company* pCompany, FILE* fp);
void writeClientToBinaryFile(const Client* pClient, FILE* fp);
int writeCandidatesArrayToBinaryFile(const Company* pCompany, FILE* fp);
void writeCandidateToBinaryFile(const Candidate * pCandidate, FILE* fp);
int writeTeamMembersArrayToBinaryFile(const Company* pCompany, FILE* fp);
void writeTeamMemberToBinaryFile(const TeamMember * pTeamMember, FILE* fp);
int writeProjectManagementsArrayToBinaryFile(const Company* pCompany, FILE* fp);
void writeProjectManagementToBinaryFile(const ProjectManagement * pProjectManagement, FILE* fp);
void writeProjectToBinaryFile(const Project * pProject, FILE* fp);

//BINARYFILE READ
int readCompanyFromBinaryFile( Company* pCompany, const char* fileName);
int readContactFromBinaryFile(Contact* pContact, FILE* fp);
int readClientsArrayFromBinaryFile(Company* pCompany, FILE* fp);
int readClientFromBinaryFile(Client* pClient, FILE* fp);
int readCandidatesArrayFromBinaryFile(Company* pCompany, FILE* fp);
int readCandidateFromBinaryFile(Candidate * pCandidate, FILE* fp);
int readTeamMembersArrayFromBinaryFile(Company* pCompany, FILE* fp);
int readTeamMemberFromBinaryFile(TeamMember* pTeamMember, FILE* fp);
int readProjectManagementsArrayFromBinaryFile(Company* pCompany, FILE* fp);
int readProjectManagementFromBinaryFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp);
int readProjectFromBinaryFile(Project * pProject, FILE* fp);





//BinaryFile Helper
void writeDateToBinaryFile(const Date *date, FILE *fp);
void writeStringToBinaryFile(FILE* file, const char* string);
void writeWebsiteRequirements(char **websiteReq, int reqNumber,FILE *file);
void writePMTeamMembersIDArrayToBinaryFile(const ProjectManagement* pProjectManagement, FILE* fp);
void writePMTaskToBinaryFile(const Task* pTask, FILE* fp);
void writePMTasksToBinaryFile(const ProjectManagement* pProjectManagement, FILE* fp);
void writePaymentToBinaryFile(const Payment *pPayment, FILE *fp);


//BinaryRead Helper
char* readStringFromFile(FILE *file);
int readWebsiteReqBinary(Project *pProject, FILE *fp);
int readPMTeamMembersIDArrayFromBinaryFile(ProjectManagement* pProjectManagement, TeamMember** teamMemberArr, int amount,  FILE* fp);
int readTasksArrayFromBinaryFile(ProjectManagement* pProjectManagement, FILE* fp);
int readyTaskFromBinaryFile(ProjectManagement* pProjectManagement,Task* pTask,FILE* fp);
int readPaymentFromBinaryFile(Payment * pPayment, FILE* fp);

#endif //WEBSITE_MANAGER_COMPANYBINARYFILE_H
