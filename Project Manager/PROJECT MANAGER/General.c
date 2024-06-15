#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"
#include "Macros.h"

//Helper Functions
enum eSpecialization getSpeciality(){
    
    printf("Please type the number associated with your profession:\n");
    printSpeciality();
    
    
    CHOOSE(choose, 1, 4);
    choose--;
    return (eSpecialization)choose;
}

void printSpeciality(){
    for (int i = 0; i < 4; i++)
        printf("%d - %s\n", i + 1, enumSpeciality[i]);
}


int getExperience(){
    int experience;
    do{
        printf("How many years of experience do you have in this profession?\n");
        int ignore = scanf("%d" ,&experience);
        if(experience < 0 || experience > 50){
            printf("The number of years of experience you can input should be between 0 to 50.\n");
        }
    }while(experience < 0 || experience > 50);

    return experience;
}

//General Functions
char* getStrExactName(const char* msg)
{
    char* str;
    char temp[MAX_STR_LEN];
    printf("%s\n", msg);
    myGets(temp, MAX_STR_LEN);

    str = getDynStr(temp);
    return str;
}

char* getDynStr(char* str)
{
    char* theStr;
    theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
    if (!theStr)
        return NULL;

    strcpy(theStr, str);
    return theStr;
}

char* myGets(char* buffer, int size)
{
    char* ok;
    if (buffer != NULL && size > 0)
    {
        do { //skip only '\n' strings
            ok = fgets(buffer, size, stdin);
        } while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
        if (ok)
        {
            char* back = buffer + strlen(buffer);
            //trim end spaces
            while ((buffer < back) && (isspace(*--back)));
            *(back + 1) = '\0';
            return buffer;
        }
        buffer[0] = '\0';
    }
    return NULL;
}

char** splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
    char temp[255];
    char* delimiters = " ";
    char* word;
    int count = 0;

    strcpy(temp, str);
    char** wordsArray = NULL;
    *pTotalLength = 0;

    word = strtok(temp, delimiters);
    while (word != NULL)
    {
        wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
        if (!wordsArray)
            return 0;
        wordsArray[count] = getDynStr(word);
        count++;
        *pTotalLength += (int)strlen(word);
        word = strtok(NULL, delimiters);
    }
    *pCount = count;
    return wordsArray;
}

int strcicmp(char const* a, char const* b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

void generalArrayFunctionWithNumbers(void* arr, int size, size_t typeSize , void (*f)(const void* element)){
    for (int i = 0; i < size ;i++) {
        printf("%d)", i+1);
        f((char*)arr + i*typeSize);
    }
}
