#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("At least one argument is needed to operate. Arguments provided %i\n", argc-1);
    }

    for (size_t i = 1; i < argc; i++)
    {
        char *argument = argv[i];
        printf("Argument: %s\n",argument);

        char argumentLength = strlen(argument);

        for (size_t j = 0; j < argumentLength; j++)
        {
            if(!isdigit(argument[j]) && !isblank(argument[j])){
               printf("Letter: %c\n",argument[j]);      
            }
           
        }
        
    }

    return EXIT_SUCCESS;
}