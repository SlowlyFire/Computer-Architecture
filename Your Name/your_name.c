#include <stdio.h>
#include <string.h>
#define SIZE 30

int main()
{
    char nameArr[30]="\0";
    printf("hello, what's your name?\n");
    scanf("%s", nameArr);
    printf("your name is: %s\n", nameArr);
    return 0;
}