#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/h_prompt.h"


int main(void) {

    /* Name ----------------------------- */
    char *name = getString("Nombre:");

    printf("Name choose: %s\n", name);

    free(name);
    
    /* Age ----------------------------- */

    unsigned short int age = getInt("Edad:");

    printf("Age choose: %hu\n", age);


    /* Address ------------------------- */

    char *address = getString("Address:");

    printf("Address choose: %s\n", address);

    free(address);


    /* Country ------------------------- */
    printf("Country:\n");
    char *country = getLine();
    printf("Country choose: %s\n", country);

    free(country);

   

    return EXIT_SUCCESS;   
}