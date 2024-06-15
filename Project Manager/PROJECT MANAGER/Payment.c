#include "Payment.h"
#include "Macros.h"

int     initPayment(Payment* pPayment, int numOfTasks){
    CHECK_RETURN_INT(pPayment,"Payment was not found!");
    pPayment->id = paymentID++;
    pPayment->tasksNumber = numOfTasks;
    pPayment->total =AMOUNT_PER_PROJECT + pPayment->tasksNumber * AMOUNT_PER_TASK;
    pPayment->status = eNotDecided;
    pPayment->NofPayments = 0;
    return 1;
}


void    printPayment(const Payment* pPayment){
    if(!pPayment) {
        printf("Payment not found!");
        return;
    }
    printf("Tasks Amount: %d\tLeft To Pay: %d\tStatus: %s\n",pPayment->tasksNumber,pPayment->total,enumPayment[pPayment->status]);

    if(pPayment->status == eCompleted){
        printf("Receive ");
        printDate(&pPayment->paymentDate);
    }
    printf("\n\n");
}

int payPayment(Payment* p1){
    if(p1->total == 0){
        printf("The payment already been payed!\n");
        return 0;
    }
    if(p1->status == eNotDecided){
        printf("\n---CLIENT MODE--------\n");
        printf("The cost of your website is : %d\n", p1->total);
        printf("And it consists from ");
        printf("Tasks Amount: %d \t Amount Per Task: %d\n\n",p1->tasksNumber,AMOUNT_PER_TASK);
        printf("How many payments would you like to divide the amount into? (max 3 payments)\n");
        printf("Please keep in mind that we cannot deliver the website until the whole amount has been payed\n");
        do {
            int ignore = scanf("%d",&p1->NofPayments);
            if(p1->NofPayments > 3 || p1->NofPayments<1) printf("We only can divide into 1-3 payments");
        }while(p1->NofPayments > 3 || p1->NofPayments<1);
        p1->status = ePending;
    }
    return payPaymentHelper(p1);
}

int payPaymentHelper(Payment* p1){
    int pay;
    if(p1->NofPayments == 1){
        p1->status = eCompleted;
        printf("You will be charged for %d  , Whole Payment has been payed!.\n",p1->total );
        pay = p1->total;
        p1->total = 0;
        getCorrectDate(&p1->paymentDate);
    }
    else {
        printf("The whole amount left: %d , You will now be charge for %d\n", p1->total , p1->total/p1->NofPayments);
        pay = p1->total/p1->NofPayments;
        p1->total = MINUS(p1->total, p1->total/p1->NofPayments);
        p1->NofPayments--;
    }

    return pay;
}

void addTasksToTasksNumberPayment(Payment* p1, int plus){
    p1->tasksNumber = p1->tasksNumber + plus;
    p1->total = p1->total + (AMOUNT_PER_TASK * plus);
}

void deleteTasksToTasksNumberPayment(Payment* p1, int less){
    p1->tasksNumber = MINUS(p1->tasksNumber , less);
    p1->total = MINUS(p1->total, AMOUNT_PER_TASK * less);
}

//BITWISE OPERATIONS
int     savePaymentToBinaryFile(FILE* fp, const Payment* pPayment) {

    CHECK_RETURN_INT(pPayment, "Payment was not found!")

    BYTE data[4];
    data[0] = (pPayment->id << 2) | (pPayment->tasksNumber >> 2);
    data[1] = (pPayment->tasksNumber << 6) | (pPayment->total >> 6);
    data[2] = (pPayment->total << 2) | pPayment->NofPayments;
    data[3] =  (pPayment->status << 6);

    
    if (fwrite(data, sizeof(BYTE), 4, fp) != 4)
    {
        return 0;
    }
    if (pPayment->status == 1) {
        saveDateToFile(fp, &pPayment->paymentDate);
    }

    return 1;
}

int     readPaymentToBinaryFile(FILE* fp,  Payment* pPayment) {

    CHECK_RETURN_INT(pPayment, "Payment was not found!")

    BYTE data[4];
    if (fread(data, sizeof(char), 4, fp) != 4)
    {
        return 0;
    }

    pPayment->id = (data[0] >> 2);
    pPayment->tasksNumber = ((data[0] & 0x03) << 2) | (data[1] >> 6);
    pPayment->total = ((data[1] & 0x3F) << 6) | (data[2] >> 2);
    pPayment->NofPayments = (data[2] & 0x03);
    pPayment->status = (data[3] >>6) & 0x3;

    if (pPayment->status == 1) {
        readDateFromBinaryFile(&pPayment->paymentDate, fp);
    }
        

    return 1;
}