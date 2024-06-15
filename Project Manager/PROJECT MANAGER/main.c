#include "Macros.h"
#include "CompanyTextFile.h"
#include "CompanyBinaryFile.h"

#define COMPANY_FILE_NAME_TEXT "company.txt"
#define COMPANY_FILE_NAME_BIN "company.bin"

typedef enum{
    ePrintCompany, ePrintComponent, eAddComponent,eRemoveComponent ,eSortClient, eFindClient,
    eToProjectManagementSystem , ePayPayment ,  eNofMenuOptions} eMenuOptionsMenu;

const char* str[eNofMenuOptions] = { "Print Company","Print Component Of The Company","Add Component To The Company",
                                     "Remove Component To The Company" ,"Sort Client", "Find Client",
                                     "Enter To Project Management System" , "Pay payment"};


void initCompanyFromFileByChoice(Company* pCompany, const char* fileName);
int menu();

int main()
{

    Company CP;
    initCompanyFromFileByChoice(&CP, COMPANY_FILE_NAME_TEXT);

    Client* client;
    int option;
    int stop = 0;
    do
    {
        option = menu();
        PRINT_SEPARATION()
        switch (option)
        {
            case ePrintCompany:
                printCompany(&CP);
                break;

            case ePrintComponent:
                PrintObjectByUserPreference(&CP);
                break;

            case eAddComponent:
                addObjectByUserPreference(&CP);
                break;

            case eRemoveComponent:
                removeObjectByUserPreference(&CP);
                break;

            case eSortClient:
                sortClient(&CP);
                break;

            case eFindClient:
                client = findClient(&CP);
                if(client){
                    printClient(client);
                }
                break;

            case eToProjectManagementSystem:
                if(CP.numOfProjects == 0){
                    printf("No Projects Available\n");
                    break;
                }
                projectManagementMenu(pickProjectManagement(&CP));
                break;

            case ePayPayment:
                payProjectPayments(&CP);
                break;

            case EXIT:
                saveCompanyToFile(&CP, COMPANY_FILE_NAME_TEXT);
                saveCompanyToBinaryFile(&CP,COMPANY_FILE_NAME_BIN);
                printf("save to files.\n");
                printf("Goodbye!\n");
                stop = 1;
                break;

            default:
                printf("Wrong option\n");
                break;
        }
        PRINT_SEPARATION()
    } while (!stop);

    freeCompany(&CP);

    return 1;
}

int menu()
{
    int option;
    printf("\n\n");
    printf("Please choose one of the following options\n");
    for(int i = 0 ; i < eNofMenuOptions ; i++)
        printf("%d - %s\n",i,str[i]);
    printf("%d - Quit\n", EXIT);
    int ignore = scanf("%d", &option);
    //clean buffer
    char tav;
    ignore = scanf("%c", &tav);
    return option;
}


void initCompanyFromFileByChoice(Company* pCompany, const char* fileName)
{
  
    FILE* fp1;
    FILE* fp2;
    fp1 = fopen(fileName, "r");
    fp2 = fopen(fileName, "rb");

    if(!fp1 && !fp2){
        initCompany(pCompany);
    } else{
        printf("To initiate from Text File enter 1, To initiate from binary File enter 2\n");
         CHOOSE(num,1,2);
        if (num== 1){
            readCompanyFromFile(pCompany,COMPANY_FILE_NAME_TEXT);
        } else {
            readCompanyFromBinaryFile(pCompany,COMPANY_FILE_NAME_BIN);
        }
    }
}