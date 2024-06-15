#ifndef WEBSITE_MANAGER_DATE_H
#define WEBSITE_MANAGER_DATE_H

#define MIN_YEAR 2023
#include <stdio.h>
typedef unsigned char BYTE;

typedef struct
{
    unsigned int day;  //1->31
    unsigned int month;//1->12
    unsigned int year; //0 - 2047

}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);

int     saveDateToFile( FILE* fp,const Date* pDate);
int     readDateFromBinaryFile(Date* pDate, FILE* fp);


#endif //WEBSITE_MANAGER_DATE_H
