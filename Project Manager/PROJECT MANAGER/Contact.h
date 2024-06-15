#ifndef WEBSITE_MANAGER_CONTACT_H
#define WEBSITE_MANAGER_CONTACT_H

#define LengthOfPhoneNum 10

typedef struct
{
    char* name;
    char phoneNumber[LengthOfPhoneNum + 1];
    char* email;
}Contact;

//Functions
int     initContact(Contact* pContact, char* str);
int     getContactName(Contact* pContact, char* str);
char*   allocateRightLength(char** wordsArray,int count, int totalLength);
void    changeName(char* name, int count, char** wordsArray);
void    getPhoneNumber(char* phoneNumber);
int     getEmail(Contact* pContact);
int     isValidEmail(const char *email);

//Helper Function
void    printContact(const Contact* pContact);
int     isTheSameContactPhone(Contact* pContact1,Contact* pContact2);
int     isTheSameContactEmail(Contact* pContact1,Contact* pContact2);
void    freeContact(Contact* pContact);


#endif
