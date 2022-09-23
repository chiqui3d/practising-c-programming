
#include "../include/h_debug.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    FILE *input = NULL;
    char *block = NULL;

    block = malloc(100);
    check_mem(block); // should work

    log_info("It happened %f times today.", 1.3f);
    debug("I am %d years old.", 37);

    input = fopen("ex19.c", "r");
    check(input, "Failed to open %s.", "ex19.c");

    return EXIT_SUCCESS;

error:
    if (block) {
        printf("GOTO DONE\n");
        free(block);
    }
    if (input) {
        fclose(input);
    }
    return EXIT_FAILURE;
}
