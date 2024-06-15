#ifndef WEBSITE_MANAGER_PAYMENT_H
#define WEBSITE_MANAGER_PAYMENT_H

//Imports
#include <stdio.h>
#include "Date.h"

//ID COUNTER
static int paymentID = 1;

//Defines
#define AMOUNT_PER_TASK 150
#define AMOUNT_PER_PROJECT 900

//ENUM
typedef enum {ePending,eCompleted,eNotDecided, eNumofOption} ePayment;
static const char* enumPayment[eNumofOption] = { "Pending", "Completed", "Waiting"};
//--------------------------


//-------Payment Struct-----
typedef struct
{
    unsigned int tasksNumber;
    unsigned int total;
    unsigned int id;
    unsigned int NofPayments;
    Date paymentDate;
    ePayment status;
}Payment;
//-----------------------------


//Functions
int     initPayment(Payment* pPayment, int numOfTasks);
void    printPayment(const Payment* pPayment);
int     payPayment(Payment* p1);
int     payPaymentHelper(Payment* p1);
void    addTasksToTasksNumberPayment(Payment* p1, int plus);
void    deleteTasksToTasksNumberPayment(Payment* p1, int less);
int     savePaymentToBinaryFile(FILE* fp, const Payment* pPayment);
int     readPaymentToBinaryFile(FILE* fp, Payment* pPayment);

#endif //WEBSITE_MANAGER_PAYMENT_H
