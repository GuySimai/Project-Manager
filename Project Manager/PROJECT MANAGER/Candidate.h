#ifndef WEBSITE_MANAGER_CANDIDATE_H
#define WEBSITE_MANAGER_CANDIDATE_H

//Imports
#include "Contact.h"
#include "Date.h"
#include "General.h"

//Candidate struct
typedef struct
{
    int experience;
    Contact info;
    eSpecialization speciality;
}Candidate;


//Functions
int         initCandidate(Candidate* pCandidate);
void        printCandidate(const void* candidate);
void        freeCandidate(Candidate* pCandidate);

//Helper Functions
void        welcomeCandidate();

#endif //WEBSITE_MANAGER_CANDIDATE_H
