#include <stdio.h>
#include <string.h>

#include "General.h"
#include "Date.h"
#include "Macros.h"


const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '/'

void getCorrectDate(Date* pDate)
{
    char date[MAX_STR_LEN];
    int ok;

    do {
        printf("Enter Date dd%cmm%cyyyy  minimum year %d\t",
               SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
        myGets(date, MAX_STR_LEN);
        ok = checkDate(date, pDate);
        if (!ok)
            printf("Error try again\n");
    } while (!ok);
}

int	 checkDate(char* date, Date* pDate)
{
    CHECK_RETURN_INT(pDate,"Date was not found!")
    int day, month, year;
    if (strlen(date) != 10)
        return 0;
    if ( (date[2] != SPECIAL_TAV) || (date[5] != SPECIAL_TAV)) return 0;
    int ignore = sscanf(date, "%d%*c%d%*c%d", &day, &month, &year);
    if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
        return 0;

    if (day > DAY_MONTHS[month - 1])
        return 0;

    pDate->day = day;
    pDate->month = month;
    pDate->year = year;

    return 1;
}

void printDate(const Date* pDate)
{
    printf("Date: %d/%d/%d", pDate->day, pDate->month, pDate->year);
}


int saveDateToFile(FILE* fp, const Date* pDate)
{
    CHECK_RETURN_INT(pDate,"Date was not found!")

    BYTE data[3];


    data[0] = pDate->year >> 7;
    data[1] = (pDate->month >> 3) | (pDate->year << 1);
    data[2] = pDate->day | (pDate->month << 5);

    if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
    {
        return 0;
    }
    return 1;
}


int readDateFromBinaryFile(Date* pDate, FILE* fp)
{
    CHECK_RETURN_INT(pDate,"Date was not found!")

    BYTE data[3];
    if (fread(data, sizeof(char), 3, fp) != 3)
    {
        return 0;
    }

    pDate->year = (data[1] >> 1) | ((data[0] & 0xF) << 7);
    pDate->month =  (data[2] >> 5) | ((data[1] & 0x1) << 3);
    pDate->day = data[2] & 0x1F;

    return 1;
}