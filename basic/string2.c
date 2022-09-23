#include <stdio.h>
#include <string.h>

char *returnString(){

    char string[] = "Hello papi";
    char *stringMalloc = malloc(strlen(string) + 1);
    strcpy(stringMalloc, string);

    return stringMalloc;
}

int main(void)
{
    char string[] = "Hello word";

    printf("%s\n",string);
    printf("%c\n",*string);
    printf("%-20.5s\n", string);

     int a = 0;
     scanf("%d", &a); // stdin is 45
     printf("%d\n", a);
     for (int i = 0; i < strlen(string); i++)
     {
         printf("%c\n", string[i]);
     }

     return 0;


}
