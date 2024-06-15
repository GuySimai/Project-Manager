#ifndef __GENERAL__
#define __GENERAL__

#include <stddef.h>
#define MAX_STR_LEN 255


typedef enum eSpecialization{eFrontendDeveloper, eBackendDeveloper, eDesigner, eProjectManager, eCEO, eNULL, eNotSorted, eNofOption
} eSpecialization;

static const char* enumSpeciality[eNofOption]
        = { "Front-end Developer", "Back-end Developer",
            "Designer" ,"ProjectManager","CEO","NULL","Not Sorted" };

//--------------------------------------------------------------------
typedef enum {eWaiting, eWorking, eDone, eStatusNofOption} eStatus;

static const char* enumStatus[eStatusNofOption]
        = { "Waiting", "Working on It","Done"};

//--------------------------------------------------------------------


//General Functions
char*	getStrExactName(const char* msg);
char**  splitCharsToWords(char* str, int* pCount, int* pTotalLength);
void    generalArrayFunctionWithNumbers(void* arr, int size, size_t typeSize , void (*f)(const void* element));

//Helper Functions
char*	        getDynStr(char* str);
char*	        myGets(char* buffer, int size);
enum eSpecialization getSpeciality();
void            printSpeciality();
int             getExperience();
int strcicmp(char const* a, char const* b);


#endif
