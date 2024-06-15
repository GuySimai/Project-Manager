#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "General.h"
#include "Contact.h"
#include "Macros.h"

int     initContact(Contact* pContact, char* str){
    CHECK_RETURN_INT(pContact,"Contact was not found!")
    if (!getContactName(pContact, str))
        return 0;
    getPhoneNumber(pContact->phoneNumber);
    if (!getEmail(pContact)){
        free(pContact->name);
        return 0;
    }
    return 1;
}

int     getContactName(Contact* pContact, char* str){
    CHECK_RETURN_INT(pContact,"Contact was not found!")
    char temp[255];
    char** wordsArray = NULL;
    int totalLength = 0;
    int count = 0;

    while (count == 0)
    {
        printf("Enter %s name\n", str);
        myGets(temp, MAX_STR_LEN);
        wordsArray = splitCharsToWords(temp, &count, &totalLength);
    }
    pContact->name = allocateRightLength(wordsArray, count, totalLength);
    if (!pContact->name)
        return 0;

    changeName(pContact->name,count, wordsArray);

    //clean temp data
    for (int i = 0; i < count; i++)
        free(wordsArray[i]);
    free(wordsArray);
    return 1;
}

char* allocateRightLength(char** wordsArray, int count, int totalLength) {
    // Calculate the total length required for the name
    for (int i = 0; i < count; i++)
        totalLength += strlen(wordsArray[i]);

    char* name = (char*)calloc(totalLength + count, sizeof(char)); // + count for space characters
    if (!name)
        return NULL;

    return name;
}

void changeName(char* name, int count, char** wordsArray) {
    for (int i = 0; i < count; i++) {
        wordsArray[i][0] = toupper(wordsArray[i][0]);
        strcat(name, wordsArray[i]);
        if (i != count - 1) // Not the last word
            strcat(name, " ");
    }
}


void getPhoneNumber(char* phoneNumber) {
    char temp[MAX_STR_LEN];
    int ok = 1;
    do {
        ok = 1;
        printf("Enter phone number with %d digits\n", LengthOfPhoneNum);
        myGets(temp, MAX_STR_LEN);
        if (strlen(temp) != LengthOfPhoneNum)
        {
            printf("Phone number should be %d digits\n", LengthOfPhoneNum);
            ok = 0;
        }
        else {
            for (int i = 0; i < LengthOfPhoneNum; i++){
                if (temp[i] < '0' || temp[i] > '9') {
                    printf("Please enter only digits\n");
                    ok = 0;
                    break;
                }
            }
        }
    } while (!ok);
    strcpy(phoneNumber, temp);
}


int     getEmail(Contact* pContact){
    CHECK_RETURN_INT(pContact,"Contact was not found!")
    char temp[255];
    do{
        printf("Enter your email, like __@__._\n");
        myGets(temp, MAX_STR_LEN);
        isValidEmail(temp);
    } while (!isValidEmail(temp));

    char* str = getDynStr(temp);

    if (!str){
        return 0;
    }

    pContact->email = str;
    return 1;
}
int isValidEmail(const char *email) {
    int length = strlen(email);
    int atIndex = -1;
    int dotIndex = -1;

    if (length < 5)
        return 0;
    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            if (atIndex != -1 || i == 0 || i == length - 1)
                return 0;
            atIndex = i;
        } else if (email[i] == '.') {
            if (i < atIndex) // Dot should come after @
                return 0;
            dotIndex = i;
        }
    }

    // Check if dot exists after @ and it's not the last character
    if (dotIndex == -1 || dotIndex == atIndex + 1 || dotIndex == length - 1)
        return 0;

    return 1;
}

void    printContact(const Contact* pContact){
    printf("Name:%s\t Phone number: %s\t Email: %s\n",pContact->name, pContact->phoneNumber, pContact->email);
}

void     freeContact(Contact* pContact){
    free(pContact->name);
    free(pContact->email);
}
