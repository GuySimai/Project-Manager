#ifndef WEBSITE_MANAGER_MACROS_H
#define WEBSITE_MANAGER_MACROS_H

// MACROS
#define CHECK_RETURN_INT(ptr, str) { \
    if (!ptr) { \
        printf("%s\n", str); \
        return 0; \
    } \
}
#define MINUS(x,y)	((x)-(y))
#define CHOOSE(num,x,y) int num;while (1) { int res = scanf("%d", &num); if (num >= (x) && num <= (y)) {break; }printf("Invalid number. Please choose again.\n"); }
#define ZERO_COUNTER(ptr,str) { if(ptr == 0){printf("%s\n", str); return 0;}}
#define PRINT_SEPARATION() { printf("---------------------------------------------------------------------------------------------------\n");}
#define BREAK_IF_0(num,str) { if(num == 0){printf("%s\n", str); break;}}


#endif //WEBSITE_MANAGER_MACROS_H
