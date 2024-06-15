#include <stdio.h>

#include "Candidate.h"
#include "Macros.h"


//Functions
int initCandidate(Candidate* pCandidate){
    CHECK_RETURN_INT(pCandidate,"Candidate was not found!");
    welcomeCandidate();
    if(!initContact(&pCandidate->info, "candidate")){
        return 0;
    }
    pCandidate->speciality = getSpeciality();
    pCandidate->experience = getExperience();
    return 1;
}

void        printCandidate(const void* candidate){
    const Candidate* pCandidate = *(const Candidate**) candidate;

    if(!pCandidate){
        printf("Candidate was not found!");
        return;
    }

    printf("Name: %s,\t Speciality: %s,\t Years of experience : %d\nPhone Number: %s\n\n",pCandidate->info.name, enumSpeciality[pCandidate->speciality], pCandidate->experience, pCandidate->info.phoneNumber);
}



void freeCandidate(Candidate* pCandidate) {
    freeContact(&pCandidate->info);
}


//Helper Functions
void welcomeCandidate(){
    printf("\n---CANDIDATE MODE--------\n");
    printf("Thank you for considering our company as a potential employer.\n");
    printf("We're excited about the chance to have you on our team and contribute to our success together.\n");
    printf("We are going to ask you a few questions, Please fill them the best as you can\n");
}

