#ifndef WEBSITE_MANAGER_FEEDBACK_H
#define WEBSITE_MANAGER_FEEDBACK_H

#include "Client.h"
#include "Date.h"

//-------Feedback Struct-----
typedef struct
{
    char* feedbackStr;//255 * 8
    unsigned int rate;// 0000 0101
}Feedback;
//-----------------------------

//Functions
int         initFeedback(Feedback* pFeedback , Client* client);
void        printFeedback(const Feedback* pFeedback);
int         editFeedback(Feedback* pFeedback);
void        freeFeedback(Feedback* pFeedback);


//Helper Functions
void        getRate(Feedback* pFeedback);
char*       getFeedbackString(Feedback* pFeedback);
void writeFeedbackToBinaryFile(const Feedback* pFeedback, FILE* fp);
int readFeedbackFromBinaryFile(Feedback* pFeedback, FILE* fp);


#endif //WEBSITE_MANAGER_FEEDBACK_H
