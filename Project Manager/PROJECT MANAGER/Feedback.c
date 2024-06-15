#include <stdio.h>
#include <stdlib.h>
#include "Feedback.h"
#include "Macros.h"

//Functions
int         initFeedback(Feedback* pFeedback , Client* client){
    CHECK_RETURN_INT(pFeedback,"Feedback was not found!")
    pFeedback->feedbackStr = getFeedbackString(pFeedback);
    return 1;
}

void      printFeedback(const Feedback* pFeedback){
    printf("\n");
    if(!pFeedback) {
        printf("Feedback not found!\n");
        return;
    }

    printf("Feedback Details are:\n");
    printf("Rate : %d\n",pFeedback->rate);
    printf("Review: %s \n",pFeedback->feedbackStr);
}

int         editFeedback(Feedback* pFeedback){
    CHECK_RETURN_INT(pFeedback,"Feedback was not found!");
    printf("\n\n");
    printf("EDIT FEEDBACK MODE\n");
    getRate(pFeedback);
    pFeedback->feedbackStr = getStrExactName("What is your new Feedback?");

    return 1;
}



void         freeFeedback(Feedback* pFeedback){
    free(pFeedback->feedbackStr);
}

//Helper Functions
void getRate(Feedback* pFeedback){
    int rate;
    printf("Between 1-5 what is your satisfaction from our service?\n");
    int ignore = scanf("%d" ,&rate);
    while(rate < 1 || rate > 5){
        printf("Invalid Input, Between 1-5 what is your satisfaction from our service?\n");
        ignore = scanf("%d" ,&rate);
    }
    pFeedback->rate =rate;

}

char* getFeedbackString(Feedback* pFeedback){
    int choice;
    printf("\n------CLIENT MODE--------\n");
    printf("Thank you for buying our service, Can you please leave a review for us?\n");
    printf("Note: Every review helps us to get better and growth.\n");
    printf("Please type 1-Yes or 0-No?\n");

    int ignore = scanf("%d" ,&choice);
    while(choice != 0 && choice != 1){
        printf("Invalid Input, Would you like to leave a review?(1-Yes , 0 - No)\n");
        ignore = scanf("%d" ,&choice);
    }
    if(choice == 1){
        getRate(pFeedback);
        return getStrExactName("What can you say about our service?");
    }

    printf("Thank you anyways!\n");
    return NULL;

}


void writeFeedbackToBinaryFile(const Feedback* pFeedback, FILE* fp) {
    
    size_t feedbackStrLength = strlen(pFeedback->feedbackStr);

    BYTE data[2];
    data[0] = (pFeedback->rate) | (feedbackStrLength >> 3);
    data[1] = (feedbackStrLength >> 3);

    if (fwrite(&data, sizeof(BYTE), 2, fp) != 2) {
        return;
    }

    fwrite(pFeedback->feedbackStr, sizeof(char), feedbackStrLength, fp);
}

int readFeedbackFromBinaryFile(Feedback* pFeedback, FILE* fp) {
    BYTE data[2];
    if (fread(&data, sizeof(BYTE), 2, fp) != 2) {
        return 0; 
    }
    pFeedback->rate = (data[0] >> 5); 
    int feedbackStrLength = (data[0] << 3) | (data[1] >> 5); 
    pFeedback->feedbackStr = (char*)malloc(feedbackStrLength + 1); 
    if (!pFeedback->feedbackStr) {
        return 0; 
    }

    if (fread(pFeedback->feedbackStr, sizeof(char), feedbackStrLength, fp) != feedbackStrLength) {
        free(pFeedback->feedbackStr);
        return 0; 
    }
    pFeedback->feedbackStr[feedbackStrLength] = '\0'; 
    return 1; 
}